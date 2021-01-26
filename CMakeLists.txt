cmake_minimum_required(VERSION 3.2)
project(c4)

set(CMAKE_CXX_STANDARD 17)


#include(/media/hd/workspace/c4/llvm/install/lib/cmake/llvm/LLVMConfig.cmake)

#llvm_config()

if(EXISTS "/media/hd/workspace/c4/llvm/install/bin/llvm-config")
    set(LLVM_CONFIG /media/hd/workspace/c4/llvm/install/bin/llvm-config)
else()
    set(LLVM_CONFIG llvm-config)
endif()


execute_process(COMMAND ${LLVM_CONFIG} --cxxflags OUTPUT_VARIABLE LLVM_CFLAGS)
execute_process(COMMAND ${LLVM_CONFIG} --ldflags OUTPUT_VARIABLE LLVM_LINKER_FLAGS)
execute_process(COMMAND ${LLVM_CONFIG} --system-libs OUTPUT_VARIABLE LLVM_LIBS_FLAGS)
execute_process(COMMAND ${LLVM_CONFIG} --libs core OUTPUT_VARIABLE LLVM_CORE_LIBS_FLAGS)
execute_process(COMMAND ${LLVM_CONFIG} --includedir OUTPUT_VARIABLE LLVM_INCLUDES)


string(REPLACE "-I" "-isystem" LLVM_CFLAGS ${LLVM_CFLAGS})
string(REPLACE "-std=c++14" "" LLVM_CFLAGS ${LLVM_CFLAGS})
string(REPLACE " " "" LLVM_CFLAGS ${LLVM_CFLAGS})
string(REPLACE "-L" "" LLVM_LINKER_FLAGS ${LLVM_LINKER_FLAGS})
string(REPLACE "-l" "" LLVM_LIBS_FLAGS ${LLVM_LIBS_FLAGS})
string(REPLACE "-l" "" LLVM_CORE_LIBS_FLAGS ${LLVM_CORE_LIBS_FLAGS})

string(REGEX REPLACE "[\r\n ]+$" "" LLVM_CFLAGS ${LLVM_CFLAGS})
string(REGEX REPLACE "[\r\n ]+$" "" LLVM_LINKER_FLAGS ${LLVM_LINKER_FLAGS})
string(REGEX REPLACE "[\r\n ]+$" "" LLVM_LIBS_FLAGS ${LLVM_LIBS_FLAGS})
string(REGEX REPLACE "[\r\n ]+$" "" LLVM_CORE_LIBS_FLAGS ${LLVM_CORE_LIBS_FLAGS})
string(REGEX REPLACE "[\r\n ]+$" "" LLVM_INCLUDES ${LLVM_INCLUDES})

string(REPLACE " " ";" LLVM_LIBS "${LLVM_LIBS_FLAGS} ${LLVM_CORE_LIBS_FLAGS}")

message(${LLVM_LIBS_FLAGS} ${LLVM_CORE_LIBS_FLAGS})
add_subdirectory(lib/googletest)

include_directories(lib/googletest)
include_directories(lib/googletest/googletest/include)
include_directories(lib/googletest/googlemock/include)


SET(CMAKE_CXX_FLAGS "-Wall -W -std=c++17 -O3 -g -Wno-unused-parameter  ${LLVM_CFLAGS} -pthread")

file(GLOB_RECURSE source_list "src/*.cpp" )

link_directories(${LLVM_LINKER_FLAGS})
include_directories(${LLVM_INCLUDES})
link_libraries(${LLVM_LIBS})

add_executable(c4 ${source_list} main.cpp )
add_executable(build ${source_list} build.cpp src/types/Types.cpp src/elements/JumpingCompare.cpp src/elements/JumpingCompare.h src/common.h)
add_executable(testC4 ${source_list} tests/main.cpp)


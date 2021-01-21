cmake_minimum_required(VERSION 3.2)
project(c4)

set(CMAKE_CXX_STANDARD 17)


add_subdirectory(lib/googletest)

include_directories(lib/googletest)
include_directories(lib/googletest/googletest/include)
include_directories(lib/googletest/googlemock/include)

#SET(CMAKE_EXE_LINKER_FLAGS "-L/media/hd/workspace/c4/llvm/install/lib  -lz -lrt -ldl -lpthread -lm -pthread -lLLVMCore -lLLVMRemarks -lLLVMBitstreamReader -lLLVMBinaryFormat -lLLVMSupport -lLLVMDemangle")
SET(CMAKE_CXX_FLAGS "-Wall -W -std=c++17 -O0 -g -Wno-unused-parameter -isystem/media/hd/workspace/c4/llvm/install/include -D_GNU_SOURCE -D_DEBUG -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS -pthread")
#SET(CMAKE_C_FLAGS "-Wall -W -std=c++17 -O0 -g -Wno-unused-parameter -isystem/media/hd/workspace/c4/llvm/install/include -D_GNU_SOURCE -D_DEBUG -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS")

#add_compile_options("-Wall -W -std=c++17 -O0 -g -Wno-unused-parameter -isystem/media/hd/workspace/c4/llvm/install/include -D_GNU_SOURCE -D_DEBUG -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS")


file(GLOB_RECURSE source_list "src/*.cpp" )

link_directories(/media/hd/workspace/c4/llvm/install/lib)
include_directories(/media/hd/workspace/c4/llvm/install/include/)
link_libraries(z rt dl pthread m LLVMCore LLVMRemarks LLVMBitstreamReader LLVMBinaryFormat LLVMSupport LLVMDemangle)

add_executable(c4 ${source_list} main.cpp)
add_executable(build ${source_list} build.cpp)
add_executable(testC4 tests/main.cpp)

target_link_libraries(testC4 gtest gtest_main)

add_executable(llvmExample examples/custom.cpp)

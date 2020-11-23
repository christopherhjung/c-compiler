cmake_minimum_required(VERSION 3.2)
project(c4)

set(CMAKE_CXX_STANDARD 17)


add_subdirectory(lib/googletest)

include_directories(lib/googletest)
include_directories(lib/googletest/googletest/include)
include_directories(lib/googletest/googlemock/include)

set(CMAKE_CXX_FLAGS_DEBUG_INIT "-std=c++17 -g -O0")
set(CMAKE_CXX_FLAGS_RELEASE_INIT "-std=c++17 -O2 -DNDEBUG")

add_executable(c4 src/main.cpp src/parser/Rule.cpp src/parser/Entity.cpp src/parser/Component.cpp src/parser/Item.cpp)
add_executable(build src/build.cpp src/parser/Rule.cpp src/parser/Entity.cpp src/parser/Component.cpp)

add_executable(testC4 tests/main.cpp)

target_link_libraries(testC4 gtest gtest_main)



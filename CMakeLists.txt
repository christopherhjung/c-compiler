cmake_minimum_required(VERSION 3.2)
project(c4)

set(CMAKE_CXX_STANDARD 17)


add_subdirectory(lib/googletest)

include_directories(lib/googletest)
include_directories(lib/googletest/googletest/include)
include_directories(lib/googletest/googlemock/include)


add_executable(c4 src/main.cpp src/parser/Rule.cpp src/parser/Entity.cpp src/parser/Component.cpp src/parser/Item.cpp)
add_executable(build src/build.cpp src/parser/Rule.cpp src/parser/Entity.cpp src/parser/Component.cpp)

add_executable(test tests/main.cpp)

target_link_libraries(test gtest gtest_main)



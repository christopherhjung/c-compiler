cmake_minimum_required(VERSION 3.2)
project(c4)

set(CMAKE_CXX_STANDARD 14)

file(GLOB SRC
        "src/*.h"
        "src/*.cpp"
        )

add_executable(c4 ${SRC})
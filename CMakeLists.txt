cmake_minimum_required(VERSION 3.2)
project(c4)

set(CMAKE_CXX_STANDARD 17)

file(GLOB RES
        "resources/*"
        )

foreach(FILE ${RES})
    file(RELATIVE_PATH REL ${PROJECT_SOURCE_DIR} ${FILE})
    configure_file(${REL} ${REL} COPYONLY)
endforeach()

add_executable(c4 src/main.cpp)
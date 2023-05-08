cmake_minimum_required(VERSION 3.4)


set(CMAKE_CXX_FLAGS )

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
# project name
project(Geometric
    VERSION 0.1.0
    DESCRIPTION "Geometric-Query-Library"
    LANGUAGES CXX
)

add_subdirectory(Geometric)
add_subdirectory(tests)


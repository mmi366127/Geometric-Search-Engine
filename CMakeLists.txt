cmake_minimum_required(VERSION 3.14)

# project name
project(Geometric
    VERSION 0.1.0
    DESCRIPTION "Geometric-Query-Library"
    LANGUAGES CXX
)

enable_testing()

# SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -fPIC -D__DEBUG__")
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -fPIC")

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)


find_package(pybind11 CONFIG REQUIRED)

add_subdirectory(Geometric)
add_subdirectory(python)
add_subdirectory(tests)

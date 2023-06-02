set(GEOMETRY_HEADERS
    include/distance.h
    include/geometric.h
    include/types.h
    # add more .h files here as you create them
)

set(GEOMETRY_SOURCES
    source/distance.cpp
    source/geometric.cpp
    # add more .cpp files here as you create them
)

add_library(geometry_search ${GEOMETRY_HEADERS} ${GEOMETRY_SOURCES})
target_include_directories(geometry_search PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
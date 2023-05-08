
add_library(geometric STATIC
    include/types.h
    include/distance.h    
    source/distance.cpp
    source/geometric.cpp
    include/geometric.h
)

target_include_directories(geometric PUBLIC include/)

add_executable(test_geometry_search test_geometry_search.cpp)
target_link_libraries(test_geometry_search PRIVATE geometry_search)

# This is a CMake-provided command for discovering tests in executables
include(CTest)
add_test(NAME test_geometry_search COMMAND test_geometry_search)
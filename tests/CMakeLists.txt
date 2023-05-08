add_executable(tests source/test.cpp)
target_link_libraries(tests PRIVATE geometric)



enable_testing()
add_test(NAME MyTest COMMAND test)



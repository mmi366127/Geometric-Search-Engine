add_test(NAME test_correctness
         COMMAND ${PYTHON_EXECUTABLE} -m pytest ${CMAKE_CURRENT_SOURCE_DIR}/test_correctness.py -r A)

add_test(NAME test_performance
         COMMAND ${PYTHON_EXECUTABLE} -m pytest ${CMAKE_CURRENT_SOURCE_DIR}/test_performance.py -r A)

add_test(NAME test_pybind_module
         COMMAND ${PYTHON_EXECUTABLE} -m pytest ${CMAKE_CURRENT_SOURCE_DIR}/python/test_pybind_module.py)
target_include_directories(hiemal PUBLIC ${CMAKE_BINARY_DIR}/src/core/ops)

target_sources(hiemal PRIVATE ${CMAKE_BINARY_DIR}/src/core/ops/ops.c)

if(WITH_PYTHON)
  target_sources(hm_python_wrapper PRIVATE ${CMAKE_BINARY_DIR}/src/core/ops/ops_python.c)
endif()
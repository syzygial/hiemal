target_include_directories(hiemal PUBLIC ${CMAKE_BINARY_DIR}/src/core/ops)

target_sources(hiemal PRIVATE ${CMAKE_BINARY_DIR}/src/core/ops/ops.c)
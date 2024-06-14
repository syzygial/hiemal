SET(TEST_FILES 
  test_buffer.c 
  test_dsp.c 
  test_math.c 
  test_io.c 
  test_common.c
  test_semaphore.c
  test_thread.c
  test_event.c
  test_args.c
  test_str.c
  test_file.c
  test_op_filter.c)

if(WITH_PROTOBUF)
  list(APPEND TEST_FILES test_pb_c_wrap.c)
endif()

if(WITH_ALSA)
  list(APPEND TEST_FILES test_alsa.c)
endif()

if(WITH_PULSEAUDIO)
  list(APPEND TEST_FILES test_pulse.c)
endif()

set(GENDRIVER_EXCLUDED_FILES test_main.c)

# skip protobuf-related tests
if(NOT WITH_PROTOBUF)
  list(APPEND GENDRIVER_EXCLUDED_FILES test_pb_c_wrap.c)
endif()

# skip ALSA-related tests
if(NOT WITH_ALSA)
  list(APPEND GENDRIVER_EXCLUDED_FILES test_alsa.c)
endif()

# skip ALSA-related tests
if(NOT WITH_PULSEAUDIO)
  list(APPEND GENDRIVER_EXCLUDED_FILES test_pulse.c)
endif()

set(GENDRIVER_ARGS --exclude_files ${GENDRIVER_EXCLUDED_FILES})
set(PYTEST_DIRS)
if (PYTEST)
  list(APPEND GENDRIVER_ARGS --pytest)
  list(APPEND PYTEST_DIRS --pytest_dir
      ${PROJECT_SOURCE_DIR}/test/python
      ${PROJECT_SOURCE_DIR}/src/core/ops/util)
endif()

add_executable(run_tests EXCLUDE_FROM_ALL test_main.c common.c util/util.c ${TEST_FILES})
add_custom_command(OUTPUT test_main.c 
  DEPENDS test_main.c.in common.c util/gendriver.py ${TEST_FILES}
  COMMAND python3 util/gendriver.py ${GENDRIVER_ARGS}
  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/test
  COMMENT "Generating test_main.c"
  POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy 
  ${CMAKE_SOURCE_DIR}/test/test_main.c
  ${PROJECT_BINARY_DIR}/test/test_main.c)
target_link_libraries(run_tests hiemal)
target_include_directories(run_tests PRIVATE include ${PROJECT_SOURCE_DIR}/src/core/include ${CMAKE_BINARY_DIR}/include)
if(WITH_PROTOBUF)
  target_include_directories(run_tests PRIVATE ${PROJECT_SOURCE_DIR}/src/protobuf/_c_wrap)
endif()
if(PYTEST)
  target_include_directories(run_tests PRIVATE ${Python_INCLUDE_DIRS})
  target_link_libraries(run_tests "python${Python_VERSION_MAJOR}.${Python_VERSION_MINOR}")
endif()
target_include_directories(run_tests PRIVATE ${CMAKE_BINARY_DIR}/src/core/ops)

add_custom_target(test COMMAND test/run_tests ${PYTEST_DIRS}
  WORKING_DIRECTORY ${PROJECT_BINARY_DIR} 
  DEPENDS run_tests
  COMMENT "Running tests")
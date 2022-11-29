if(WITH_PYTHON)
  add_library(hiemal_python MODULE)
  set_target_properties(hiemal_python PROPERTIES PREFIX "")
  set_target_properties(hiemal_python PROPERTIES OUTPUT_NAME "hiemal")
  target_include_directories(hiemal_python PRIVATE ${Python_INCLUDE_DIRS})
  target_link_libraries(hiemal_python ${Python_LIBRARIES})
  target_sources(hiemal_python PRIVATE hiemal.c)
  install(TARGETS hiemal_python
  LIBRARY
    DESTINATION "lib/python${Python_VERSION_MAJOR}.${Python_VERSION_MINOR}")
endif()
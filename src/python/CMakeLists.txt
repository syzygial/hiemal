if(WITH_PYTHON)
  add_library(hm MODULE)
  set_target_properties(hm PROPERTIES PREFIX "")
  target_include_directories(hm PRIVATE ${Python_INCLUDE_DIRS})
  target_link_libraries(hm ${Python_LIBRARIES})
  target_sources(hm PRIVATE hiemal.c)
  install(TARGETS hm
  LIBRARY
    DESTINATION "lib/python${Python_VERSION_MAJOR}.${Python_VERSION_MINOR}")
endif()
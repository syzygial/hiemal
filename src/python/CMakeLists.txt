# add_library(hiemal_python MODULE)
# set_target_properties(hiemal_python PROPERTIES PREFIX "")
# set_target_properties(hiemal_python PROPERTIES OUTPUT_NAME "hiemal")
# target_include_directories(hiemal_python PRIVATE ${Python_INCLUDE_DIRS})
# target_link_libraries(hiemal_python ${Python_LIBRARIES})
# target_sources(hiemal_python PRIVATE hiemal.c)
# install(TARGETS hiemal_python
# LIBRARY
#   DESTINATION "lib/python${Python_VERSION_MAJOR}.${Python_VERSION_MINOR}")
add_library(_hm_pywrap MODULE)
set_target_properties(_hm_pywrap PROPERTIES PREFIX "")
target_include_directories(_hm_pywrap PRIVATE ${Python_INCLUDE_DIRS} ${hiemal_INCLUDE_DIRS})
target_link_libraries(_hm_pywrap hiemal)
target_sources(_hm_pywrap PRIVATE hiemal/backend/src/_hm_pywrap.c)
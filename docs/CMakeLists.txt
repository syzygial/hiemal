set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/Modules ${CMAKE_MODULE_PATH})
find_package(Doxygen)
find_package(Sphinx)
#set(DOXY_FILES ../src/core/fmt.hpp
#  ../src/core/fmt.cpp)


set(DOC_OUTPUT ${PROJECT_BINARY_DIR}/docs)

set(DOXYGEN_XML_DIR ${DOC_OUTPUT}/xml)
set(DOXYGEN_XML_OUTPUT $${DOXYGEN_XML_DIR}/index.xml)
configure_file(${PROJECT_SOURCE_DIR}/docs/Doxyfile.in ${DOC_OUTPUT}/Doxyfile @ONLY)
add_custom_command(OUTPUT ${DOXYGEN_XML_OUTPUT} 
#  DEPENDS ${DOXY_FILES} 
  COMMAND ${DOXYGEN_EXECUTABLE}
  WORKING_DIRECTORY ${DOC_OUTPUT}
  COMMENT "Generating Doxygen XML")
add_custom_target(doxy_xml DEPENDS ${DOXYGEN_XML_OUTPUT})

set(SPHINX_COMMIT_INFO "OFF" CACHE BOOL "Add Git commit info to Sphinx footer")
if(SPHINX_COMMIT_INFO)
  set(SPHINX_CONF_COMMIT "True")
else()
  set(SPHINX_CONF_COMMIT "False")
endif()

set(SPHINX_BUILD_DIR ${DOC_OUTPUT}/_build) 
set(SPHINX_HTML_DIR ${SPHINX_BUILD_DIR}/html)
set(SPHINX_HTML_OUTPUT ${SPHINX_HTML_DIR}/index.html)
configure_file(${PROJECT_SOURCE_DIR}/docs/conf.py.in ${DOC_OUTPUT}/conf.py @ONLY)
file(MAKE_DIRECTORY ${DOC_OUTPUT}/_static ${DOC_OUTPUT}/_templates)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/_static/hiemal.png ${DOC_OUTPUT}/_static COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/_templates/footer.html ${DOC_OUTPUT}/_templates COPYONLY)
add_custom_command(OUTPUT ${SPHINX_HTML_OUTPUT} 
  DEPENDS ${DOXYGEN_XML_OUTPUT} 
  COMMAND ${SPHINX_EXECUTABLE} -c ${DOC_OUTPUT}
    ${CMAKE_CURRENT_SOURCE_DIR} ${SPHINX_BUILD_DIR} 
  WORKING_DIRECTORY ${DOC_OUTPUT} 
  COMMENT "Generating Sphinx Docs")
add_custom_target(docs DEPENDS ${SPHINX_HTML_OUTPUT})
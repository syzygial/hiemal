add_executable(hminfo hminfo.c)
target_sources(hminfo PUBLIC ${CMAKE_SOURCE_DIR}/src/core/include)
target_link_libraries(hminfo hiemal)
install(TARGETS hminfo
  RUNTIME
    DESTINATION "bin")
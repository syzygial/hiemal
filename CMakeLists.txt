cmake_minimum_required(VERSION 3.13)

# get git commit hash to put into the version string
execute_process(COMMAND "git" "rev-parse" "--short" "HEAD" 
  OUTPUT_VARIABLE HIEMAL_GIT_COMMIT 
  OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND "git" "rev-parse" "HEAD" 
  OUTPUT_VARIABLE HIEMAL_GIT_COMMIT_LONG
  OUTPUT_STRIP_TRAILING_WHITESPACE)
project(HIEMAL VERSION 0.1.0)

configure_file(cmake/version.h.in version.h @ONLY)

add_library(hiemal SHARED)
target_link_libraries(hiemal asound m)
add_subdirectory(src/core)
add_subdirectory(docs)
add_subdirectory(test)


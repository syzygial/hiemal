add_library(hmops STATIC)
target_include_directories(hmops PUBLIC ../include)
target_sources(ops.c)
add_custom_command(OUTPUT ops.c
  DEPENDS util/gen_ops.py ops.c.in
  COMMAND python3 util/gen_ops.py
  COMMENT "generating ops")
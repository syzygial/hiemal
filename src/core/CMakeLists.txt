
target_include_directories(hiemal PUBLIC include)
target_sources(hiemal PRIVATE 
  fmt.c 
  playback.c 
  error.c 
  buffer.c 
  dsp.c 
  math.c 
  async.c 
  signal_chain.c 
  io.c
  info.c
  logging.c)

set(HIEMAL_PUBLIC_INCLUDE
  "include/intern/dsp.h"
  "include/intern/buffer.h")

set_target_properties(hiemal PROPERTIES PUBLIC_HEADER "${HIEMAL_PUBLIC_INCLUDE}")
install(TARGETS hiemal
  PUBLIC_HEADER
    DESTINATION "include/hiemal"
  RESOURCE
    DESTINATION "lib/pkgconfig")
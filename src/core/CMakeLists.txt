if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  set(WITH_ALSA "ON" CACHE BOOL "Use ALSA")  
  find_package(ALSA)
  if(NOT ALSA_FOUND)
    set(WITH_ALSA "OFF")
  endif()
endif()

target_include_directories(hiemal PUBLIC include)
target_sources(hiemal PRIVATE 
  fmt.c 
  playback.c 
  error.c 
  buffer.c 
  dsp.c 
  intern/math.c 
  intern/async.c 
  signal_chain.c 
  io.c)

set (HIEMAL_PUBLIC_INCLUDE
  "include/dsp.h"
  "include/buffer.h")

set_target_properties(hiemal PROPERTIES PUBLIC_HEADER "${HIEMAL_PUBLIC_INCLUDE}")
install(TARGETS hiemal
  PUBLIC_HEADER
    DESTINATION "include/hiemal")
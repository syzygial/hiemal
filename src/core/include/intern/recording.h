#ifndef _INTERN_RECORDING_H
#define _INTERN_RECORDING_H

#include <stdbool.h>
#include "api/recording.h"

#include "hm_protobuf.h"

struct _recording {
  char *filename;
  buffer_t *buf;
  bool active;
  pb_buffer_t *buf_pb_msg;
};

#endif
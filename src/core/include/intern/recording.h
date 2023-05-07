#ifndef _INTERN_RECORDING_H
#define _INTERN_RECORDING_H

#include <stdbool.h>

#include "common.h"
#include "api/recording.h"

#include "hm_protobuf.h"

struct _recording {
  char *filename;
  bool active;
  pb_buflist_t *buflist;
  MEMBER_ARRAY(buffer)
};

#endif
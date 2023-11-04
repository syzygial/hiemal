#ifndef _INTERN_ASYNC_H
#define _INTERN_ASYNC_H

#include "api/async.h"
#include "intern/common.h"
#include "intern/thread.h"
#include <stdbool.h>

struct _async_handle {
  hm_event_list_t *l;
  bool loop_active;
  bool thread_created;
  hm_thread_id thread_id;
};

#endif

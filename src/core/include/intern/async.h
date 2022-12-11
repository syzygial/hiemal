#ifndef _TEST_ASYNC_H
#define _TEST_ASYNC_H

#include "api/async.h"
#include "core.h"
#include <stdbool.h>

struct _async_handle {
  bool *loop_active;
  bool *thread_created;
  unsigned int n_fn;
  void *inputs[ASYNC_LOOP_MAX_FN];
  void *outputs[ASYNC_LOOP_MAX_FN];
  kwargs_t kwargs[ASYNC_LOOP_MAX_FN];
  int (*fn_ptrs[ASYNC_LOOP_MAX_FN])(IMPL_ARGS);
};

#endif
#ifndef _INTERN_ASYNC_H
#define _INTERN_ASYNC_H

#include "core.h"

#define ASYNC_LOOP_MAX_FN 100

typedef struct _async_handle async_handle_t;
typedef struct _async_args {
  int (*fn)(IMPL_ARGS);
  void *inputs;
  void *outputs;
  kwargs_t kwargs;
} async_args_t;



int async_loop_init(async_handle_t **h);
int async_loop_delete(async_handle_t **h);

int async_loop_add_fn(async_handle_t *h, async_args_t args);

#endif

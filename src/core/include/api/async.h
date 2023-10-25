#ifndef _ASYNC_H
#define _ASYNC_H

#include "intern/common.h"
#include "intern/event.h"

typedef struct _async_handle async_handle_t;
typedef struct _async_args {
  void *inputs;
  void *outputs;
  kwargs_t kwargs;
} async_args_t;



int async_loop_init(async_handle_t **h);
int async_loop_delete(async_handle_t **h);
void async_loop_dispatch(async_handle_t *h);
void async_loop_stop(async_handle_t *h);
int async_loop_add_fn(async_handle_t *h, async_args_t args);

#endif

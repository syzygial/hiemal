#ifndef _INTERN_ASYNC_H
#define _INTERN_ASYNC_H

#include "intern/common.h"

#define ASYNC_LOOP_MAX_FN 100

typedef enum {INTERN_PIPE, EXTERN, ASYNC_MSG, BAD_FD} fd_type_t;

typedef struct _async_handle async_handle_t;
typedef struct _async_args {
  int fd;
  fd_type_t fd_type;
  int (*fn)(IMPL_ARGS);
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

#ifndef _INTERN_ASYNC_H
#define _INTERN_ASYNC_H

#define ASYNC_LOOP_MAX_FN 100

typedef struct _async_handle async_handle_t;

int async_loop_init(async_handle_t **h);
int async_loop_delete(async_handle_t **h);

#endif
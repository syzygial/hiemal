/// \file intern/async.c
#include <stdbool.h>
#include <pthread.h>
#include <stdlib.h>

#include "core.h"
#include "intern/async.h"

struct _async_handle {
  bool *loop_active;
  unsigned int n_fn;
  void *inputs[ASYNC_LOOP_MAX_FN];
  void *outputs[ASYNC_LOOP_MAX_FN];
  kwargs_t kwargs[ASYNC_LOOP_MAX_FN];
  int (*fn_ptrs[ASYNC_LOOP_MAX_FN])(IMPL_ARGS);
};

void *_async_loop(void *_h) {
  async_handle_t* h = (async_handle_t*)_h;
  int i;
  while (h->loop_active) {
    for (i = 0; i < h->n_fn; i++) {
      (*(h->fn_ptrs)[i])(1000, (h->inputs)[i], (h->outputs)[i], &((h->kwargs)[i]));
    }
  }
  pthread_exit(NULL);
}

void async_loop_dispatch(async_handle_t *h) {
  // TODO: run this in a separate thread
  pthread_t loop_thread;
  pthread_create(&loop_thread, NULL, _async_loop, (void*)h);
}

/*! \brief Initialize async event loop
* \param h address of ``async_handle_t*`` handle
* \return exit code
*/
int async_loop_init(async_handle_t **h) {
  *h = (async_handle_t*)malloc(sizeof(async_handle_t));
  (*h)->loop_active = (bool*)malloc(sizeof(bool));
  *((*h)->loop_active) = false;
  (*h)->n_fn = 0;
  return 0;
}

/*! \brief Delete async event loop
* \param h address of ``async_handle_t*`` handle
* \return exit code
*/
int async_loop_delete(async_handle_t **h) {
  free((*h)->loop_active);
  free(*h);
  *h = NULL;
  return 0;
}
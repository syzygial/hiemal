#include <stdbool.h>
#include <pthread.h>

#include "intern/async.h"

struct _async_handle {
  bool *loop_active;
  unsigned int n_fn;
  int (*fn_ptrs[ASYNC_LOOP_MAX_FN])();
};

void *_async_loop(void *_h) {
  async_handle_t* h = (async_handle_t*)_h;
  int i;
  while (h->loop_active) {
    for (i = 0; i < h->n_fn; i++) {
      (*(h->fn_ptrs)[i])();
    }
  }
  pthread_exit(NULL);
}

void async_loop_dispatch(async_handle_t *h) {
  // TODO: run this in a separate thread
  pthread_t loop_thread;
  pthread_create(&loop_thread, NULL, _async_loop, (void*)h);
}
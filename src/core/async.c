/// \file intern/async.c
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "intern/async.h"


void *_async_loop(void *_h) {
  async_handle_t* h = (async_handle_t*)_h;
  int i;
  while (h->loop_active) {
    for (i = 0; i < h->n_fn; i++) {
      (*(h->fn_ptrs)[i])(1000, (h->inputs)[i], (h->outputs)[i], &((h->kwargs)[i]));
    }
  }
  //pthread_exit(NULL);
}

void async_loop_dispatch(async_handle_t *h) {
  // TODO: run this in a separate thread
  h->thread_created = true;
  h->loop_active = true;
  pthread_t *loop_thread = &(h->thread_id);
  pthread_create(loop_thread, NULL, _async_loop, (void*)h);
}

void async_loop_stop(async_handle_t *h) {
  h->thread_created = false;
  h->loop_active = false;
  pthread_join(h->thread_id, NULL);
}
/*! \brief Initialize async event loop
* \param h address of ``async_handle_t*`` handle
* \return exit code
*/
int async_loop_init(async_handle_t **h) {
  *h = (async_handle_t*)malloc(sizeof(async_handle_t));
  (*h)->loop_active = (bool*)malloc(sizeof(bool));
  (*h)->thread_created = (bool*)malloc(sizeof(bool));
  (*h)->loop_active = false;
  (*h)->thread_created = false;
  (*h)->n_fn = 0;
  (*h)->thread_id = (pthread_t)NULL;
  return 0;
}

/*! \brief Delete async event loop
* \param h address of ``async_handle_t*`` handle
* \return exit code
*/
int async_loop_delete(async_handle_t **h) {
  free(*h);
  *h = NULL;
  return 0;
}

int async_loop_add_fn(async_handle_t *h, async_args_t args) {
  unsigned int fn_pos = h->n_fn;
  // TODO: add error code for trying to add a function when the loop is already full
  (h->inputs)[fn_pos] = args.inputs;
  (h->outputs)[fn_pos] = args.outputs;
  (h->kwargs)[fn_pos] = args.kwargs;
  (h->fn_ptrs)[fn_pos] = args.fn;
  h->n_fn += 1;
  return 0;
}
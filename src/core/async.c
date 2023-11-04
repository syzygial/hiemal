/// \file intern/async.c
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "intern/async.h"
#include "intern/thread.h"


void *_async_loop(void *h) {
  async_handle_t *_h = (async_handle_t *)h;
  _h->loop_active = true;
  while(1) {
    hm_event_poll_list(_h->l);
  }
  return NULL;
}

void async_loop_dispatch(async_handle_t *h)
{
  if(!(h->loop_active)) {
    h->thread_created = true;
    HM_THREAD_CREATE(_async_loop, (void*)h)
    while(!(h->loop_active));
  }
}

void async_loop_stop(async_handle_t *h)
{
  pthread_cancel(h->thread_id);
  pthread_join(h->thread_id, NULL);
  h->thread_created = false;
  h->loop_active = false;
}

/*! \brief Initialize async event loop
 * \param h address of ``async_handle_t*`` handle
 * \return exit code
 */
int async_loop_init(async_handle_t **h)
{
  *h = (async_handle_t *)malloc(sizeof(async_handle_t));
  (*h)->loop_active = (bool *)malloc(sizeof(bool));
  (*h)->thread_created = (bool *)malloc(sizeof(bool));
  (*h)->loop_active = false;
  (*h)->thread_created = false;
  (*h)->thread_id = (pthread_t)NULL;
  return 0;
}

/*! \brief Delete async event loop
 * \param h address of ``async_handle_t*`` handle
 * \return exit code
 */
int async_loop_delete(async_handle_t **h)
{
  free(*h);
  *h = NULL;
  return 0;
}
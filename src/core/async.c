/// \file intern/async.c
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "intern/async.h"


void *_async_loop(void *_h) {
  async_handle_t *h = (async_handle_t *)_h;
  int i;
  int i_fn;
  int n_samples;
  h->loop_active = true;
  while (h->loop_active) {
    poll(h->async_fds,h->n_fds, -1);
    n_samples = 0;
    for (i = 0; i < h->n_fds; i++) {
      if ((h->async_fds)[i].revents & POLLIN == POLLIN) {
        if (async_fd_map_get_fd_type(h->map, (h->async_fds)[i].fd) == INTERN_PIPE) {
          read((h->async_fds)[i].fd, &n_samples, 4);
        }
        else if (async_fd_map_get_fd_type(h->map, (h->async_fds)[i].fd) == EXTERN) {
          ioctl((h->async_fds)[i].fd, FIONREAD, &n_samples);
        }
        else {
          break;
        }
        i_fn = async_fd_map_get_index(h->map, (h->async_fds)[i].fd);
        (*(h->fn_ptrs)[i_fn])(n_samples, (h->inputs)[i_fn], (h->outputs)[i_fn], &((h->kwargs)[i_fn]));
      }
    }
  }
}

void async_loop_dispatch(async_handle_t *h)
{
  if(!(h->loop_active)) {
    h->thread_created = true;
    pthread_t *loop_thread = &(h->thread_id);
    pthread_create(loop_thread, NULL, _async_loop, (void *)h);
    while(!(h->loop_active));
  }
}

void async_loop_stop(async_handle_t *h)
{
  char msg = '\0';
  h->thread_created = false;
  h->loop_active = false;
  // break the loop out of poll()-ing by sending a null byte
  write(h->msg_fd, &msg, 1);
  pthread_join(h->thread_id, NULL);
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
  (*h)->n_fn = 0;
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

int async_loop_add_fn(async_handle_t *h, async_args_t args)
{
  unsigned int fn_pos = h->n_fn;
  unsigned int fd_pos = h->n_fds;

  if(fd_pos == h->n_fds_alloc) {
    h->async_fds = (struct pollfd*)realloc(h->async_fds, 2 * h->n_fds_alloc);
    h->n_fds_alloc *= 2;
  }

  async_fd_map_t *map = h->map;
  async_fd_map_insert(map, args.fd, args.fd_type, fn_pos);
  (h->async_fds)[fd_pos].fd = args.fd;
  (h->async_fds)[fd_pos].events = POLLIN;
  h->n_fds += 1;
  // TODO: add error code for trying to add a function when the loop is already full
  (h->inputs)[fn_pos] = args.inputs;
  (h->outputs)[fn_pos] = args.outputs;
  (h->kwargs)[fn_pos] = args.kwargs;
  (h->fn_ptrs)[fn_pos] = args.fn;
  h->n_fn += 1;
  return 0;
}
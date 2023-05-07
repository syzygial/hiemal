/// \file intern/async.c
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "intern/async.h"

typedef struct _async_fd_set_node async_fd_set_node;
typedef struct _async_fd_set async_fd_set;
typedef struct _async_index_set_node async_index_set_node;
typedef struct _async_index_set async_index_set;

struct _async_fd_set_node {
  async_fd_set_node *next;
  int fd;
  fd_type_t fd_type;
};

struct _async_fd_set {
  async_fd_set_node *head;
  int n_fds;
};

struct _async_index_set_node {
  async_index_set_node *next;
  int index;
};

struct _async_index_set {
  async_index_set_node *head;
  int n_index;
};

struct _async_fd_map {
  async_fd_set *fds;
  async_index_set *indexes;
  int n_mappings;
};

int async_fd_map_init(async_fd_map_t **map)
{
  *map = (async_fd_map_t*)malloc(sizeof(async_fd_map_t));
  (*map)->fds = (async_fd_set*)malloc(sizeof(async_fd_set));
  (*map)->indexes = (async_index_set*)malloc(sizeof(async_index_set));
  async_fd_set_node *fd_head = 
    (async_fd_set_node*)malloc(sizeof(async_fd_set_node));
  async_index_set_node *index_head = 
    (async_index_set_node*)malloc(sizeof(async_index_set_node));
  fd_head->next = NULL;
  fd_head->fd = -1;
  index_head->next = NULL;
  index_head->index = -1;
  (*map)->fds->head = fd_head;
  (*map)->indexes->head = index_head;
  (*map)->n_mappings = 0;
  return 0;
}

int async_fd_map_delete(async_fd_map_t **map) {
  async_fd_set_node *fd_itr = (*map)->fds->head;
  async_fd_set_node *fd_next = fd_itr->next;
  async_index_set_node *index_itr = (*map)->indexes->head;
  async_index_set_node *index_next = index_itr->next;

  // free keys
  while(fd_itr != NULL) {
    fd_next = fd_itr->next;
    free(fd_itr);
    fd_itr = fd_next;
  }
  // free values
  while(index_itr != NULL) {
    index_next = index_itr->next;
    free(index_itr);
    index_itr = index_next;
  }  
  free(*map);
  *map = NULL;
  return 0;
}

int async_fd_map_insert(async_fd_map_t *map, int fd, fd_type_t fd_type, int index)
{
  // search keys
  async_fd_set_node *fd_itr = map->fds->head;
  async_fd_set_node *fd_prev = fd_itr;
  async_index_set_node *index_itr = map->indexes->head;
  async_index_set_node *index_prev = index_itr;

  while (fd_itr != NULL) {
    if (fd == fd_itr->fd)
    {
      return 0;
    }
    fd_prev = fd_itr;
    index_prev = index_itr;
    fd_itr = fd_itr->next;
    index_itr = index_itr->next;
  }
  async_fd_set_node *new_fd_node =
    (async_fd_set_node*)malloc(sizeof(async_fd_set_node));
  async_index_set_node *new_index_node =
    (async_index_set_node*)malloc(sizeof(async_index_set_node));
  new_fd_node->next = NULL;
  new_fd_node->fd = fd;
  new_fd_node->fd_type = fd_type;
  new_index_node->next = NULL;
  new_index_node->index = index;

  fd_prev->next = new_fd_node;
  index_prev->next = new_index_node;
  map->n_mappings += 1;
  return 0;
}

int async_fd_map_get_index(async_fd_map_t *map, int fd) {
  async_fd_set_node *fd_itr = map->fds->head;
  async_index_set_node *index_itr = map->indexes->head;
  while (fd_itr != NULL) {
    if (fd == fd_itr->fd)
    {
      return index_itr->index;
    }
    fd_itr = fd_itr->next;
    index_itr = index_itr->next;
  } 
  return -1; // no matching key
}

fd_type_t async_fd_map_get_fd_type(async_fd_map_t *map, int fd) {
  async_fd_set_node *fd_itr = map->fds->head;
  while (fd_itr != NULL) {
    if (fd == fd_itr->fd)
    {
      return fd_itr->fd_type;
    }
    fd_itr = fd_itr->next;
  } 
  return BAD_FD; // no matching key
}

int async_fd_map_n_keys(async_fd_map_t *map) {
  return map->n_mappings;
}

void *_async_loop(void *_h) {
  async_handle_t *h = (async_handle_t *)_h;
  int i;
  int i_fn;
  int n_samples;
  while (h->loop_active) {
    poll(h->async_fds,h->n_fds, -1);
    n_samples = 0;
    for (i = 0; i < h->n_fds; i++) {
      if ((h->async_fds)[i].revents & POLLIN == POLLIN) {
        if (async_fd_map_get_fd_type(h->map, (h->async_fds)[i].fd) == INTERN_PIPE) {
          read((h->async_fds)[i].fd, &n_samples, 4);
        }
        else {
          ioctl((h->async_fds)[i].fd, FIONREAD, &n_samples);
        }
        i_fn = async_fd_map_get_index(h->map, (h->async_fds)[i].fd);
        (*(h->fn_ptrs)[i_fn])(n_samples, (h->inputs)[i_fn], (h->outputs)[i_fn], &((h->kwargs)[i_fn]));
      }
    }
    usleep(10000);
  }
  // pthread_exit(NULL);
}

void async_loop_dispatch(async_handle_t *h)
{
  // TODO: run this in a separate thread
  h->thread_created = true;
  h->loop_active = true;
  pthread_t *loop_thread = &(h->thread_id);
  pthread_create(loop_thread, NULL, _async_loop, (void *)h);
}

void async_loop_stop(async_handle_t *h)
{
  h->thread_created = false;
  h->loop_active = false;
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
  (*h)->async_fds = (struct pollfd*)malloc(100*sizeof(struct pollfd));
  (*h)->n_fds = 0;
  (*h)->n_fds_alloc = 100;
  async_fd_map_init(&((*h)->map));
  return 0;
}

/*! \brief Delete async event loop
 * \param h address of ``async_handle_t*`` handle
 * \return exit code
 */
int async_loop_delete(async_handle_t **h)
{
  async_fd_map_delete(&((*h)->map));
  free((*h)->async_fds);
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
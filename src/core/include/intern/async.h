#ifndef _TEST_ASYNC_H
#define _TEST_ASYNC_H

#include "api/async.h"
#include "common.h"

#include <stdbool.h>
#include <pthread.h>
#include <poll.h>

typedef struct _async_fd_map async_fd_map_t;

struct _async_handle {
  bool loop_active;
  bool thread_created;
  pthread_t thread_id;
  unsigned int n_fn;
  unsigned int n_fds;
  unsigned int n_fds_alloc;
  struct pollfd* async_fds;
  async_fd_map_t *map;
  int msg_fd;
  void *inputs[ASYNC_LOOP_MAX_FN];
  void *outputs[ASYNC_LOOP_MAX_FN];
  kwargs_t kwargs[ASYNC_LOOP_MAX_FN];
  int (*fn_ptrs[ASYNC_LOOP_MAX_FN])(IMPL_ARGS);
};

int async_fd_map_init(async_fd_map_t **map);
int async_fd_map_delete(async_fd_map_t **map);
int async_fd_map_insert(async_fd_map_t *map, int fd, fd_type_t fd_type, int index);
int async_fd_map_n_keys(async_fd_map_t *map);
int async_fd_map_get_index(async_fd_map_t *map, int fd);
fd_type_t async_fd_map_get_fd_type(async_fd_map_t *map, int fd);

#endif

#ifndef _INTERN_ASYNC_H
#define _INTERN_ASYNC_H

#include "api/async.h"
#include "intern/common.h"
#include "intern/thread.h"
#include <stdbool.h>

struct _async_handle {
  hm_event_list_t *l;
  bool loop_active;
  bool thread_created;
  hm_thread_id thread_id;
};

int async_fd_map_init(async_fd_map_t **map);
int async_fd_map_delete(async_fd_map_t **map);
int async_fd_map_insert(async_fd_map_t *map, int fd, fd_type_t fd_type, int index);
int async_fd_map_n_keys(async_fd_map_t *map);
int async_fd_map_get_index(async_fd_map_t *map, int fd);
fd_type_t async_fd_map_get_fd_type(async_fd_map_t *map, int fd);

#endif

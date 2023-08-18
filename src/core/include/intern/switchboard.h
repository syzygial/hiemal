#ifndef _INTERN_SWITCHBOARD_H
#define _INTERN_SWITCHBOARD_H

#include "api/switchboard.h"
#include "intern/common.h"
#include "intern/async.h"
#include "intern/buffer.h"

#ifdef __unix__
#include <pthread.h>
#endif

typedef union {
  buffer_t *buf;
  async_handle_t *async_handle;
} node_resource;

typedef union {
  int pipe_fd[2];
  int bd_pipe_fd[2][2]; // bidirectional pipe
} connection_resource;

// TODO: replace these typedef's with the thread/mutex
// typedefs in common.h
#ifdef __unix__
typedef pthread_t context_thread_id;
typedef pthread_mutex_t node_mutex;
#endif

struct _switchboard_node {
  HM_LIST_NODE_HEAD(switchboard_node_t)
  char *name;
  node_type_t type;
  node_resource res;
  unsigned int node_id;
  switchboard_connection_list_t *connections;
  switchboard_context_list_t *context_list;
  switchboard_context_t *active_context;
  node_mutex mutex;
};

struct _switchboard_node_list {
  HM_LIST_HEAD(switchboard_node_t)
};

struct _switchboard_connection {
  HM_LIST_NODE_HEAD(switchboard_connection_t)
  char *name;
  unsigned int connection_id;
  connection_type_t connection_type;
  switchboard_node_t *nodes[2];
  switchboard_connection_dir dir;
  connection_resource res;
};

struct _switchboard_connection_list {
  HM_LIST_HEAD(switchboard_connection_t)
};

struct _switchboard_context {
  HM_LIST_NODE_HEAD(switchboard_context_t)
  unsigned int context_id;
  switchboard_node_t *node;
  switchboard_connection_list_t *incoming_connections;
  switchboard_connection_list_t *outgoing_connections;
  context_thread_id thread_id;
};

struct _switchboard_context_list {
  HM_LIST_HEAD(switchboard_context_t)
};

struct _switchboard {
  switchboard_node_list_t *nodes;
  switchboard_connection_list_t *connections;
  switchboard_context_list_t *context_list;
};

int switchboard_add_node(switchboard_t *s, node_type_t node_type, char *node_name, node_resource node_res);

switchboard_node_t* switchboard_get_node_by_id(switchboard_t *s, unsigned int node_id);
switchboard_node_t* switchboard_get_node_by_name(switchboard_t *s, const char *node_name);

switchboard_context_t* switchboard_add_context(switchboard_t *s, switchboard_node_t *node);
switchboard_context_t* get_context(switchboard_t *s, switchboard_node_t *node);

int _context_thread_cmp(hm_list_node_t *_ctx, void *_thread_id);

context_thread_id get_thread_id();

#endif

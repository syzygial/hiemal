#ifndef _INTERN_SWITCHBOARD_H
#define _INTERN_SWITCHBOARD_H

#include "api/switchboard.h"
#include "intern/common.h"
#include "intern/async.h"
#include "intern/buffer.h"

#ifdef __unix__
#include <pthread.h>
#endif

// switchboard_send/switchboard_recv flags

#define SB_NONBLOCKING 0x1
#define SB_POLL 0x2


typedef enum {
  ASYNC_LOOP=0,
  BUFFER,
  SOURCE,
  SINK,
  RPC
} node_type_t;

typedef enum {
  UNIX_PIPE=0,
  UNIX_PIPE_BD,
  UNIX_SOCKET,
  TCP_SOCKET,
  DBUS
} connection_type_t;

typedef union {
  buffer_t *buf;
  async_handle_t *async_handle;
} node_resource;

typedef union {
  int pipe_fd[2];
  int bd_pipe_fd[2][2]; // bidirectional pipe
} connection_resource;


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

switchboard_node_t* switchboard_get_node_by_id(switchboard_t *s, unsigned int node_id);

// OS-specific functionality
int switchboard_add_connection(switchboard_t *s, switchboard_node_t *src, switchboard_node_t *dest, char *name, switchboard_connection_dir dir);

int activate_context(switchboard_context_t *ctx, int flags);
int deactivate_context(switchboard_context_t *ctx);

int connection_poll(switchboard_node_t *recv_node, switchboard_connection_t *connection);
int connection_poll_stop(switchboard_node_t *recv_node, switchboard_connection_t *connection);

#endif

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "intern/switchboard.h"

int switchboard_add_connection(switchboard_t *s, switchboard_node_t *src, switchboard_node_t *dest, char *name, switchboard_connection_dir dir) {
  connection_type_t conn_type;
  connection_resource res;
  switch(dir) {
    case BIDIRECTIONAL:
      int bd_pipe_fd[2][2];
      pipe(bd_pipe_fd[0]);
      pipe(bd_pipe_fd[1]);
      memcpy(res.bd_pipe_fd, bd_pipe_fd, sizeof(int[2][2]));
      conn_type = UNIX_PIPE_BD;
    case FORWARD:
    case BACKWARD:
      int pipe_fd[2];
      pipe(pipe_fd);
      memcpy(res.bd_pipe_fd, bd_pipe_fd, sizeof(int[2]));
      conn_type = UNIX_PIPE;
    default:
      return -1;
  }
  switchboard_connection_t *new_connection = (switchboard_connection_t*)malloc(sizeof(switchboard_connection_t));
  HM_LIST_NODE_INIT(new_connection)
  new_connection->connection_type = conn_type;
  new_connection->nodes[0] = src;
  new_connection->nodes[1] = dest;
  new_connection->dir = dir;
  new_connection->res = res;
  hm_list_append(src->connections, new_connection);
  hm_list_append(dest->connections, new_connection);
  hm_list_append(s->connections, new_connection);
  return 0;
}

int switchboard_add_context(switchboard_t *s, unsigned int node_id) {
  switchboard_context_t *new_context = (switchboard_context_t*)malloc(sizeof(switchboard_context_t));
  HM_LIST_NODE_INIT(new_context)
  switchboard_node_t *node = switchboard_get_node_by_id(s, node_id);
  new_context->node = node;
  // TODO: setup context connections
  new_context->incoming_connections = NULL;
  new_context->outgoing_connections = NULL;
  new_context->thread_id = pthread_self();
  hm_list_append(node->context_list, new_context);
  hm_list_append(s->context_list, new_context);
  return 0;
}

int activate_context(switchboard_t *s, switchboard_context_t *ctx) {
  switchboard_node_t *node = ctx->node;
  if (node->active_context == ctx) {
    return 0;
  }
  else {
    pthread_mutex_lock(&(node->mutex));
  }
  node->active_context = ctx;
  return 0;
}

int deactivate_context(switchboard_context_t *ctx) {
  return 0;
}
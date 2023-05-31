#include <stdlib.h>
#include <string.h>

#include "intern/switchboard.h"


int node_name_is_free(switchboard_node_list_t *list, char *name) {
  switchboard_node_t *node_itr = list->head;
  while (node_itr != NULL) {
    if (strcmp(node_itr->name, name) == 0) {
      return 1;
    }
  }
  return 0;
}

int switchboard_init(switchboard_t **s) {
  *s = (switchboard_t*)malloc(sizeof(switchboard_t));
  (*s)->nodes = (switchboard_node_list_t*)malloc(sizeof(switchboard_node_t));
  (*s)->connections = (switchboard_connection_list_t*)malloc(sizeof(switchboard_connection_list_t));
  (*s)->context_list = (switchboard_context_list_t*)malloc(sizeof(switchboard_context_list_t));
  HM_LIST_INIT((*s)->nodes)
  HM_LIST_INIT((*s)->connections)
  HM_LIST_INIT((*s)->context_list)
  return 0;
}

int switchboard_delete(switchboard_t **s) {
  hm_list_delete((*s)->nodes, NULL);
  hm_list_delete((*s)->connections, NULL);
  hm_list_delete((*s)->context_list, NULL);
  free(*s);
  *s = NULL;
  return 0;
}

int switchboard_add_node(switchboard_t *s, node_type_t node_type, char *node_name, node_resource node_res) {
  switchboard_node_t *new_node = malloc(sizeof(switchboard_node_t));
  new_node->type = node_type;
  new_node->res = node_res;
  new_node->name = strdup(node_name);
  hm_list_append(s->nodes, new_node);
  return 0;
}

int switchboard_add_buffer(switchboard_t *s, buffer_t *buf, char *name) {
  node_resource node_res;
  node_res.buf = buf;
  return switchboard_add_node(s, BUFFER, name, node_res);
}

int switchboard_add_async_loop(switchboard_t *s, async_handle_t *async_handle, char *name) {
  node_resource node_res;
  node_res.async_handle = async_handle;
  return switchboard_add_node(s, ASYNC_LOOP, name, node_res);
}

switchboard_node_t* switchboard_get_node_by_id(switchboard_t *s, unsigned int node_id) {
  switchboard_node_t *node_itr = s->nodes->head;
  while (node_itr != NULL) {
    if (node_itr->node_id == node_id) {
      return node_itr;
    }
    node_itr = node_itr->next;
  }
  return NULL;
}


switchboard_node_t* common_async_loop(switchboard_node_t *node1, switchboard_node_t *node2) {
  switchboard_connection_t *node1_itr = node1->connections->head;
  switchboard_connection_t *node2_itr = node1->connections->head;
  switchboard_node_t *node1_connection;
  switchboard_node_t *node2_connection;
  while (node1_itr != NULL) {
    while (node2_itr != NULL) {
      node1_connection = (node1_itr->nodes[0] == node1) ? node1_itr->nodes[1] : node1_itr->nodes[0];
      node2_connection = (node2_itr->nodes[0] == node2) ? node2_itr->nodes[1] : node2_itr->nodes[0];
      if (node1_connection == node2_connection && node1_connection->type == ASYNC_LOOP) {
        return node1_connection;
      }
      node2_itr = node2_itr->next;
    }
    node1_itr = node1_itr->next;
  }
  return NULL;
}

bool ctx_same_thread(switchboard_context_t *ctx1, switchboard_context_t *ctx2) {
  return (ctx1->thread_id == ctx2->thread_id);
}

int switchboard_send_buf_buf(switchboard_t *s, buffer_t *src, buffer_t *dest, unsigned int n_bytes) {
  unsigned int n_bytes_transferred = 0;
  unsigned int src_bytes = buffer_n_read_bytes(src);
  unsigned int dest_bytes = buffer_n_write_bytes(src);
  unsigned int bytes_to_transfer = (src_bytes < dest_bytes) ? src_bytes : dest_bytes;
  bytes_to_transfer = (bytes_to_transfer < n_bytes) ? bytes_to_transfer : n_bytes;
  void *tmp_buf = malloc(n_bytes);
  bytes_to_transfer = buffer_read(src, tmp_buf, bytes_to_transfer);
  n_bytes_transferred = buffer_write(src, tmp_buf, bytes_to_transfer);
  free(tmp_buf);
  return n_bytes_transferred;
}

int switchboard_send(switchboard_t *s, switchboard_context_t *src, unsigned int node_id) {
  switchboard_node_t *dest_node = switchboard_get_node_by_id(s, node_id);
  switchboard_context_t *dest_ctx = dest_node->active_context;
  return 0;
}
/*
int switchboard_recv(switchboard_t *s, unsigned int node_id) {
  return 0;
}
*/

int switchboard_dump(switchboard_t *s) {
  return 0;
}

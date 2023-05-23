#include "intern/common.h"
#include "intern/switchboard.h"

typedef enum {
  ASYNC_LOOP=0,
  BUFFER,
  SOURCE,
  SINK,
  EXTERNAL
} node_type_t;

typedef enum {
  UNIX_PIPE,
  UNIX_SOCKET,
  TCP_SOCKET,
} connection_type_t;

typedef union {
  int fd;
} node_resource;

typedef struct _switchboard switchboard_t;
typedef struct _switchboard_node switchboard_node_t;
typedef struct _switchboard_connection switchboard_connection_t;

struct _switchboard_node {
  HM_LIST_NODE_HEAD(switchboard_node_t)
  node_type_t type;
  node_resource res;
  unsigned int node_id;
};

struct _switchboard_connection {
  HM_LIST_NODE_HEAD(switchboard_connection_t)
  connection_type_t connection_type;
  switchboard_node_t *nodes[2];
  enum {FORWARD, BACKWARD, BIDIRECTIONAL} dir;
};

struct _switchboard_connection_list {
  HM_LIST_HEAD(switchboard_connection_t)
};

struct _switchboard {
  
};

int switchboard_init(switchboard_t **s) {
  return 0;
}

int switchboard_delete(switchboard_t **s) {
  return 0;
}

int switchboard_send(switchboard_t *s, unsigned int node_id) {
  return 0;
}

int switchboard_recv_wait(switchboard_t *s, unsigned int node_id) {
  return 0;
}
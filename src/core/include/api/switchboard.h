#ifndef _SWITCHBOARD_H
#define _SWITCHBOARD_H

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

typedef enum {FORWARD, BACKWARD, BIDIRECTIONAL} switchboard_connection_dir;

typedef struct _switchboard switchboard_t;
typedef struct _switchboard_node switchboard_node_t;
typedef struct _switchboard_node_list switchboard_node_list_t;
typedef struct _switchboard_connection switchboard_connection_t;
typedef struct _switchboard_connection_list switchboard_connection_list_t;
typedef struct _switchboard_context switchboard_context_t;
typedef struct _switchboard_context_list switchboard_context_list_t;

int switchboard_init(switchboard_t **s);
int switchboard_delete(switchboard_t **s);

#ifdef _BUFFER_H
int switchboard_add_buffer(switchboard_t *s, buffer_t *buf, char *name);
#endif

#ifdef _ASYNC_H
int switchboard_add_async_loop(switchboard_t *s, async_handle_t *async_handle, char *name);
#endif

int switchboard_send(switchboard_t *s, const char *src, const char *dest, unsigned int n_bytes, const int flags);

int switchboard_dump(switchboard_t *s);

// OS-specific functionality
int switchboard_add_connection(switchboard_t *s, switchboard_node_t *src, switchboard_node_t *dest, char *name, switchboard_connection_dir dir);
int switchboard_add_connection_by_name(switchboard_t *s, const char *src, const char *dest, char *name, switchboard_connection_dir dir);

switchboard_context_t* switchboard_add_context_by_name(switchboard_t *s, const char *node_name);

int activate_context(switchboard_context_t *ctx, int flags);
int deactivate_context(switchboard_context_t *ctx);

int connection_poll(switchboard_node_t *recv_node, switchboard_connection_t *connection);
int connection_poll_stop(switchboard_node_t *recv_node, switchboard_connection_t *connection);

int switchboard_node_acquire(switchboard_t *s, const char *node_name);

#endif

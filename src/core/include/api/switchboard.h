#ifndef _SWITCHBOARD_H
#define _SWITCHBOARD_H

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

#endif

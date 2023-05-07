#ifndef _SIGNAL_CHAIN_H
#define _SIGNAL_CHAIN_H

#include "intern/buffer.h"
#include "intern/common.h"
#include "intern/async.h"

#define SIGCHAIN_BUF_MAX 100

typedef struct _signal_chain {
  unsigned int n_src;
  unsigned int n_sink;
  unsigned int n_fn;
  unsigned int n_buf;
  
  buffer_t *buf_list[SIGCHAIN_BUF_MAX];

  async_handle_t *h;
} signal_chain_t;

typedef struct _sc_args {
  enum {SRC=0,SINK,OP} node_type;
  int (*fn)(IMPL_ARGS);
  void *inputs;
  void *outputs;
  kwargs_t kwargs;
} sc_args_t;

// typedef struct _signal_chain signal_chain_t;
// typedef struct _sc_node sc_node_t;
#endif
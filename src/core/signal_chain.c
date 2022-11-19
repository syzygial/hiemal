#include "signal_chain.h"
#include "buffer.h"

struct _sc_node {
  enum {SRC=0, SINK, OP} node_type;
  unsigned int n_input;
  unsigned int n_output;
  buffer_t **inputs;
  buffer_t **outputs;
  unsigned int argc;
  void **argv;
  int (*node_fn)(int, void**);
};

struct _signal_chain {
  unsigned int n_src;
  sc_node_t *src_list;
};



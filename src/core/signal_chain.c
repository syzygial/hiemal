#include <stdlib.h>

#include "intern/signal_chain.h"


int hm_sc_init(signal_chain_t **sc) {
  *sc = (signal_chain_t*)malloc(sizeof(signal_chain_t));
  (*sc)->n_src = 0;
  (*sc)->n_sink = 0;
  (*sc)->n_fn = 0;
  (*sc)->n_buf = 0;
  async_loop_init(&((*sc)->h));
  return 0;
}

int hm_sc_delete(signal_chain_t **sc) {
  unsigned int i = 0;
  for (i = 0; i < (*sc)->n_buf; i++) {
    buffer_delete(&(((*sc)->buf_list)[i]));
  }
  async_loop_delete(&((*sc)->h));
  free(*sc);
  *sc = NULL;
  return 0;
}


int hm_sc_add_node(signal_chain_t *sc, sc_args_t sc_args) {
  unsigned buf_pos = sc->n_buf;
  async_handle_t *h = sc->h;
  async_args_t async_args;
  //async_args.fn = sc_args.fn;
  async_args.kwargs = sc_args.kwargs;
  if (sc_args.node_type != SINK) {
    buffer_init(&((sc->buf_list)[buf_pos]), 0x1000, RING);
    (sc->n_buf)++;
  }
  if (sc_args.node_type == SRC) {
    async_args.inputs = sc_args.inputs;
    async_args.outputs = (void*)((sc->buf_list)[buf_pos]);
    (sc->n_src)++;
  }
  else if (sc_args.node_type == SINK) {
    async_args.inputs = (void*)((sc->buf_list)[buf_pos-1]);
    async_args.outputs = sc_args.outputs;
    (sc->n_sink)++;
  }
  else { // sc_args.node_type == OP
    async_args.inputs = (void*)((sc->buf_list)[buf_pos-1]);
    async_args.outputs = (void*)((sc->buf_list)[buf_pos]);
  }
  //async_args.fn = sc_args.fn;
  async_loop_add_fn(sc->h, async_args);
  (sc->n_fn)++;
  return 0;
}


// TODO: implement these
int hm_sc_run(signal_chain_t *sc) {
  return 0;
}

int hm_sc_stop(signal_chain_t *sc) {
  return 0;
}
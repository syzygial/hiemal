// testing header that exposes more dsp internals

#ifndef _TEST_RBUF_H
#define _TEST_RBUF_H

#include <stdbool.h>
#include "buffer.h"

unsigned int _rbuf_n_write_bytes(buffer_t *buf);
unsigned int _rbuf_n_read_bytes(buffer_t *buf);

struct _buffer {
  void *buf;
  unsigned int buf_len_bytes;
  void *read_ptr;
  void *write_ptr;
  buffer_type_t type;
  buffer_state_t state;
  bool in_use;
};

#endif

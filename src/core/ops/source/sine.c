#include "ops_internal.h"

SOURCE_OP_INIT_FN(sine) {
  __SOURCE_SINE_ARGS_UNPACK(kwargs)
  __SOURCE_SINE_STATE_UNPACK(state)
  return 0;
}

SOURCE_OP_FINI_FN(sine) {
  __SOURCE_SINE_STATE_UNPACK(state)
  return 0;
}

SOURCE_OP(sine) {
  __SOURCE_SINE_ARGS_UNPACK(kwargs)
  __SOURCE_SINE_STATE_UNPACK(state)
  return 0;  
}

SOURCE_BYTES_READABLE_FN(sine) {
  __SOURCE_SINE_STATE_UNPACK(src_op->state)
  return 0;
}
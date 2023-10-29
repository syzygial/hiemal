#include "intern/buffer.h"
#include "intern/common.h"

typedef struct _encoder_stack encoder_stack;
typedef struct _decoder_stack decoder_stack;

typedef int (stream_fn)(void *src, void *dest, unsigned int n_bytes);

struct _encoder {
  HM_LIST_NODE_HEAD(struct _encoder)
  stream_fn *encoder_fn;
};

struct _encoder_stack {
  HM_LIST_HEAD(struct _encoder)
};

struct _decoder {
  HM_LIST_NODE_HEAD(struct _decoder)
  stream_fn *decoder_fn;
};

struct _decoder_stack {
  HM_LIST_HEAD(struct _decoder)
};

union stream_obj {
  int fd;
  buffer_t *buf;
};

enum stream_obj_type {
  STREAM_FD=0,
  STREAM_BUF
};

struct _stream {
  encoder_stack *enc;
  decoder_stack *dec;
  stream_obj src_obj;
  stream_obj_type src_type;
  stream_obj dest_obj;
  stream_obj_type dest_type;
};
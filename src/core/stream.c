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

struct _stream {
  encoder_stack *enc;
  decoder_stack *dec;
};
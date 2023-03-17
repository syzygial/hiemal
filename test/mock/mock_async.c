#include "mock/mock_async.h"
#include "intern/core.h"

#include <errno.h>

// operation that mutliplies input by 2
IMPL(times_two) {

  buffer_t *src = (buffer_t*)_inputs;
  buffer_t *dest = (buffer_t*)_outputs;

  unsigned int src_bytes = buffer_n_read_bytes(src);
  if (src->state == EMPTY  || dest->state == FULL) {
    return -EAGAIN;
  }
  if (src_bytes < n_bytes) {
    n_bytes = src_bytes;
  }
  unsigned int n_samples = n_bytes / sizeof(double_t);
  double_t input_sample, output_sample;
  int i;
  for (i = 0; i < n_samples; i++) {
    buffer_read(src, &input_sample, sizeof(double_t));
    output_sample = 2 * input_sample;
    buffer_write(dest, &output_sample, sizeof(double_t));
  }
  return src_bytes;
}

int times_two(double_t *src, double_t *dest, unsigned int n_samples) {
  // TODO: implement
  return 0;
}

async_args_t times_two_async(buffer_t *src, buffer_t *dest) {
  async_args_t async_args;
  kwargs_t kwargs;
  async_args.fn = times_two_impl;
  async_args.inputs = (void*)src;
  async_args.outputs = (void*)dest;
  async_args.kwargs = kwargs;
  return async_args;
}
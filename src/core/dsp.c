#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include "intern/dsp.h"
#include "intern/core.h"
#include "intern/math.h"


int _buf_shift_down_64(void *buf, const unsigned int n_bytes, 
    const unsigned int n_shift) {
  const unsigned int n_items = n_bytes / 8;
  if (n_shift >= n_items) {
    memset(buf, 0, n_bytes);
    return 0;
  }
  int i;
  for (i = 0; i < (n_items - n_shift); i++) {
    ((uint64_t*)buf)[i] = ((uint64_t*)buf)[i + n_shift];
  }
  // zero out the rest of the buffer
  memset(buf + 8*(n_items - n_shift), 0, n_shift*8);
  return 0;
}

int _buf_shift_up_64(void *buf, const unsigned int n_bytes, 
    const unsigned int n_shift) {
  const unsigned int n_items = n_bytes / 8;
  if (n_shift >= n_items) {
    memset(buf, 0, sizeof(double_t)*n_items);
    return 0;
  }
  int i;
  for (i = (n_items - n_shift - 1); i >= 0; i--) {
    ((uint64_t*)buf)[i+n_shift] = ((uint64_t*)buf)[i];
  }
  // zero out the rest of the buffer
  memset(buf , 0, n_shift*8);
  return 0;
}

IMPL(pcm16_to_double) {
  unsigned int n_samples = n_bytes / sizeof(uint16_t);
  buf_array_t *inputs = (buf_array_t*)_inputs;
  buf_array_t *outputs = (buf_array_t*)_outputs;
  // no kwargs
  if ((inputs->n_buffer != 1) || (outputs->n_buffer != 1)) {
    return MISSING_IMPL;
  }

  buffer_t *src = *(inputs->buffers);
  buffer_t *dest = *(outputs->buffers);

  unsigned int i;
  uint16_t pcm_sample;
  double_t float_sample;
  for (i = 0; i < n_samples; i++) {
    buffer_read(src, &pcm_sample, sizeof(uint16_t));
    float_sample = ((float)pcm_sample) / 0x8000;
    buffer_write(dest, &float_sample, sizeof(double_t));
  }  
}

IMPL(double_to_pcm16) {
  unsigned int n_samples = n_bytes / sizeof(double_t);
  buf_array_t *inputs = (buf_array_t*)_inputs;
  buf_array_t *outputs = (buf_array_t*)_outputs;
  // no kwargs
  if ((inputs->n_buffer != 1) || (outputs->n_buffer != 1)) {
    return MISSING_IMPL;
  }

  buffer_t *src = *(inputs->buffers);
  buffer_t *dest = *(outputs->buffers);

  unsigned int i;
  uint16_t pcm_sample;
  double_t float_sample;
  for (i = 0; i < n_samples; i++) {
    buffer_read(src, &float_sample, sizeof(double_t));
    pcm_sample = (uint16_t)(float_sample * 0x8000);
    buffer_write(dest, &pcm_sample, sizeof(uint16_t));
  }  
}

IMPL(fir_filter) {
  unsigned int n_samples = n_bytes / sizeof(double_t);
  buf_array_t *inputs = (buf_array_t*)_inputs;
  buf_array_t *outputs = (buf_array_t*)_outputs;
  unsigned long int b_order = 0;
  double_t *b = NULL;
  buffer_t *hist = NULL;
  int ii;
  for (ii = 0; ii < kwargs->argc; ii++) {
    if (strcmp((kwargs->arg_names)[ii], "b") == 0) {
      b = (double_t*)(kwargs->args[ii]);
    }
    else if (strcmp((kwargs->arg_names)[ii], "b_order") == 0) {
      b_order = (unsigned long int)(kwargs->args[ii]);
    }
    else if (strcmp((kwargs->arg_names)[ii], "hist") == 0) {
      hist = (buffer_t*)(kwargs->args[ii]);
    }
  }
  if ((b == NULL) || (b_order == 0)) {
    return -BAD_ARG;
  }
  if ((inputs->n_buffer != 1) || (outputs->n_buffer != 1)) {
    return -MISSING_IMPL;
  }

  buffer_t *src = *(inputs->buffers);
  buffer_t *dest = *(outputs->buffers);

  unsigned int src_bytes = buffer_n_read_bytes(src);
  if (src->state == EMPTY  || dest->state == FULL) {
    return -EAGAIN;
  }
  if (src_bytes < n_bytes) {
    n_bytes = src_bytes;
  }

  unsigned int i = 0;
  void *fir_buf = malloc(sizeof(double_t)*b_order);
  memset(fir_buf, 0, sizeof(double_t)*b_order);
  if (hist != NULL) {
    unsigned int hist_samples = buffer_n_read_bytes(hist) / sizeof(double_t);
    for (ii = 0; ii < hist_samples; ii++) {
      buffer_read(hist, fir_buf, sizeof(double_t));
      _buf_shift_up_64(fir_buf, sizeof(double_t)*b_order,1);
    }
  }
  //buffer_read(src, fir_buf, sizeof(double_t));
  for (i = 0; i < n_samples; i++) {
    buffer_read(src, fir_buf, sizeof(double_t));
    if (hist != NULL) {
      if (hist->state == FULL) {
        buffer_read(hist, NULL, sizeof(double_t));
      }
      buffer_write(hist, fir_buf, sizeof(double_t));
    }
    double_t new_sample = ddot('n', fir_buf, b, b_order);
    buffer_write(dest, &new_sample, sizeof(double_t));
    _buf_shift_up_64(fir_buf, sizeof(double_t)*b_order, 1);
    //buffer_read(src, fir_buf, sizeof(double_t));
  }
  free(fir_buf);
  return n_bytes;
}

/*! \brief FIR Filter
* \param src source array
* \param dest dest array
* \param b filter coefficients
* \param b_order filter order
* \param n_samples number of samples
* \return exit code
*/
int fir_filter(double_t *src, double_t *dest, double_t *b, \
    unsigned long int b_order, int n_samples) {
  // construct kwargs
  kwargs_t kwargs;
  int argc = 2;
  char *arg_names[] = {"b", "b_order"};
  void **args = (void**)malloc(argc*sizeof(void*));
  args[0] = (void*)b;
  args[1] = (void*)b_order;

  kwargs.argc = argc;
  kwargs.arg_names = arg_names;
  kwargs.args = args;

  // construct input buffer
  buf_array_t inputs, outputs;
  buffer_t *in_buf;
  buffer_t *out_buf;
  buffer_init_ext(&in_buf, n_samples*sizeof(double_t), RING, src);
  buffer_init_ext(&out_buf, n_samples*sizeof(double_t), RING, dest);
  buffer_set_state(in_buf, FULL);
  inputs.n_buffer = 1;
  inputs.buffers = &in_buf;
  outputs.n_buffer = 1;
  outputs.buffers = &out_buf;
  
  fir_filter_impl(n_samples*sizeof(double_t), &inputs, &outputs, &kwargs);

  //clean up
  free(args);
  buffer_delete(&in_buf);
  buffer_delete(&out_buf);
  return 0;
}

int dct_2_64f(buffer_t *src, void *dest, unsigned int n_samples) {
  int ii, jj;
  double_t new_result = 0;
  double_t cos_arg = 0;
  void *dct_buf = malloc(sizeof(double_t)*n_samples);
  buffer_read(src, dct_buf, sizeof(double_t)*n_samples);
  for (ii = 0; ii < n_samples; ii++) {
    new_result = 0;
    cos_arg = 0;
    for (jj = 0; jj < n_samples; jj++) {
      cos_arg = (M_PI * ii * (2*jj + 1)) / (2*n_samples);
      new_result += (((double_t*)dct_buf)[jj] * cosf(cos_arg));
    }
    ((double_t*)dest)[ii] = 2*new_result;
  }
  free(dct_buf);
  return 0;
}

int upsample_linear_32(buffer_t *src, void *dest, unsigned int M) {
  
}
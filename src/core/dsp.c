#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include "intern/dsp.h"
#include "intern/common.h"
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


IMPL(src_sin) {
  double_t _a = 1.0;
  double_t _ph = 0.0;
  buf_array_t *buffers = (buf_array_t*)_outputs;
  buffer_t *buf = *(buffers->buffers);
  double_t *dt_freq, *a, *ph;
  a = &_a;
  ph = &_ph; 
  unsigned int *arg;
  kwargs_unpack(kwargs, "(a;ph;)arg;dt_freq;", &a, &ph, &arg, &dt_freq);
  int n_samples = n_bytes / sizeof(double_t);
  int i = 0;
  double_t *new_samples = (double_t*)malloc(n_bytes);
  for (i = 0; i < n_samples; i++) {
    new_samples[i] = (*a) * sin((*dt_freq)*(i + *arg) + (*ph));
  }
  buffer_write(buf, new_samples, n_bytes);
  (*arg) += (*dt_freq)*n_samples;
  free(new_samples);
  return n_bytes;
}
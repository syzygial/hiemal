#include <stdlib.h>

#include "intern/math.h"
#include "intern/dsp.h"

#include "ops_internal.h"

// [b: double*, b_order: int]
DSP_OP(fir_filter, double*, double*) {
  __FIR_FILTER_ARGS_UNPACK
  unsigned int n_samples = n_bytes / sizeof(double);
  double *fir_buf = (double*)calloc(b_order, sizeof(double));

  int i = 0;
  for (i = 0; i < n_samples; i++) {
    _buf_shift_up_64(fir_buf, sizeof(double) * b_order, 1);
    fir_buf[0] = src[i];
    dest[i] = ddot('n', fir_buf, b, b_order);
  }
  free(fir_buf);
  return n_samples * sizeof(double);
}
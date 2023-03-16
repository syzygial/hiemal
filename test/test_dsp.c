#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "intern/core.h"
#include "intern/dsp.h"
#include "test_common.h"

TEST(shift_down_64) {
  const unsigned int n_items = 6;
  uint64_t buf[] = {123,234,345,456,567,678};
  uint64_t ref[] = {345,456,567,678,0,0};
  int rc;
  rc = _buf_shift_down_64(buf, n_items*8, 2);
  ASSERT_TRUE(rc == 0);
  ASSERT_TRUE(memcmp((void*)buf, (void*)ref, n_items*8) == 0);
  return TEST_SUCCESS;
}

TEST(shift_up_64) {
  const unsigned int n_items = 6;
  uint64_t buf[] = {123,234,345,456,567,678};
  uint64_t ref[] = {0, 0, 123,234,345,456};
  int rc;
  rc = _buf_shift_up_64(buf, n_items*8, 2);
  ASSERT_TRUE(rc == 0);
  ASSERT_TRUE(memcmp((void*)buf, (void*)ref, n_items*8) == 0);
  return TEST_SUCCESS;
}

TEST(fir_filter_impl) {
  int rc1, rc2, rc3;
  unsigned int n_samples = 6;
  long unsigned int b_order = 3;
  double_t signal[] = {1,2,3,4,5,6};
  double_t filtered_signal_ref[] = {0.1, 0.5, 2.4, 4.3, 6.2, 8.1};
  double_t b[] = {0.1, 0.3, 1.5};
  buffer_t *filtered_signal = NULL;
  buffer_t *signal_buf = NULL;
  buffer_t *hist = NULL;
  buffer_init(&filtered_signal, n_samples * sizeof(double_t), RING);
  buffer_init(&hist, (b_order - 1)*sizeof(double_t), RING);
  buffer_init_ext(&signal_buf, n_samples * sizeof(double_t), RING, signal);
  signal_buf->state = FULL;
  double_t tol = 1e-5;
  buf_array_t inputs, outputs;
  inputs.n_buffer = 1;
  outputs.n_buffer = 1;
  inputs.buffers = &signal_buf;
  outputs.buffers = &filtered_signal;
  kwargs_t kwargs;
  kwargs.argc = 3;
  kwargs.arg_names = (char*[]){"b", "b_order", "hist"};
  kwargs.args = (void**)malloc(kwargs.argc*sizeof(void*));
  kwargs.args[0] = (void*)b;
  kwargs.args[1] = (void*)b_order;
  kwargs.args[2] = (void*)hist;
  rc1 = fir_filter_impl(b_order*sizeof(double_t), &inputs, &outputs, &kwargs);
  ASSERT_TRUE(rc1 == b_order*sizeof(double_t));
  ASSERT_ALMOST_EQUAL_1D(filtered_signal->buf, 
    filtered_signal_ref,b_order,tol);
  rc2 = fir_filter_impl(1*sizeof(double_t), &inputs, &outputs, &kwargs);
  ASSERT_TRUE(rc2 == 1*sizeof(double_t));
  ASSERT_ALMOST_EQUAL_1D(filtered_signal->buf, 
    filtered_signal_ref,b_order+1,tol);
  rc3 = fir_filter_impl((n_samples - (b_order+1))*sizeof(double_t), &inputs, &outputs, &kwargs);
  ASSERT_TRUE(rc3 == (n_samples - (b_order+1))*sizeof(double_t));
  ASSERT_ALMOST_EQUAL_1D(filtered_signal->buf, 
    filtered_signal_ref,b_order,tol);
  free(kwargs.args);
  buffer_delete(&filtered_signal);
  buffer_delete(&hist);
  buffer_delete(&signal_buf);
  return TEST_SUCCESS;
}

TEST(fir_filter) {
  int rc;
  unsigned int n_samples = 6;
  long unsigned int b_order = 3;
  double_t signal[] = {1,2,3,4,5,6};
  double_t filtered_signal_ref[] = {0.1, 0.5, 2.4, 4.3, 6.2, 8.1};
  double_t b[] = {0.1, 0.3, 1.5};
  double_t filtered_signal[6];
  double_t tol = 1e-5;
  rc = fir_filter(signal, filtered_signal, b, b_order, n_samples);
  ASSERT_TRUE(rc == 0);
  ASSERT_ALMOST_EQUAL_1D(filtered_signal, 
    filtered_signal_ref,n_samples,tol);
  return TEST_SUCCESS;
}

TEST(dct_type_II) {
  //set up
  int rc;
  double_t tol = 1e-3;
  unsigned int n_samples = 10;
  double_t signal[] = {0.3, 1.5, 0.9, 6.3, -0.6, -1.2, 0.2, 3.4, 2.6, 1.1};
  double_t dct_ref[] = {29.0, -1.34964, 3.26539, -10.8288, -15.7984, 13.435, 8.3269,
       -1.24971, -8.79837, -12.4721};
  buffer_t *rbuf = NULL;
  buffer_init(&rbuf, sizeof(double_t)*n_samples, RING);
  buffer_write(rbuf, signal, sizeof(double_t)*n_samples);
  double_t dct[n_samples];
  // test
  rc = dct_2_64f(rbuf, dct, n_samples);
  ASSERT_TRUE(rc == 0);
  ASSERT_ALMOST_EQUAL_1D(dct, dct_ref, n_samples, tol);
  // tear down
  buffer_delete(&rbuf);
  return TEST_SUCCESS;
}

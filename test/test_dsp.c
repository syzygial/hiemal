#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "intern/common.h"
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

TEST(src_sin_impl) {
  double_t tol = 1e-3;
  int n_samples = 5;
  int rc = 0;
  unsigned long int arg = 0;
  double_t dt_freq = 0.1;
  double_t sin_ref[] = {0.0, 0.0998, 0.1987, 0.2955, 0.3894};
  kwargs_t kwargs;
  kwargs.argc = 2;
  char *arg_names[] = {"arg", "dt_freq"};
  void *args[] = {(void*)(&arg), (void*)(&dt_freq)};
  kwargs.arg_names = arg_names;
  kwargs.args = args;
  buffer_t *buf = NULL;
  buf_array_t out;
  out.n_buffer = 1;
  out.buffers = &buf;
  buffer_init(&buf, sizeof(double_t)*n_samples, RING);
  rc = src_sin_impl(n_samples*sizeof(double_t), NULL, &out, &kwargs);
  ASSERT_TRUE(rc == n_samples * sizeof(double_t));
  ASSERT_ALMOST_EQUAL_1D(sin_ref, buf->buf, n_samples, tol)
  buffer_delete(&buf);
  return TEST_SUCCESS;
}
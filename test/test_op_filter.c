#include "ops.h"

#include "test_common.h"

TEST(fir_filter_op) {
  int rc;
  unsigned int n_samples = 6;
  unsigned int n_bytes = n_samples * sizeof(double);
  long unsigned int b_order = 3;
  double signal[] = {1,2,3,4,5,6};
  double filtered_signal_ref[] = {0.1, 0.5, 2.4, 4.3, 6.2, 8.1};
  double b[] = {0.1, 0.3, 1.5};
  double filtered_signal[6];
  double tol = 1e-5;
  rc = fir_filter(signal, filtered_signal, n_bytes, b, b_order);
  ASSERT_TRUE(rc == n_bytes);
  ASSERT_ALMOST_EQUAL_1D(filtered_signal, 
    filtered_signal_ref,n_samples,tol);
  return TEST_SUCCESS;
}
#include "test_math.h"
#include "test_common.h"
#include <stdio.h>

TEST(math_blas_ddot) {
  unsigned int len = 4;
  double_t tol = 1e-5;
  double_t a[] = {1.0, 2.0, 3.0, 4.0};
  double_t b[] = {2.2, 3.3, 4.4, 5.5};
  double_t dot = ddot('n', a, b, len);
  double_t dotf = ddot('f', a, b, len);
  double_t res = 44.0;
  double_t resf = 33.0;
  ASSERT_ALMOST_EQUAL(dot, res, tol);
  ASSERT_ALMOST_EQUAL(dotf, resf, tol);
  return TEST_SUCCESS;
}
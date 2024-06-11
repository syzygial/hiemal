#include <math.h>
#include <stdio.h>

#include "test_common.h"

const int TEST_SUCCESS = 0;
const int TEST_FAILURE = 1;
const int TEST_SKIPPED = 2;
unsigned int LOG_LEVEL = 1;
inline int _assert_true(int expr) {
  return (expr) ? 1 : 0;
}

inline int _assert_false(int expr) {
  return !_assert_true(expr);
}

inline int _assert_almost_equal(double a, double b, double tol) {
  return (fabs(a - b) < tol) ? 1 : 0;
}

int _assert_almost_equal_1d(void* a, void* b, unsigned int len, double tol) {
  double_t residual = 0;
  int i;
  for (i = 0; i < len; i++) {
    residual += fabs(((double_t*)a)[i] - ((double_t*)b)[i]);
  }
  return (residual < tol) ? 1 : 0;
}

// file utils

#ifdef __unix__
#include <unistd.h>
#include <sys/stat.h>

int create_reg_file(const char *name) {
  FILE *f = fopen(name, "a");
  return fclose(f);
}

int delete_reg_file(const char *name) {
  struct stat s;
  if (stat(name, &s) != 0) {
    return -1;
  }
  else if ((s.st_mode & S_IFMT) != S_IFREG) {
    return -1;
  }
  return unlink(name);
}

int create_dir(const char *name) {
  return mkdir(name, 0755);
}

int delete_dir(const char *name) {
  struct stat s;
  if (stat(name, &s) != 0) {
    return -1;
  }
  else if ((s.st_mode & S_IFMT) != S_IFDIR) {
    return -1;
  }
  return rmdir(name);
}
#endif
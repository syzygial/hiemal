#ifndef _TEST_COMMON_H
#define _TEST_COMMON_H

#include <stddef.h>

extern const int TEST_SUCCESS;
extern const int TEST_FAILURE;
extern const int TEST_SKIPPED;
extern unsigned int LOG_LEVEL;

#define TEST(name) int test_##name()
typedef int (test_fn)();

#define ASSERT_TRUE(expr) if (!_assert_true(expr)) { return TEST_FAILURE; }
#define ASSERT_FALSE(expr) if (!_assert_false(expr)) { return TEST_FAILURE; }
#define ASSERT_ALMOST_EQUAL(a,b,tol) if(!_assert_almost_equal(a,b,tol)) { return TEST_FAILURE; }
#define ASSERT_ALMOST_EQUAL_1D(a,b,len,tol) if(!_assert_almost_equal_1d(a,b,len,tol)) { return TEST_FAILURE; }

int _assert_true(int expr);
int _assert_false(int expr);
int _assert_almost_equal(double a, double b, double tol);
int _assert_almost_equal_1d(void* a, void* b, unsigned int len, double tol);
unsigned int get_log_level();
int set_log_level(unsigned int lvl);
#endif

// file utils

int create_reg_file(const char *name);
int delete_reg_file(const char *name);
int create_dir(const char *name);
int delete_dir(const char *name);
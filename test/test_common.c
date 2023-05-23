#include "test_common.h"
#include "intern/common.h"

#include <string.h>
#include <stddef.h>
#include <math.h>

TEST(next_arg) {
  bool opt = false;
  int arglen = 0;
  char *test_str = "(a;a_order;)b;b_order;";
  char *str_itr = test_str;
  ASSERT_TRUE((str_itr = next_arg(test_str, ';', NULL, &arglen, &opt)) == (test_str + 1));
  ASSERT_TRUE(arglen == -1);
  ASSERT_TRUE(opt == true);
  
  ASSERT_TRUE((str_itr = next_arg(test_str + 1, ';', NULL, &arglen, &opt)) == (test_str + 3));
  ASSERT_TRUE(arglen == 1);
  ASSERT_TRUE(opt == true);
  
  ASSERT_TRUE((str_itr = next_arg(test_str + 3, ';', NULL, &arglen, &opt)) == (test_str + 12));
  ASSERT_TRUE(arglen == 7);
  ASSERT_TRUE(opt == false);
  
  ASSERT_TRUE((str_itr = next_arg(test_str + 12, ';', NULL, &arglen, &opt)) == (test_str + 14));
  ASSERT_TRUE(arglen == 1);
  ASSERT_TRUE(opt == false);
  
  ASSERT_TRUE((str_itr = next_arg(test_str + 14, ';', NULL, &arglen, &opt)) == NULL);
  ASSERT_TRUE(arglen == 7);
  ASSERT_TRUE(opt == false);
  
  return TEST_SUCCESS;
}

TEST(kwargs_unpack) {
  kwargs_t kwargs;
  double_t _b[] = {1.2, 3.2, -1.4};
  unsigned long int _b_order = 3;
  kwargs.argc = 2;
  char *arg_names[] = {"b", "b_order"};
  void *args[] = {(void*)_b, (void*)_b_order};
  kwargs.arg_names = arg_names;
  kwargs.args = args;

  double_t *a = NULL;
  double_t *b = NULL;
  unsigned long int a_order = 0;
  unsigned long int b_order = 0;
  int rc = kwargs_unpack(&kwargs, "(a;a_order;)b;b_order;", &a, &a_order, &b, &b_order);
  ASSERT_TRUE(rc == 0);
  ASSERT_TRUE(a == NULL);
  ASSERT_TRUE(a_order == 0);
  ASSERT_TRUE(b == _b);
  ASSERT_TRUE(b_order == 3);
  return TEST_SUCCESS;
}
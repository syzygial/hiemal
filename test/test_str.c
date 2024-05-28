#include "util/str.h"
#include "test_common.h"

TEST(int_str) {
  char *str1 = "1234";
  char *str2 = "1a34";
  ASSERT_TRUE(is_int_str(str1));
  ASSERT_FALSE(is_int_str(str2));
  return TEST_SUCCESS;
}

TEST(int_range) {
  char *str1 = "1-15";
  char *str2 = "1-2-3";
  char *str3 = "4-1";
  ASSERT_TRUE(is_int_range(str1));
  ASSERT_FALSE(is_int_range(str2));
  ASSERT_FALSE(is_int_range(str3));

  int range[2] = {-1,-1};
  ASSERT_TRUE(parse_int_range(str1, range) == 0)
  ASSERT_TRUE((range[0] == 1) && (range[1] == 15))
  return TEST_SUCCESS;
}
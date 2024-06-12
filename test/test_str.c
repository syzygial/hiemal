#include <stdlib.h>
#include <string.h>

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

TEST(str_in_arr) {
  char **str_arr = (char*[]){"hello", "world", "abc", NULL};
  ASSERT_TRUE(str_in_arr("world", str_arr))
  ASSERT_TRUE(str_in_arr("abc", str_arr))
  ASSERT_FALSE(str_in_arr("xyz", str_arr))
  return TEST_SUCCESS;
}

TEST(str_arr_len) {
  char **arr = (char*[]){"hello", "world", "abc", "123", NULL};
  int arr_len = 4;
  ASSERT_TRUE(str_arr_len(arr) == arr_len)
  return TEST_SUCCESS;
}

TEST(str_arr_eq) {
  char **arr1 = (char*[]){"hello", "world", "abc", "123", NULL};
  char **arr2 = (char*[]){"hello", "world", "abc", "123", NULL};
  char **arr3 = (char*[]){"hello", "world", "abc", "xyz", NULL};
  ASSERT_TRUE(str_arr_eq(arr1, arr2) == 1)
  ASSERT_TRUE(str_arr_eq(arr1, arr3) == 0)
  return TEST_SUCCESS;
}

TEST(str_arr_copy) {
  char **arr1 = (char*[]){"hello", "world", "abc", "123", NULL};
  char **arr2 = (char*[]){NULL, NULL, NULL, NULL, NULL};
  ASSERT_TRUE(str_arr_copy(arr2, arr1) == 0)
  ASSERT_TRUE(str_arr_eq(arr1, arr2) == 1)
  return TEST_SUCCESS;
}

TEST(str_arr_copy_clear) {
  // setup
  int arr_len = 2;
  char *s1 = "hello";
  char *s2 = "world";
  char **arr = (char**)calloc(arr_len+1, sizeof(char*));
  arr[0] = (char*)calloc(strlen(s1) + 1, sizeof(char));
  arr[1] = (char*)calloc(strlen(s2) + 1, sizeof(char));
  strcpy(arr[0], s1);
  strcpy(arr[1], s2);

  ASSERT_TRUE(str_arr_copy_clear(arr) == 0)

  // teardown
  free(arr);
  return TEST_SUCCESS;
}

TEST(str_arr_copy_m) {
  char **arr1 = (char*[]){"hello", "world", "abc", "123", NULL};
  char **arr2 = NULL;
  ASSERT_TRUE(str_arr_copy_m(&arr2, arr1) == 0)
  ASSERT_TRUE(arr2 != NULL)
  ASSERT_TRUE(str_arr_eq(arr1, arr2) == 1)
  ASSERT_TRUE(str_arr_copy_clear(arr2) == 0)
  free(arr2);
  return TEST_SUCCESS;
}

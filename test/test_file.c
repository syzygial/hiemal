#include "util/file.h"
#include "test_common.h"

#include <stdio.h>

TEST(is_regular_file) {
  const char *fname1 = "file1.txt";
  const char *fname2 = "file2.txt";
  ASSERT_TRUE(create_reg_file(fname1) == 0)
  ASSERT_TRUE(is_regular_file(fname1) == 1)
  ASSERT_TRUE(is_regular_file(fname2) == 0)
  ASSERT_TRUE(delete_reg_file(fname1) == 0)
  return TEST_SUCCESS;
}

TEST(is_dir) {
  const char *dname1 = "dir1";
  const char *dname2 = "dir2";
  ASSERT_TRUE(create_dir(dname1) == 0)
  ASSERT_TRUE(is_dir(dname1) == 1)
  ASSERT_TRUE(is_dir(dname2) == 0)
  ASSERT_TRUE(delete_dir(dname1) == 0)
  return TEST_SUCCESS;
}

TEST(is_valid_path) {
  const char *dname = "dir1";
  const char *fname1 = "dir1/file1.txt";
  const char *fname2 = "dir1/file2.txt";
  ASSERT_TRUE(create_dir(dname) == 0)
  ASSERT_TRUE(create_reg_file(fname1) == 0)
  ASSERT_TRUE(is_valid_path(dname) == 1)
  ASSERT_TRUE(is_valid_path(fname1) == 1)
  ASSERT_TRUE(is_valid_path(fname2) == 0)
  ASSERT_TRUE(delete_reg_file(fname1) == 0)
  ASSERT_TRUE(delete_dir(dname) == 0)
  return TEST_SUCCESS;
}
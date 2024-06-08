#include "util/args.h"
#include "test_common.h"

#include <limits.h>
#include <string.h>

TEST(_parse_arg_sig) {
  char *sig1 = "+";
  char *sig2 = "*";
  char *sig3 = "2+";
  char *sig4 = "3";
  // test 1
  {
    int range[4][2] = {{-1,-1},{-1,-1},{-1,-1},{-1,-1}};
    ASSERT_TRUE(_parse_arg_sig(sig1, range[0]) == 0)
    ASSERT_TRUE(range[0][0] == 1 && range[0][1] == INT_MAX)
    ASSERT_TRUE(_parse_arg_sig(sig2, range[1]) == 0)
    ASSERT_TRUE(range[1][0] == 0 && range[1][1] == INT_MAX)
    ASSERT_TRUE(_parse_arg_sig(sig3, range[2]) == 0)
    ASSERT_TRUE(range[2][0] == 2 && range[2][1] == INT_MAX)
    ASSERT_TRUE(_parse_arg_sig(sig4, range[3]) == 0)
    ASSERT_TRUE(range[3][0] == 3 && range[3][1] == 3)
  }
  return TEST_SUCCESS;
}

TEST(parse_args) {
  // test 1
  {
    hm_arg args[] = {
      {"test", 't', "+", COLLECT, NULL},
      {NULL, 0, NULL, 0, NULL}
    };
    ASSERT_TRUE(args_init(args) == 0)
    int argc = 3;
    char **argv = (char*[]){"--test", "hello", "world", NULL};
    int rc = parse_args(args, argc, argv);
    ASSERT_TRUE(rc == 0)
    ASSERT_TRUE(strcmp(((char**)(args[0].result))[0],"hello") == 0)
    ASSERT_TRUE(strcmp(((char**)(args[0].result))[1],"world") == 0)
    ASSERT_TRUE(((char**)(args->result))[2] == NULL)
    ASSERT_TRUE(args_clear(args) == 0)
  }

  // test 2
  {
    hm_arg args[] = {
      {HM_ARG_DEFAULT, '\0', "+", COLLECT, NULL},
      {"test", 't', "2", COLLECT, NULL},
      {NULL, 0, NULL, 0, NULL}
    };
    ASSERT_TRUE(args_init(args) == 0)
    int argc = 5;
    char **argv = (char*[]){"--test", "hello", "world", "123", "abc", NULL};
    int rc = parse_args(args, argc, argv);
    ASSERT_TRUE(rc == 0)
    ASSERT_TRUE(strcmp(((char**)(args[0].result))[0],"123") == 0)
    ASSERT_TRUE(strcmp(((char**)(args[0].result))[1],"abc") == 0)
    ASSERT_TRUE(((char**)(args[0].result))[2] == NULL)
    ASSERT_TRUE(strcmp(((char**)(args[1].result))[0],"hello") == 0)
    ASSERT_TRUE(strcmp(((char**)(args[1].result))[1],"world") == 0)
    ASSERT_TRUE(((char**)(args[1].result))[2] == NULL)
    ASSERT_TRUE(args_clear(args) == 0)
  }

  return TEST_SUCCESS;
}
#include "util/args.h"
#include "test_common.h"

#include <string.h>

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
    ASSERT_TRUE(rc == 0);
    ASSERT_TRUE(strcmp(((char**)(args->result))[0],"hello") == 0)
    ASSERT_TRUE(strcmp(((char**)(args->result))[1],"world") == 0)
    ASSERT_TRUE(((char**)(args->result))[2] == NULL)
    ASSERT_TRUE(args_clear(args) == 0)
  }
  return TEST_SUCCESS;
}
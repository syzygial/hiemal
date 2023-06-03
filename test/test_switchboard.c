#include "intern/switchboard.h"
#include "test_common.h"

TEST(switchboard_init_delete) {
  switchboard_t *s = NULL;
  int rc = 0;
  rc = switchboard_init(&s);
  ASSERT_TRUE(rc == 0);
  ASSERT_TRUE(s != NULL);
  ASSERT_TRUE(is_list(s->nodes));
  ASSERT_TRUE(is_list(s->connections));
  ASSERT_TRUE(is_list(s->context_list));
  rc = switchboard_delete(&s);
  ASSERT_TRUE(rc == 0);
  ASSERT_TRUE(s == NULL);
  return TEST_SUCCESS;
}
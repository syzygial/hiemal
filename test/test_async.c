#include "test_async.h"
#include "test_common.h"
#include <stddef.h>

TEST(async_loop_init_delete) {
  async_handle_t *h = NULL;
  async_loop_init(&h);
  ASSERT_TRUE(h != NULL);
  ASSERT_TRUE(h->n_fn == 0);
  ASSERT_TRUE(*(h->loop_active) == false);
  async_loop_delete(&h);
  ASSERT_TRUE(h == NULL);
  return TEST_SUCCESS;
}
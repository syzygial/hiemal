#include "test_async.h"
#include "mock/mock_async.h"
#include "test_common.h"
#include <stddef.h>

TEST(async_loop_init_delete) {
  async_handle_t *h = NULL;
  async_loop_init(&h);
  ASSERT_TRUE(h != NULL);
  ASSERT_TRUE(h->n_fn == 0);
  ASSERT_TRUE(*(h->loop_active) == false);
  ASSERT_TRUE(*(h->thread_created) == false);
  async_loop_delete(&h);
  ASSERT_TRUE(h == NULL);
  return TEST_SUCCESS;
}

TEST(async_loop_add_fn) {
  // setup
  buffer_t *src = NULL;
  buffer_t *dest = NULL;
  buffer_init(&src, 0x1000, LINEAR);
  buffer_init(&dest, 0x1000, LINEAR);
  int rc;
  async_handle_t *h = NULL;
  async_loop_init(&h);

  // test
  rc = async_loop_add_fn(h, times_two_async(src, dest));
  ASSERT_TRUE(rc == 0);
  ASSERT_TRUE(h->n_fn == 1);
  ASSERT_TRUE(h->inputs[0] == src);
  ASSERT_TRUE(h->outputs[0] == dest);
  
  // teardown
  async_loop_delete(&h);
  buffer_delete(&src);
  buffer_delete(&dest);
  return TEST_SUCCESS;
}
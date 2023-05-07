#include "intern/async.h"
#include "mock/mock_async.h"
#include "test_common.h"
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>

TEST(async_loop_init_delete) {
  async_handle_t *h = NULL;
  async_loop_init(&h);
  ASSERT_TRUE(h != NULL);
  ASSERT_TRUE(h->n_fn == 0);
  ASSERT_TRUE(h->loop_active == false);
  ASSERT_TRUE(h->thread_created == false);
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

TEST(async_loop_dispatch) {
  // setup
  buffer_t *src = NULL;
  buffer_t *buf = NULL;
  buffer_t *dest = NULL;
  double_t input_data[5] = {1.1, 2.2, 3.3, 4.4, 5.5};
  double_t output_data_ref[5] = {4.4, 8.8, 13.2, 17.6, 22};
  buffer_init(&src, 5*sizeof(double_t), RING);
  buffer_init(&buf, 5*sizeof(double_t), RING);
  buffer_init(&dest, 5*sizeof(double_t), RING);
  int rc;
  double_t tol = 1e-5;
  async_handle_t *h = NULL;
  async_loop_init(&h);
  async_loop_add_fn(h, times_two_async(src, buf));
  async_loop_add_fn(h, times_two_async(buf, dest));
  buffer_write(src, input_data, 5*sizeof(double_t));
  async_loop_dispatch(h);
  while(dest->state != FULL);
  async_loop_stop(h);
  ASSERT_ALMOST_EQUAL_1D(dest->buf, output_data_ref, 5, tol);

  // teardown
  async_loop_delete(&h);
  buffer_delete(&src);
  buffer_delete(&buf);
  buffer_delete(&dest);  
  return TEST_SUCCESS;
}

TEST(async_fd_map) {
  async_fd_map_t *test_map = NULL;
  int rc = async_fd_map_init(&test_map);
  ASSERT_TRUE(rc == 0);
  ASSERT_TRUE(test_map != NULL);

  rc = async_fd_map_insert(test_map, 3, INTERN_PIPE, 7);
  ASSERT_TRUE(rc == 0);
  rc = async_fd_map_insert(test_map, 3, EXTERN, 2);
  ASSERT_TRUE(rc == 0);
  ASSERT_TRUE(async_fd_map_n_keys(test_map) == 1);

  rc = async_fd_map_get_index(test_map, 3);
  ASSERT_TRUE(rc == 7)
  rc = async_fd_map_get_index(test_map, 4);
  ASSERT_TRUE(rc == -1)


  rc = async_fd_map_delete(&test_map);
  ASSERT_TRUE(rc == 0);
  ASSERT_TRUE(test_map == NULL);
  return TEST_SUCCESS;
}
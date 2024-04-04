#include <string.h>

#include "intern/buffer.h"
#include "intern/switchboard.h"
#include "intern/thread.h"
#include "intern/semaphore.h"
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

TEST(switchboard_buf_node) {
  switchboard_t *s = NULL;
  buffer_t *buf = NULL;
  buffer_init(&buf, 0x1000, RING);
  switchboard_init(&s);
  switchboard_add_buffer(s, buf, "buf:0");

  switchboard_node_t *node = switchboard_get_node_by_name(s, "buf:0");
  ASSERT_TRUE(node != NULL)
  ASSERT_TRUE((node->res).buf == buf)

  buffer_delete(&buf);
  switchboard_delete(&s);
  return TEST_SUCCESS;
}

TEST(switchboard_connection) {
  switchboard_t *s = NULL;
  buffer_t *buf0 = NULL;
  buffer_t *buf1 = NULL;
  buffer_init(&buf0, 0x1000, RING);
  buffer_init(&buf1, 0x1000, RING);
  switchboard_init(&s);
  switchboard_add_buffer(s, buf0, "buf:0");
  switchboard_add_buffer(s, buf1, "buf:1");
  switchboard_add_connection_by_name(s, "buf:0", "buf:1", "connection:0", BIDIRECTIONAL);

  switchboard_node_t *node0 = switchboard_get_node_by_name(s, "buf:0");
  switchboard_node_t *node1 = switchboard_get_node_by_name(s, "buf:1");
  ASSERT_TRUE(node0->connections->n_items == 1)
  ASSERT_TRUE(node1->connections->n_items == 1)
  switchboard_connection_t *conn = hm_list_node_extract(node0->connections->head);
  ASSERT_TRUE(((conn->nodes)[0] == node0) && ((conn->nodes)[1] == node1))

  switchboard_delete(&s);
  buffer_delete(&buf0);
  buffer_delete(&buf1);
  return TEST_SUCCESS;
}

TEST(switchboard_context) {
  semaphore_t sem;
  switchboard_t *s;
  switchboard_node_t *node;
  void *ctx_test(void *args) {
    switchboard_add_context(s, node);
    semaphore_inc(&sem);
    return NULL;
  }
  semaphore_init(&sem, 0);
  switchboard_init(&s);
  switchboard_context_list_t *ctx_list = s->context_list;
  buffer_t *buf = NULL;
  buffer_init(&buf, 0x1000, RING);
  switchboard_add_buffer(s, buf, "buf:0");
  node = switchboard_get_node_by_name(s, "buf:0");
  switchboard_add_context(s, node);
  HM_THREAD_CREATE(ctx_test, NULL)
  semaphore_dec(&sem); // wait for thread to establish context
  ASSERT_TRUE(ctx_list->n_items == 2)
  ASSERT_TRUE(ctx_list->head->thread_id == thread_id())
  ASSERT_TRUE(ctx_list->head->next->thread_id != 0 && ctx_list->head->next->thread_id != thread_id())
  thread_join_all();
  buffer_delete(&buf);
  switchboard_delete(&s);
  semaphore_delete(&sem);
  return TEST_SUCCESS;
}

TEST(switchboard_send_buf_buf) {
  buffer_t *buf0 = NULL;
  buffer_t *buf1 = NULL;
  switchboard_t *s = NULL;
  int my_dat[] = {1,2,3,4,5};
  switchboard_init(&s);
  buffer_init(&buf0, 0x1000, RING);
  buffer_init(&buf1, 0x1000, RING);
  switchboard_add_buffer(s, buf0, "buf:0");
  switchboard_add_buffer(s, buf1, "buf:1");
  buffer_write(buf0, my_dat, sizeof(my_dat));
  switchboard_add_connection_by_name(s, "buf:0", "buf:1", "connection:0", BIDIRECTIONAL);
  switchboard_node_acquire(s, "buf:0");
  switchboard_node_acquire(s, "buf:1");
  switchboard_send(s, "buf:0", "buf:1", sizeof(my_dat), 0);
  ASSERT_TRUE(strncmp((char*)my_dat, (char*)(buf1->buf), sizeof(my_dat)) == 0)
  switchboard_delete(&s);
  buffer_delete(&buf0);
  buffer_delete(&buf1);
  return TEST_SUCCESS;
}
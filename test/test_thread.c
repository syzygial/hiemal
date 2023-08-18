#include "intern/thread.h"
#include "test_common.h"

TEST(hm_thread) {

  int ii = 0;
  hm_mutex_t m;
  thread_mutex_init(&m);
  void *fn1(void *args) {
    thread_mutex_lock(&m);
    ii += 1;
    thread_mutex_unlock(&m);
    return NULL;
  }

  HM_THREAD_CREATE(fn1, NULL)
  HM_THREAD_CREATE(fn1, NULL)
  thread_join_all();
  ASSERT_TRUE(ii == 2)
  thread_mutex_delete(&m);
  return TEST_SUCCESS;
}
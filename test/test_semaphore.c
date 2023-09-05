#include "intern/semaphore.h"
#include "test_common.h"

TEST(semaphore) {
  semaphore_t sem;
  semaphore_init(&sem, 3);
  ASSERT_TRUE(semaphore_val(&sem) == 3)
  semaphore_inc(&sem);
  ASSERT_TRUE(semaphore_val(&sem) == 4)
  semaphore_dec(&sem);
  semaphore_dec(&sem);
  ASSERT_TRUE(semaphore_val(&sem) == 2)
  semaphore_delete(&sem);

  return TEST_SUCCESS;
}
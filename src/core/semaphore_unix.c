#include "intern/common.h"
#include "intern/semaphore.h"

int semaphore_init(semaphore_t *sem, int val) {
  return sem_init(sem, 0, val);
}

int semaphore_inc(semaphore_t *sem) {
  return sem_post(sem);
}

int semaphore_dec(semaphore_t *sem) {
  return sem_wait(sem);
}

int semaphore_val(semaphore_t *sem) {
  int val = 0;
  sem_getvalue(sem, &val);
  return val;
}

int semaphore_delete(semaphore_t *sem) {
  return sem_destroy(sem);
}
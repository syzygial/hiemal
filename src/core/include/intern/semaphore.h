#ifndef _INTERN_SEMAPHORE_H
#define _INTERN_SEMAPHORE_H

#ifdef __unix__
#include <semaphore.h>
typedef sem_t semaphore_t;
#endif

int semaphore_init(semaphore_t *sem, int val);
int semaphore_inc(semaphore_t *sem);
int semaphore_dec(semaphore_t *sem);
int semaphore_val(semaphore_t *sem);
int semaphore_delete(semaphore_t *sem);

#endif
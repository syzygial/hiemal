#include "intern/thread.h"

int thread_init(void *(*fn)(void*), void *args, const char *file, const char *func, int line) {
  hm_thread_id new_thread_id;
  thread_info_t *new_thread_info = (thread_info_t*)malloc(sizeof(thread_info_t));
  HM_LIST_NODE_INIT(new_thread_info)
  xasprintf((char**)&(new_thread_info->created_by), "%s:%s (%d)", \
    file, func, line);
  pthread_create(&new_thread_id, NULL, fn, args);
  new_thread_info->thread_id = new_thread_id;
  hm_list_append(threads_info, new_thread_info);
  return 0;
}

int thread_mutex_init(hm_mutex_t *m) {
  return pthread_mutex_init(m, NULL);
}

int thread_mutex_lock(hm_mutex_t *m) {
  return pthread_mutex_lock(m);
}

int thread_mutex_unlock(hm_mutex_t *m) {
  return pthread_mutex_unlock(m);
}

int thread_mutex_delete(hm_mutex_t *m) {
  return pthread_mutex_destroy(m);
}

int thread_join(hm_thread_id id) {
  pthread_join(id, NULL);
  return 0;
}

hm_thread_id thread_id() {
  return pthread_self();
}

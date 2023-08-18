#include <stdlib.h>

#include "intern/thread.h"

#if defined(unix) && defined(__GNUC__)
int threads_info_init() __attribute__((constructor));
int threads_info_cleanup() __attribute__((destructor));
#endif

struct _threads_info {
  HM_LIST_HEAD(thread_info_t)
};

threads_info_t *threads_info = NULL;

int threads_info_init() {
  if (threads_info == NULL) {
    threads_info = (threads_info_t*)malloc(sizeof(threads_info_t));
    HM_LIST_INIT(threads_info)
    return 0;
  }
  else {
    return -1;
  }
}

int threads_info_cleanup() {
  if (threads_info != NULL) {
    free(threads_info);
    return 0;
  }
  else {
    return -1;
  }
}

int threads_info_reset() {
  if (threads_info == NULL) return 0;
  free(threads_info);
  threads_info = (threads_info_t*)malloc(sizeof(threads_info_t));
  return 0;
}

int _thread_info_join(hm_list_node_t *t) {
  thread_info_t *_t = (thread_info_t*)t;
  thread_join(_t->thread_id);
  return 0;
}

int thread_join_all() {
  hm_list_itr(threads_info, _thread_info_join);
  return 0;
}
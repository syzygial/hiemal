#include "intern/common.h"



typedef struct _hm_event hm_event_t;
typedef struct _hm_event_list hm_event_list_t;
union hm_event_obj {
  int fd;
  hm_mutex_t mutex;
};

typedef int (event_condition)(hm_event_t*);

struct _hm_event {
  union hm_event_obj obj;
  event_condition *cond;
};

struct _hm_event_list {
  HM_LIST_HEAD(hm_event_t)
};

int hm_event_list_init(hm_event_list_t **l) {
  return 0;
}

int hm_event_list_delete(hm_event_list_t **l) {
  return 0;
}

int hm_event_add(hm_event_list_t *l, hm_event_t *e) {
  return 0;
}

int hm_event_poll(hm_event_t *e) {
  return 0;
}

int hm_event_poll_list(hm_event_list_t *l) {
  return 0;
}

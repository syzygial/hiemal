#ifndef _INTERN_COMMON_H
#define _INTERN_COMMON_H

#include <assert.h>
#include <stdint.h>
#include <stdbool.h>

#include "cmake_info.h"

#include "error.h"

enum {
  HM_LIST_MAGIC=0x1,
  HM_LIST_NODE_MAGIC
};

#define HM_LIST_ASSERT(name) assert(name->magic == HM_LIST_MAGIC);
#define HM_LIST_NODE_ASSERT(name) assert(name->magic == HM_LIST_NODE_MAGIC);

#define UNIT_TESTING

#ifdef UNIT_TESTING
#define STATIC
#else
#define STATIC static
#endif


#define HM_FLAG_SET(flags, flag) ((flags & flag) == flag)


typedef void hm_type_t;
typedef void hm_list_t;
typedef void hm_list_node_t;

#define HM_TYPE_HEAD uint64_t magic;
#define HM_LIST_HEAD(type) uint64_t magic; type *head; unsigned int n_items;
#define HM_LIST_NODE_HEAD(type) uint64_t magic; type *prev; type *next;

#define HM_LIST_INIT(name) name->head = NULL; name->n_items = 0; name->magic = HM_LIST_MAGIC;
#define HM_LIST_NODE_INIT(name) name->prev = NULL; name->next = NULL; name->magic = HM_LIST_NODE_MAGIC;

typedef int (list_node_fn)(hm_list_node_t *node); 

typedef struct _kwargs {
  int argc;
  char **arg_names;
  void **args;
} kwargs_t;

#define CREATE_ARRAY_TYPE(name) typedef struct \
  _##name##_array { \
  unsigned int n_##name; \
  name##_t **name##s; \
  } name##_array_t;

#define MEMBER_ARRAY(type) unsigned int max_##type; \
 unsigned int n_##type; \
 type##_t **type##s;

typedef struct _array {
  unsigned int n_items;
  unsigned int max_items;
  void **data;
} hm_array_t;

#define IMPL(name) int name##_impl(unsigned int n_bytes, \
  void *_inputs, void *_outputs, kwargs_t *kwargs)
#define IMPL_ARGS unsigned int n_bytes, void *_inputs, void *_outputs, kwargs_t *kwargs

#define SET_DEFAULT_ARG(arg, val, nullval) if (arg == nullval) { arg = val; }



bool is_list(hm_type_t *obj);
bool is_list_node(hm_type_t *obj);
int hm_list_insert(hm_list_t *list, hm_list_node_t *node, unsigned int index);
int hm_list_append(hm_list_t *list, hm_list_node_t *node);
int hm_list_itr(hm_list_t *list, list_node_fn *itr_fn);
int hm_list_remove(hm_list_t *list, unsigned int index);
int hm_list_delete(hm_list_t *list, list_node_fn *free_fn);

char *next_arg(char *const str, char d, char *d_opt, int *arglen, bool *optional);
int kwargs_unpack(kwargs_t *kwargs, char *fmt, ...);
#endif
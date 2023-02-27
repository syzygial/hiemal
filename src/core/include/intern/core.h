#ifndef _CORE_H
#define _CORE_H

#include "cmake_info.h"

#include "error.h"
#define UNIT_TESTING

#ifdef UNIT_TESTING
#define STATIC
#else
#define STATIC static
#endif

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

#endif
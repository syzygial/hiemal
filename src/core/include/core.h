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

#define IMPL(name) int name##_impl(unsigned int n_samples, \
  void *_inputs, void *_outputs, kwargs_t *kwargs)
#define IMPL_ARGS unsigned int n_samples, void *_inputs, void *_outputs, kwargs_t *kwargs

#endif
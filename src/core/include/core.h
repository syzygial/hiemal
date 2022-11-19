#ifndef _CORE_H
#define _CORE_H

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

#endif
#ifndef _UTIL_ARGS_H
#define _UTIL_ARGS_H

#include <stdbool.h>

typedef struct _arg hm_arg;

typedef int (arg_check_fn)(const char *arg);

enum arg_action {
  COLLECT=0,
  SET_TRUE,
  SET_FALSE,
  SEPARATE
};

struct _arg {
  char *name;
  char short_name;
  char *sig;
  //char **opts;
  enum arg_action action;
  void *result;
  arg_check_fn *arg_check;
};

#define HM_ARG_DEFAULT (char*)0x1

int args_init(hm_arg *args);
int args_clear(hm_arg *args);
int parse_args(hm_arg *args, int argc, char **argv);
int _parse_arg_sig(char *sig, int range[2]);

#endif
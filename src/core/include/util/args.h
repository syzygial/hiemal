#ifndef _UTIL_ARGS_H
#define _UTIL_ARGS_H

typedef struct _arg hm_arg;

enum arg_action {
  COLLECT=0,
  SET_TRUE,
  SET_FALSE
};

struct _arg {
  char *name;
  char short_name;
  char *sig;
  //char **opts;
  enum arg_action action;
  void *result;
};

#define HM_ARG_DEFAULT (char*)0x1;

int args_init(hm_arg *args);
int args_clear(hm_arg *args);
int parse_args(hm_arg *args, int argc, char **argv);

#endif
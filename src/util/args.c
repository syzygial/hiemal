#include "util/args.h"

#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef struct _res res;

struct _res {
  bool found;
  void **data;
};

int args_init(hm_arg *args) {
  while (args->name != NULL) {
    if (args->action == SET_TRUE) {
      args->result = malloc(sizeof(bool));
      *(bool*)(args->result) = false;
    }
    else if (args->action == SET_FALSE) {
      args->result = malloc(sizeof(bool));
      *(bool*)(args->result) = true;
    }
    args++;
  }
}

int args_result();

int args_clear(hm_arg *args) {
  while (args->name != NULL) {
    if (args->action == SET_TRUE || args->action == SET_FALSE) {
      free(args->result);
    }
    else if (args->action == COLLECT) {
      if (args->result != NULL) {
        char **collected_str = (char**)(args->result);
        while(*collected_str != NULL) {
          free(*collected_str);
          collected_str++;
        }
        free(args->result);
      }
    }
    args++;
  }
  return 0;
}

int _parse_arg_sig(char *sig, int range[2]) {
  if (strcmp(sig, "*") == 0) {
    range[0] = 1;
    range[1] = INT_MAX;
  }
  else if (strcmp(sig, "+") == 0) {
    range[0] = 2;
    range[1] = INT_MAX;
  }
  return 0;
}

int _parse_arg_seq(hm_arg *arg, int argc, char **argv) {
  int n_parsed = 1;
  if (arg->action == SET_TRUE) {
    if (arg->result == NULL) return -1;
    *(bool*)(arg->result) = true;
  }
  else if (arg->action == SET_FALSE) {
    if (arg->result == NULL) return -1;
    *(bool*)(arg->result) = false;
  }
  else if (arg->action == COLLECT) {
    int n_found = 0;
    char **_argv = argv;
    while (*argv != NULL) {
      // TODO: compare *argv against a list of argument switches
      // instead of just looking at the first character to see if the
      // string starts with '-'
      if ((*argv)[0] == '-') break;
      n_found++; n_parsed++; argv++;
    }
    argv = _argv;
    if (n_found > 0) {
      arg->result = calloc(n_found + 1, sizeof(char*));
      int i = 0;
      int len = 0;
      for (i = 0; i < n_found; i++) {
        len = strlen(argv[i]) + 1;
        ((char**)(arg->result))[i] = (char*)malloc(len * sizeof(char)); 
        strcpy(((char**)(arg->result))[i], argv[i]);
      }
    }
  }
  else {
    return -1;
  }
  return n_parsed;
}

int parse_args(hm_arg *args, int argc, char **argv) {
  hm_arg *_args = args;
  int n_parsed = 0;
  char short_opt[3] = "-\x00";
  char *long_opt = NULL;
  while (argc > 0) {
    while (args->name != NULL) {
      short_opt[1] = args->short_name;
      long_opt = (char*)calloc(2+strlen(args->name)+1,sizeof(char));
      strcpy(long_opt, "--");
      strcpy(long_opt+2, args->name);
      if ((strcmp(long_opt, *argv) == 0) ||
          (strcmp(short_opt, *argv) == 0)) {
            if ((n_parsed = _parse_arg_seq(args, argc-1, argv+1)) < 0) {
              return n_parsed;
            }
            argc -= n_parsed;
            argv += n_parsed;
          }
      free(long_opt);
      short_opt[1] = '\x00';
      args++;
    }
    args = _args;
  }
  return 0;
}

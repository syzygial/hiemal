#ifndef _UTIL_H
#define _UTIL_H

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct _test_info test_info_t;
typedef struct _testing_results testing_results_t;

struct _testing_results {
  unsigned int n_tests;
  struct _test_info *tests;
};

struct _test_info {
  char *test_name;
  enum {OK=0,FAILED,SKIPPED} result;
  double elapsed_time;
  bool run_test;
  bool completed;
};

int parse_test_list(testing_results_t *test_list, char *fmt);
int print_results_json(char *filename, const testing_results_t *results);
#endif
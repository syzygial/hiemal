#ifndef _UTIL_STR_H
#define _UTIL_STR_H

#include <stdbool.h>

bool is_int_str(char *s);
bool is_int_range(char *s);
int parse_int_range(char *s, int range[2]);
bool str_in_arr(char *s, char **arr);
int str_arr_len(char **s);
int str_arr_eq(char **arr1, char **arr2);
int str_arr_copy(char **dest, char **src);
int str_arr_copy_m(char ***dest, char **src);
int str_arr_copy_clear(char **s);
#endif
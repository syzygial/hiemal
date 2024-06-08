#ifndef _UTIL_STR_H
#define _UTIL_STR_H

#include <stdbool.h>

bool is_int_str(char *s);
bool is_int_range(char *s);
int parse_int_range(char *s, int range[2]);
bool str_in_arr(char *s, char **arr);

#endif
#include "util/str.h"

#include <stdlib.h>
#include <string.h>

bool is_int_str(char *s) {
  int i = 0;
  for  (i = 0; i < strlen(s); i++) {
    if ((s[i] < '0') || (s[i] > '9')) {
      return false;
    }
  }
  return true;
}

bool is_int_range(char *s) {
  char *_s = (char*)calloc(strlen(s)+1, sizeof(char));
  strcpy(_s, s);
  // results of strchr & strrchr
  char *_s1 = strchr(_s, '-');
  char *_s2 = strrchr(_s, '-');
  if ((_s1 == NULL) || (_s2 == NULL)) {
    free(_s);
    return false;
  }
  if (_s1 != _s2) {
    free(_s);
    return false;
  }
  
  *_s1 = '\0';
  if ((!is_int_str(_s)) || (!is_int_str(_s1+1))) {
    free(_s);
    return false;
  }
  int i1 = atoi(_s);
  int i2 = atoi(_s1+1);
  if (i1 > i2) {
    free(_s);
    return false;
  }
  return true;
}

int parse_int_range(char *s, int range[2]) {
  if (!is_int_range(s)) {
    return -1;
  }
  char *_s = (char*)calloc(strlen(s)+1, sizeof(char));
  strcpy(_s, s);
  char *_s1 = strchr(_s, '-');
  *_s1 = '\0';
  range[0] = atoi(_s);
  range[1] = atoi(_s1+1);
  free(_s);
  return 0;
}

bool str_in_arr(char *s, char **arr) {
  char **arr_itr = arr;
  while (*arr_itr != NULL) {
    if (strcmp(s, *arr_itr) == 0) {
      return true;
    }
    arr_itr++;
  }
  return false;
}
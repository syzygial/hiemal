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

int str_arr_len(char **s) {
  if (s == NULL) {
    return -1;
  }
  int len = 0;
  while (*s != NULL) {
    len++;
    s++;
  }
  return len;
}

int str_arr_eq(char **arr1, char **arr2) {
  int arr1_len = str_arr_len(arr1);
  int arr2_len = str_arr_len(arr2);
  if (arr1_len != arr2_len) {
    return 0;
  }
  
  int i = 0;
  for (i = 0; i < arr1_len; i++) {
    if (strcmp(arr1[i], arr2[i]) != 0) {
      return 0;
    }
  }
  return 1;
}

int str_arr_copy(char **dest, char **src) {
  int src_len = str_arr_len(src);
  int str_len = 0;
  int i = 0;

  for (i = 0; i < src_len; i++) {
    str_len = strlen(src[i]);
    dest[i] = (char*)calloc(str_len+1, sizeof(char));
    strcpy(dest[i], src[i]);
  }
  return 0;
}

int str_arr_copy_m(char ***dest, char **src) {
  int src_len = str_arr_len(src);
  *dest = (char**)calloc(src_len+1, sizeof(char*));
  return str_arr_copy(*dest, src);
}

int str_arr_copy_clear(char **s) {
  int str_len = str_arr_len(s);
  int i = 0;
  for (i = 0; i < str_len; i++) {
    free(s[i]);
  }
  return 0;
}
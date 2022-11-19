#include "intern/math.h"

double_t ddot(char mode, void *a, void *b, unsigned int len) {
  int i;
  double_t res = 0;
  if (mode == 'f') {
     for (i = 0; i < len; i++) {
      res += (((double_t*)a)[i] * ((double_t*)b)[len - (i+1)]);
    }   
  } else {
    for (i = 0; i < len; i++) {
      res += (((double_t*)a)[i] * ((double_t*)b)[i]);
    }
  }
  return res;
}
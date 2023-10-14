#ifndef _INTERN_MATH_H
#define _INTERN_MATH_H

#include <math.h>
#include <stdint.h>

double_t ddot(char mode, void *a, void *b, unsigned int len);
uint64_t u64sum(uint64_t *buf, unsigned int len);
#endif
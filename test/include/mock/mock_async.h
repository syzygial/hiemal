#ifndef _MOCK_ASYNC_H
#define _MOCK_ASYNC_H

#include <math.h>

#include "test_async.h"
#include "buffer.h"

int times_two(double_t *src, double_t *dest, unsigned int n_samples);
async_args_t times_two_async(buffer_t *src, buffer_t *dest);

#endif

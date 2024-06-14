#ifndef _INTERN_DSP_H
#define _INTERN_DSP_H

#include <math.h>

#include "buffer.h"
#include "common.h"
int _buf_shift_down_64(void *buf, const unsigned int n_bytes, 
    const unsigned int n_shift);

int _buf_shift_up_64(void *buf, const unsigned int n_bytes, 
    const unsigned int n_shift);

int dct_2_64f(buffer_t *src, void *dest, unsigned int n_samples);

IMPL(src_sin);
#endif
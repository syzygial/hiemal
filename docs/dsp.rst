.. highlight:: C

DSP 
====

DSP operations can be called as standalone functions.

FIR Filter
-----------

::

  #define N_SAMPLES ...
  #define B_ORDER ...
  double src[N_SAMPLES], dest[N_SAMPLES];
  double b[] = {...}; // FIR coefficients
  fir_filter(src, dest, b, B_ORDER, N_SAMPLES);
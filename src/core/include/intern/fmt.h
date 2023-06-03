#ifndef _INTERN_FMT_H
#define _INTERN_FMT_H

#include <stdint.h>

typedef struct _chunk {
  unsigned int n_samples;
  unsigned int n_bytes;
  unsigned int n_channels;
  unsigned int bits_per_sample;
  unsigned int fs;
  void *data;
  struct _chunk *next;
} chunk_t;

typedef struct _chunkList {
  unsigned int n_chunks;
  chunk_t *head;
} chunkList_t;

typedef enum _combine_chunks_opts {
  COPY = 0,
  MOVE
} combine_chunks_opts_t;

typedef enum _byte_order {
  INTERLEAVED_16S = 0
} byte_order_t;

typedef struct _audio_opts {
  unsigned int fs;
  byte_order_t byte_order;
} read_raw_opts_t, audio_params_t;

typedef enum _file_processing_opts {
  // follow the spec 100%, return an error if anything is wrong
  STRICT = 0,
  // try to extract audio data from file, ignore format issues/noncompliance
  PERMISSIVE,
  // same as permissive but keep a running list of errors
  DIAGNOSTIC
} file_processing_opts_t;

typedef struct _wav_params {
  unsigned int file_offset;
  unsigned int n_bytes;
  unsigned int n_channels;
  unsigned int bits_per_sample;
  unsigned int fs;
} wav_params_t;

extern const read_raw_opts_t RAW_AUDIO_DEFAULTS;

chunk_t *combineChunks(chunk_t *head, unsigned int n_chunks, \
  combine_chunks_opts_t opts);
int processWAVHeader(char *filename, wav_params_t *params, file_processing_opts_t opts);
int readWAV(char *filename, chunk_t *data, file_processing_opts_t opts);

#endif
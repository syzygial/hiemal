#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>

#include "intern/fmt.h"
#include "intern/error.h"

const read_raw_opts_t RAW_AUDIO_DEFAULTS = {44100, INTERLEAVED_16S};

chunk_t *readRaw(char *filename, read_raw_opts_t opts) {
  struct stat file_stat;
  stat(filename, &file_stat);
  int size = file_stat.st_size;
  FILE *fp = fopen(filename, "r");
  char *data = malloc(size);
  fread(data, size, 1, fp);
  fclose(fp);
  
  chunk_t *new_chunk = (chunk_t*)malloc(sizeof(chunk_t));
  new_chunk->n_samples = size / 4;
  new_chunk->fs = opts.fs;
  new_chunk->data = data;
  return new_chunk;
}

static char* wave_fmt_tag(uint16_t code) {
  switch (code) {
    case 0x0001:
      return "WAVE_FORMAT_PCM (0x0001): Microsoft PCM format";
    case 0x0101:
      return "IBM_FORMAT_MULAW (0x0101): IBM mu-law format";
    case 0x0102:
      return "IBM_FORMAT_ALAW (0x0102): IBM a-law format";
    case 0x0103:
      return "IBM_FORMAT_AD_CM (0x0103): IBM VAC Adaptive Differential "
        "Pulse Code Modulation format";
    default:
      return "Unknown format";
  }
}

int processWAVHeader(char *filename, wav_params_t *params, file_processing_opts_t opts) {
  const int RIFF_OFFSET = 8;
  FILE *fp = fopen(filename, "r");
  char header_string_buff[5] = {'\0'};
  fread(header_string_buff, 4, 1, fp);
  // check the WAVE magic string
  if(opts == STRICT) {
    if (strcmp(header_string_buff, "RIFF") == 0) {
      // do nothing
    }
    else if (strcmp(header_string_buff, "RIFX") == 0) {
      fclose(fp);
      strcpy(AL_errtxt, "Big-endian WAV not supported\n");
      return MISSING_IMPL;
    }
    else {
      fclose(fp);
      strcpy(AL_errtxt, "Bad RIFF tag\n");
      return BAD_HEADER;
    }
  }
  fread(header_string_buff, 4, 1, fp);
  if (opts == STRICT) {
    struct stat file_stat;
    uint32_t n_wav_bytes = *(uint32_t*)header_string_buff;
    stat(filename, &file_stat);
    if(file_stat.st_size != n_wav_bytes + RIFF_OFFSET) {
      fclose(fp);
      strcpy(AL_errtxt, "Incorrect RIFF chunk size\n");
      return BAD_HEADER;
    }
  }
  fread(header_string_buff, 4, 1, fp);
  if (opts == STRICT) {
    if (strcmp(header_string_buff, "WAVE") != 0) {
      fclose(fp);
      strcpy(AL_errtxt, "Bad WAV tag\n");
      return BAD_HEADER;
    }
  }
  fread(header_string_buff, 4, 1, fp);
  if (opts == STRICT) {
    if (strcmp(header_string_buff, "fmt ") != 0) {
      fclose(fp);
      strcpy(AL_errtxt, "fmt tag missing\n");
      return BAD_HEADER;
    }
  }

  // fmt chunk size
  fread(header_string_buff, 4, 1, fp);

  //wFormatTag
  fread(header_string_buff, 2, 1, fp);
  if (*(uint16_t*)header_string_buff != 0x0001) {
    fclose(fp);
    char err_msg[ERRTXT_LEN];
    strcpy(err_msg, wave_fmt_tag(*(uint16_t*)header_string_buff));
    strcpy(AL_errtxt, strcat(err_msg, " not supported\n"));
    return MISSING_IMPL;
  }
  //wChannels
  fread(header_string_buff, 2, 1, fp);
  if (*(uint16_t*)header_string_buff > 2) {
    fclose(fp);
    strcpy(AL_errtxt, "More than 2 audio channels not supported\n");
    return MISSING_IMPL;
  }
  params->n_channels = *(uint16_t*)header_string_buff;
  //dwSamplesPerSec
  fread(header_string_buff, 4, 1, fp);
  params->fs = *(uint32_t*)header_string_buff;
  //dwAvgBytesPerSec
  fread(header_string_buff, 4, 1, fp);

  //wBlockalign
  fread(header_string_buff, 2, 1, fp);

  //wBitsPerSample
  fread(header_string_buff, 2, 1, fp);
  if (*(uint16_t*)header_string_buff != 16) {
    fclose(fp);
    strcpy(AL_errtxt, "Only 16-big signed PCM supported\n");
    return MISSING_IMPL;
  }
  params->bits_per_sample = *(uint16_t*)header_string_buff;
  //next chunk
  fread(header_string_buff, 4, 1, fp);
  if (strcmp(header_string_buff, "data") != 0) {
    fclose(fp);
    strcpy(AL_errtxt, "Optional chunks not supported\n");
    return MISSING_IMPL;
  }
  // wav data (bytes)
  fread(header_string_buff, 4, 1, fp);
  params->file_offset = ftell(fp);
  params->n_bytes = *(uint32_t*)header_string_buff;
  fclose(fp);
  return 0;
}

int readWAV(char *filename, chunk_t *data, file_processing_opts_t opts) {
  wav_params_t params;
  processWAVHeader(filename, &params, opts);
  char *wav_data = (char*)malloc(params.n_bytes);
  FILE *fp = fopen(filename, "r");
  fseek(fp, params.file_offset, SEEK_SET);
  fread(wav_data, params.n_bytes, 1, fp);
  fclose(fp);
  data->n_samples = (params.n_bytes * 8) / params.bits_per_sample;
  data->n_bytes = params.n_bytes;
  data->n_channels = params.n_channels;
  data->bits_per_sample = params.bits_per_sample;
  data->fs = params.fs;
  data->data = wav_data;
  data->next = NULL;
  return 0;
}
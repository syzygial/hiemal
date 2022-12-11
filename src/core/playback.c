#include "intern/playback.h"

#ifdef __linux__
void playChunkALSA(chunk_t *chunk) {
  #ifdef WITH_ALSA
  snd_pcm_t *handle;
  snd_pcm_uframes_t frames;
  int e;
  if ((e = snd_pcm_open(&handle, "default", SND_PCM_STREAM_PLAYBACK, 0)) < 0) {
    fprintf(stderr, "pcm open error: %s\n", snd_strerror(e));
    exit(EXIT_FAILURE);
  }

  if ((e = snd_pcm_set_params(handle, SND_PCM_FORMAT_S16_LE, \
      SND_PCM_ACCESS_RW_INTERLEAVED, chunk->n_channels, chunk->fs, 0, 500000)) < 0) {
    fprintf(stderr, "param set error: %s\n", snd_strerror(e));
  }
  
  char* read_ptr = (char*)(chunk->data);
  unsigned int buf_samples = 10000;
  unsigned n_writes = chunk->n_bytes / buf_samples;
  int i;
  for (i = 0; i < n_writes; i++) {
    frames = snd_pcm_writei(handle, read_ptr, buf_samples / 4);
    if (frames < 0) {
      frames = snd_pcm_recover(handle, frames, 0);
    }
    if (frames < 0) {
      fprintf(stderr, "write error: %s\n", snd_strerror(frames));
      exit(EXIT_FAILURE);
    }
    read_ptr += buf_samples;
  }
  if ((e = snd_pcm_drain(handle)) < 0) {
    fprintf(stderr, "drain error: %s\n", snd_strerror(e));
    exit(EXIT_FAILURE);
  }
  snd_pcm_close(handle);
  return;
  #else
  return;
  #endif // #ifdef WITH_ALSA
}
#endif // #ifdef __linux__
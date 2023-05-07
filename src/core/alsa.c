#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "intern/alsa.h"
#include "intern/buffer.h"
#include "intern/common.h"

int hm_alsa_n_cards() {
  int n_cards = -1;
  int card_index = -1;
  do {
    n_cards++;
    snd_card_next(&card_index);
  } while(card_index != -1);
  return n_cards;
}

int hm_alsa_n_pcm(int card) {
  int n_pcm = 0;
  void **hints = NULL;
  void **cur_hint = NULL;
  char *hint_result_name = NULL;
  char *hint_result_ioid = NULL;
  snd_device_name_hint(card, "pcm", &hints);
  if (!hints) {
    return 0;
  }
  cur_hint = hints;
  while(*cur_hint != NULL) {
    hint_result_name = snd_device_name_get_hint(*cur_hint, "NAME");
    hint_result_ioid = snd_device_name_get_hint(*cur_hint, "IOID");
    if (strncmp(hint_result_name, "hw", 2) == 0) {
      if (hint_result_ioid == NULL) {
        n_pcm += 2;
      }
      else {
        n_pcm++;
      }
    }
    free(hint_result_name);
    free(hint_result_ioid);
    cur_hint++;
  }
  snd_device_name_free_hint(hints);
  return n_pcm;
}

int hm_alsa_device_io_init(hm_device_io_t **io, hm_device_t *device, int index) {
  snd_ctl_t *alsa_card = (snd_ctl_t*)device->backend_handle;
  snd_pcm_t *alsa_pcm;
  *io = (hm_device_io_t*)malloc(sizeof(hm_device_io_t));
  (*io)->dev_backend = ALSA;
  return 0;
}

int hm_alsa_device_io_delete(hm_device_io_t **io) {
  snd_pcm_t *pcm = (*io)->backend_handle;
  snd_pcm_drain(pcm);
  snd_pcm_close(pcm);
  //free((*io)->name);
  free(*io);
  *io = NULL;
  return 0;
}

int hm_alsa_device_init(hm_device_t **dev, int card_index) {
  char *name = NULL;
  snd_card_get_name(card_index, &name);
  snd_ctl_t *alsa_card = NULL;
  snd_ctl_open(&alsa_card, name, 0);

  *dev = (hm_device_t*)malloc(sizeof(hm_device_t));
  (*dev)->name = name;
  (*dev)->io_backend = ALSA;
  (*dev)->backend_handle = (void*)alsa_card;

  int n_pcm = hm_alsa_n_pcm(card_index);
  (*dev)->io_devices = (hm_device_io_t**)malloc(card_index*sizeof(hm_device_io_t*));
  int i = 0;
  hm_device_io_t **pcm = NULL;
  for (i = 0; i < n_pcm; i++) {
    pcm = &(((*dev)->io_devices)[i]);
    hm_alsa_device_io_init(pcm, *dev, i);
  }
  return 0;
}

int hm_alsa_default_io_init(hm_device_io_t **io, hm_io_type_t io_type) {
  snd_pcm_stream_t alsa_io_type = (io_type == RECORDING) ? 
    SND_PCM_STREAM_CAPTURE : SND_PCM_STREAM_PLAYBACK;
  snd_pcm_t *default_pcm = NULL;
  snd_pcm_open(&default_pcm, "default", alsa_io_type, 0);
  *io = (hm_device_io_t*)malloc(sizeof(hm_device_io_t));
  (*io)->name = strdup("default");
  (*io)->device = NULL;
  (*io)->dev_backend = ALSA;
  (*io)->backend_handle = (void*)default_pcm;
  return 0;
}

int hm_alsa_io_write(hm_device_io_t *io, buffer_t *buf, unsigned int n_bytes) {
  // TODO: do samples-to-bytes conversion based on # of channels/bit depth
  unsigned int n_samples = n_bytes / 4;
  snd_pcm_t *pcm = (snd_pcm_t*)(io->backend_handle);
  void *linear_buf = (void*)malloc(n_bytes);
  buffer_read(buf, linear_buf, n_bytes);
  //snd_pcm_writei(pcm, linear_buf);
  free(linear_buf);
  return 0;
}

int hm_alsa_io_read(hm_device_io_t *io, buffer_t *buf, unsigned int n_bytes) {
  unsigned int n_samples = n_bytes / 4;
  snd_pcm_t *pcm = (snd_pcm_t*)(io->backend_handle);
  void *linear_buf = (void*)malloc(n_bytes);
  //snd_pcm_readi(pcm, linear_buf);
  buffer_write(buf, linear_buf, n_bytes);
  free(linear_buf);
  return 0;
}
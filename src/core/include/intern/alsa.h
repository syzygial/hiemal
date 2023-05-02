#ifndef _HM_ALSA_H
#define _HM_ALSA_H

#include <alsa/asoundlib.h>
#include "intern/device.h"


int hm_alsa_n_cards();
int hm_alsa_n_pcm(int card);
int hm_alsa_default_io_init(hm_device_io_t **io, hm_io_type_t io_type);

int hm_alsa_device_io_delete(hm_device_io_t **io);

#endif
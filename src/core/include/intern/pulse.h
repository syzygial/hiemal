#ifndef _PULSE_H
#define _PULSE_H

#include <pulse/pulseaudio.h>

#include "intern/backend.h"
#include "intern/device.h"

int hm_pulse_connection_init(hm_backend_connection_t **pulse_backend);
int hm_pulse_connection_close(hm_backend_connection_t **pulse_backend);

int hm_pulse_n_cards(hm_backend_connection_t *pulse_backend);

#endif
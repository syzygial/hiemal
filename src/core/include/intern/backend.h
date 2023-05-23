#ifndef _BACKEND_H
#define _BACKEND_H

typedef enum {ALSA=0, PULSEAUDIO} hm_backend_t;

typedef struct _hm_backend_connection hm_backend_connection_t;
typedef struct _hm_device hm_device_t;
struct _hm_backend_connection {
  hm_backend_t backend_type;
  int n_devices;
  hm_device_t **devices;
  void *backend_handle;
};

#endif
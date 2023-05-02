#ifndef _BACKEND_H
#define _BACKEND_H

typedef enum {ALSA=0, PULSEAUDIO} hm_backend_t;

typedef struct _hm_backend_connection hm_backend_connection_t;

struct _hm_backend_connection {
  hm_backend_t backend_type;
  void *backend_handle;
};

#endif
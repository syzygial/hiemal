#ifndef _DSP_H
#define _DSP_H

#include "intern/backend.h"

typedef struct _hm_device hm_device_t;
typedef struct _hm_device_io hm_device_io_t;

typedef enum {RECORDING=0, PLAYBACK} hm_io_type_t;

struct _hm_device {
  char *name;
  hm_backend_t io_backend;
  void *backend_handle;
  hm_device_io_t **io_devices;
};

struct _hm_device_io {
  char *name;
  hm_device_t *device;
  hm_backend_t dev_backend;
  void *backend_handle;
};

#endif

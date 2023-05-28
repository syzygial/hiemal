#include "intern/device.h"

int get_device_by_id(hm_device_t **dev, hm_backend_connection_t *backend, unsigned int id) {
  return 0;
}

int get_device_by_name(hm_device_t **dev, hm_backend_connection_t *backend, char *name) {
  return 0;
}

int get_device_io_by_id(hm_backend_connection_t *backend, hm_device_io_t **io, unsigned int card_id, unsigned int id) {
  return 0;
}

int get_device_io_by_name(hm_backend_connection_t *backend, hm_device_io_t **io, unsigned int card_id, char *name) {
  return 0;
}

int default_device_io_connect(hm_device_io_connection_t **io, hm_backend_connection_t *backend, hm_io_type_t dir) {
  const struct backend_info *info = get_backend_by_name(backend->backend_name);
  unsigned int io_id = (dir == PLAYBACK) ? backend->default_dev_io[0] : backend->default_dev_io[1];
  return (info->io_connect_fn)(io, backend, io_id);
}
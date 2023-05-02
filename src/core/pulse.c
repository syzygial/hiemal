#include "intern/buffer.h"
#include "intern/pulse.h"

#include <unistd.h>

typedef struct {
  pa_threaded_mainloop *mainloop;
  pa_context *hm_context;
} hm_pulse_handle_t;

void _hm_pulse_context_state_cb(pa_context *c, void *userdata) {
  return;
}

void _hm_pulse_card_list_cb (pa_context* c, const pa_card_info* i, int eol, void* userdata) {
  if (eol > 0) {
    return;
  }
  hm_backend_connection_t *hm_pulse_backend = (hm_backend_connection_t*)userdata;
}

void _hm_pulse_n_cards_cb (pa_context* c, const pa_card_info* i, int eol, void* userdata) {
  if (eol > 0) {
    return;
  }
  int *ii = (int*)userdata;
  (*ii)++;
}

int hm_pulse_n_cards(hm_backend_connection_t *pulse_backend) {
  int n_cards = 0;
  hm_pulse_handle_t *pulse_handle = (hm_pulse_handle_t*)(pulse_backend->backend_handle);
  pa_context *hm_pa_context = (pa_context*)(pulse_handle->hm_context);
  pa_operation *o = 
    pa_context_get_card_info_list(hm_pa_context, _hm_pulse_n_cards_cb, (void*)(&n_cards));
  while(pa_operation_get_state(o) != PA_OPERATION_DONE);
  return n_cards;
}

int hm_pulse_connection_init(hm_backend_connection_t **pulse_backend) {
  pa_threaded_mainloop *m = pa_threaded_mainloop_new();
  pa_mainloop_api *m_api = pa_threaded_mainloop_get_api(m);
  pa_context *hm_pa_context = pa_context_new(m_api, "hiemal");
  pa_threaded_mainloop_start(m);
  pa_context_set_state_callback(hm_pa_context, _hm_pulse_context_state_cb, NULL);
  pa_context_connect(hm_pa_context, NULL, 0, NULL);
  while(pa_context_get_state(hm_pa_context) != PA_CONTEXT_READY);
  *pulse_backend = (hm_backend_connection_t*)malloc(sizeof(hm_backend_connection_t));
  hm_pulse_handle_t *pulse_handle = 
    (hm_pulse_handle_t*)malloc(sizeof(hm_pulse_handle_t));
  pulse_handle->mainloop = m;
  pulse_handle->hm_context = hm_pa_context;
  (*pulse_backend)->backend_type = PULSEAUDIO;
  (*pulse_backend)->backend_handle = (void*)pulse_handle;
  /*
  pa_operation *o = 
    pa_context_get_card_info_list(hm_pa_context, _hm_pulse_card_list_cb, (void*)(*pulse_backend));
  while(pa_operation_get_state(o) != PA_OPERATION_DONE);
  */
  return 0;
}

int hm_pulse_connection_close(hm_backend_connection_t **pulse_backend) {
  hm_pulse_handle_t *h = (hm_pulse_handle_t*)((*pulse_backend)->backend_handle);
  pa_context *hm_pa_context = (pa_context*)(h->hm_context);
  pa_threaded_mainloop *hm_mainloop = (pa_threaded_mainloop*)(h->mainloop);

  pa_threaded_mainloop_stop(hm_mainloop);
  pa_context_disconnect(hm_pa_context);
  pa_context_unref(hm_pa_context);
  pa_threaded_mainloop_free(hm_mainloop);
  free(*pulse_backend);
  *pulse_backend = NULL;
  return 0;
}

int hm_pulse_device_init(hm_device_t **dev, int card_index) {
  return 0;
}

int hm_pulse_device_io_init(hm_device_io_t **io, hm_device_t *device, int index) {
  return 0;
}

int hm_pulse_io_write(hm_device_t *io, buffer_t *buf, unsigned int n_bytes) {
  return 0;
}

int hm_pulse_io_read(hm_device_t *io, buffer_t *buf, unsigned int n_bytes) {
  return 0;
}
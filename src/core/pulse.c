#include "intern/buffer.h"
#include "intern/pulse.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>

typedef struct {
  pa_threaded_mainloop *mainloop;
  pa_context *hm_context;
} hm_pulse_handle_t;

typedef struct {
  hm_device_t **dev;
  int dev_i;
} indexed_dev_handle;

void _hm_pulse_context_state_cb(pa_context *c, void *userdata) {
  return;
}

void _hm_pulse_card_list_cb (pa_context *c, const pa_card_info *i, int eol, void *userdata) {
  if (eol > 0) {
    return;
  }
  indexed_dev_handle *indexed_dev = (indexed_dev_handle*)userdata;
  hm_device_t **dev = indexed_dev->dev + indexed_dev->dev_i;
  *dev = (hm_device_t*)malloc(sizeof(hm_device_t));
  (*dev)->name = strdup(i->name);
  (*dev)->io_backend = PULSEAUDIO;
  int n_src = i->active_profile2->n_sources;
  int n_sink = i->active_profile2->n_sinks;
  int n_streams = n_src + n_sink;
  (*dev)->n_io_devices = n_streams;
  (*dev)->io_devices = (hm_device_io_t**)calloc(n_streams,sizeof(hm_device_io_t*));
  (indexed_dev->dev_i)++;
}

void _hm_pulse_source_list_cb (pa_context *c, const pa_source_info *i, int eol, void *userdata) {
  if (eol > 0) {
    return;
  }
  hm_backend_connection_t *pulse_backend = (hm_backend_connection_t*)userdata;
  hm_device_io_t **io_dev_itr = (pulse_backend->devices[i->card])->io_devices;
  int ii = 0;
  int n_io_dev = (pulse_backend->devices[i->card])->n_io_devices;
  while(*(io_dev_itr++) != NULL);
  *io_dev_itr = (hm_device_io_t*)malloc(sizeof(hm_device_io_t));
  (*io_dev_itr)->name = strdup(i->name);
}

void _hm_pulse_sink_list_cb (pa_context *c, const pa_sink_info *i, int eol, void *userdata) {
  if (eol > 0) {
    return;
  }
  hm_backend_connection_t *pulse_backend = (hm_backend_connection_t*)userdata;
  hm_device_io_t **io_dev_itr = (pulse_backend->devices[i->card])->io_devices;
  int ii = 0;
  int n_io_dev = (pulse_backend->devices[i->card])->n_io_devices;
  while(*(io_dev_itr++) != NULL);
  *io_dev_itr = (hm_device_io_t*)malloc(sizeof(hm_device_io_t));
  (*io_dev_itr)->name = strdup(i->name);
}

void _hm_pulse_n_cards_cb (pa_context *c, const pa_card_info *i, int eol, void *userdata) {
  if (eol > 0) {
    return;
  }
  int *ii = (int*)userdata;
  (*ii)++;
}

int hm_pulse_io_write(hm_device_io_t *io, buffer_t *buf, unsigned int n_bytes) {
  pa_stream *s = (pa_stream*)(io->backend_handle);
  void *pa_buf = malloc(n_bytes);
  buffer_read(buf, pa_buf, n_bytes);
  pa_stream_write(s, buf, n_bytes, NULL, 0, PA_SEEK_RELATIVE);
  free(pa_buf);
  return 0;
}

int hm_pulse_io_read(hm_device_io_t *io, buffer_t *buf, unsigned int n_bytes)  {
  return 0;
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
  int n_cards = hm_pulse_n_cards(*pulse_backend);
  (*pulse_backend)->n_devices = n_cards;
  (*pulse_backend)->devices = (hm_device_t**)malloc(n_cards*sizeof(hm_device_t*));
  indexed_dev_handle dev_cb_handle;
  dev_cb_handle.dev = (*pulse_backend)->devices;
  dev_cb_handle.dev_i = 0;
  pa_operation *o = 
    pa_context_get_card_info_list(hm_pa_context, _hm_pulse_card_list_cb, (void*)(&dev_cb_handle));
  while(pa_operation_get_state(o) != PA_OPERATION_DONE);
  o = pa_context_get_source_info_list(hm_pa_context, _hm_pulse_source_list_cb,
    (void*)(*pulse_backend));
  while(pa_operation_get_state(o) != PA_OPERATION_DONE);
  o = pa_context_get_sink_info_list(hm_pa_context, _hm_pulse_sink_list_cb,
    (void*)(*pulse_backend));
  while(pa_operation_get_state(o) != PA_OPERATION_DONE);
  return 0;
}

int hm_pulse_default_io_connect(hm_device_io_connection_t **io, 
  hm_backend_connection_t *pulse_backend, hm_io_type_t dir) {
  hm_pulse_handle_t *pulse_handle = (hm_pulse_handle_t*)(pulse_backend->backend_handle);
  pa_context *c = pulse_handle->hm_context;
  pa_sample_spec fmt;
  fmt.format = PA_SAMPLE_S16LE;
  fmt.rate = 44100;
  fmt.channels = 2;
  pa_stream *new_stream = pa_stream_new(c, "hiemal", &fmt, NULL);
  switch (dir) {
    case PLAYBACK:
      pa_stream_connect_playback(new_stream, "default", NULL, 0, NULL, NULL);
    case RECORDING:
      pa_stream_connect_record(new_stream, "default", NULL, 0);
  }
  *io = (hm_device_io_connection_t*)malloc(sizeof(hm_device_io_connection_t));
  (*io)->backend_handle = (void*)new_stream;
  (*io)->type = dir;
  (*io)->read_fn = hm_pulse_io_read;
  (*io)->write_fn = hm_pulse_io_write;
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
  
  int i = 0, j = 0;
  hm_device_t *cur_device = NULL;
  for (i = 0; i < ((*pulse_backend)->n_devices); i++) {
    cur_device = ((*pulse_backend)->devices)[i];
    for (j = 0; j < cur_device->n_io_devices; j++) {
      free((cur_device->io_devices)[j]);
    }
    free(cur_device->io_devices);
    free(cur_device);
  }
  free((*pulse_backend)->devices);
  free(*pulse_backend);
  *pulse_backend = NULL;
  return 0;
}
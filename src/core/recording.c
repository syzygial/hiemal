#include <stdlib.h>

#include "intern/core.h"
#include "intern/buffer.h"
#include "intern/recording.h"

#define MAX_RECORDING_BUFFERS 10

//CREATE_ARRAY_TYPE(buffer)
//CREATE_ARRAY_TYPE(recording)


int hm_recording_init(recording_t **r, char *filename) {
  *r = (recording_t*)malloc(sizeof(recording_t));
  (*r)->filename = filename;
  (*r)->max_buffer = MAX_RECORDING_BUFFERS;
  (*r)->n_buffer = 0;
  (*r)->buffers = (buffer_t**) \
    malloc(MAX_RECORDING_BUFFERS * sizeof(buffer_t*));
  _hm_pb_buflist_create(&((*r)->buflist));
  (*r)->active = true;
  return 0;
}

int hm_recording_n_buffers(recording_t *r) {
  return r->n_buffer;
}

int hm_recording_delete(recording_t **r) {
  unsigned int i;
  for (i = 0; i < (*r)->n_buffer; i++) {
    buffer_delete(&(((*r)->buffers)[i]));
  }
  _hm_pb_buflist_create(&((*r)->buflist));
  free(*r);
  *r = NULL;
  return 0;
}

int hm_recording_start(recording_t *r) {
  r->active = true;
  return 0;
}

int hm_recording_stop(recording_t *r) {
  r->active = false;
  return 0;
}


int hm_recording_buffer_add(recording_t *r, buffer_t *buf) {
  buffer_t **buf_pos = &(r->buffers[r->n_buffer]);
  *buf_pos = buf;
  buffer_add_recording(buf, r);
  _hm_pb_buflist_add_buffer(r->buflist);
  (r->n_buffer)++;
  return 0;
}
/*
int hm_recording_buffer_remove(recording_t *r, buffer_t *buf) {
  return 0;
}
*/
int hm_recording_write(recording_t *r, unsigned int r_id, const void *data, unsigned int n_bytes) {
  if (!(r->active)) {
    return -1;
  }
  return _hm_pb_buffer_write(r->buflist, r_id, data, n_bytes);
}

int hm_recording_save(recording_t *r) {
  return _hm_pb_buflist_save(r->buflist, r->filename);
}

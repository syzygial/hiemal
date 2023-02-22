#include <stdlib.h>

#include "intern/core.h"
#include "intern/buffer.h"
#include "intern/recording.h"

#define MAX_RECORDING_BUFFERS 10

CREATE_ARRAY_TYPE(buffer)
CREATE_ARRAY_TYPE(recording)


int hm_recording_init(recording_t **r, buffer_t *buf, char *filename) {
  *r = (recording_t*)malloc(sizeof(recording_t));
  (*r)->filename = filename;
  //(*r)->buf_list.n_buffer = MAX_RECORDING_BUFFERS;
  //(*r)->buf_list.buffers = (buffer_t**) \
  //  malloc(MAX_RECORDING_BUFFERS * sizeof(buffer_t*));
  (*r)->buf = buf;
  buffer_add_recording(buf, *r);
  _hm_pb_buffer_create(&((*r)->buf_pb_msg));
  (*r)->active = true;
  return 0;
}

int hm_recording_delete(recording_t **r) {
  _hm_pb_buffer_delete(&((*r)->buf_pb_msg));
  buffer_delete(&((*r)->buf));
  free(*r);
  *r = NULL;
  return 0;
}

int hm_recording_start(recording_t *r) {
  r->active = false;
  return 0;
}

int hm_recording_stop(recording_t *r) {
  r->active = true;
  return 0;
}

/*
int hm_recording_buffer_add(recording_t *r, buffer_t *buf) {
  return 0;
}

int hm_recording_buffer_remove(recording_t *r, buffer_t *buf) {
  return 0;
}
*/
int hm_recording_write(recording_t *r, const void *data, unsigned int n_bytes) {
  if (!(r->active)) {
    return -1;
  }
  return _hm_pb_buffer_write(r->buf_pb_msg, data, n_bytes);
}

int hm_recording_save(recording_t *r) {
  return _hm_pb_buffer_save(r->buf_pb_msg, r->filename);
}
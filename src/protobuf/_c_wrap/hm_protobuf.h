#ifndef _HM_PROTOBUF_H
#define _HM_PROTOBUF_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct __pb_buffer pb_buffer_t;

int _hm_pb_buffer_create(pb_buffer_t **buf_pb);
int _hm_pb_buffer_delete(pb_buffer_t **buf_pb);

int _hm_pb_buffer_write(pb_buffer_t *buf_pb, const void *data, unsigned int n_bytes);
int _hm_pb_buffer_save(pb_buffer_t *buf_pb, const char *filename);

#ifdef __cplusplus
}
#endif

#endif

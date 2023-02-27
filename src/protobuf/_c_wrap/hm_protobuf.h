#ifndef _HM_PROTOBUF_H
#define _HM_PROTOBUF_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct __pb_buflist pb_buflist_t;
typedef struct __pb_buffer pb_buffer_t;

int _hm_pb_buflist_create(pb_buflist_t **buflist_pb);
int _hm_pb_buflist_delete(pb_buflist_t **buflist_pb);

int _hm_pb_buflist_add_buffer(pb_buflist_t *buflist_pb);
int _hm_pb_buffer_write(pb_buflist_t *buflist_pb, unsigned int r_id, \
    const void *data, unsigned int n_bytes);
int _hm_pb_buflist_save(pb_buflist_t *buflist_pb, const char *filename);

#ifdef __cplusplus
}
#endif

#endif

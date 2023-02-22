#ifndef _RECORDING_H
#define _RECORDING_H


typedef struct _buffer buffer_t;
typedef struct _recording recording_t;

int hm_recording_init(recording_t **r, buffer_t *buf, char *filename);
int hm_recording_delete(recording_t **r);
int hm_recording_write(recording_t *r, const void *data, unsigned int n_bytes);
int hm_recording_save(recording_t *r);

#endif
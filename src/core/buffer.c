/// \file buffer.c

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

#include "api/recording.h"
#include "intern/core.h"
#include "intern/buffer.h"

#include "cmake_info.h"

/*! \struct _buffer buffer.c "core/buffer.h"
*   \brief buffer struct
*   \var _buffer::buf
*   pointer to raw buffer
*   \var _buffer::buf_len_bytes
*   number of bytes allocated for buffer
*   \var _buffer::read_ptr
*   read pointer
*   \var _buffer::write_ptr
*   write pointer
*   \var _buffer::type 
*   buffer type (LINEAR or RING)
*   \var _buffer::state
*   buffer state (EMPTY, FULL or NORMAL)
*   \var _buffer::in_use
*   is the buffer currently being read from/written to?
*   \var _buffer::ext_buf
*   was the buffer initialized externally (i.e. not with ``buffer_init()``)?
*/

#define ASSERT_RBUF(buf) if (buf->type != RING) { return BAD_ARG; }
#define ASSERT_LBUF(buf) if (buf->type != LINEAR) { return BAD_ARG; }

unsigned int _lbuf_n_write_bytes(buffer_t *buf) {
  ASSERT_LBUF(buf);
  return buf->buf + buf->buf_len_bytes - buf->write_ptr;
}

unsigned int _rbuf_n_write_bytes(buffer_t *buf) {
  long long int diff;
  ASSERT_RBUF(buf);
  switch(buf->state) {
    case EMPTY: 
      return buf->buf_len_bytes;
    case FULL:
      return 0;
    default:
      diff = (buf->read_ptr - buf->write_ptr) % buf->buf_len_bytes;
      if (diff < 0) {
        diff += buf->buf_len_bytes;
      }
      return (unsigned int)diff;
  }
}

unsigned int _lbuf_n_read_bytes(buffer_t *buf) {
  ASSERT_LBUF(buf);
  return buf->write_ptr - buf->read_ptr;
}

unsigned int _rbuf_n_read_bytes(buffer_t *buf) {
  ASSERT_RBUF(buf);
  return (buf->buf_len_bytes - _rbuf_n_write_bytes(buf));
}

unsigned int _rbuf_linear_bytes_write(buffer_t *buf) {
  ASSERT_RBUF(buf);
  unsigned int write_pos = buf->write_ptr - buf->buf;
  switch(buf->state) {
    case FULL:
      return 0;
    case EMPTY:
      return buf->buf_len_bytes - write_pos;
    case NORMAL:
      if (buf->write_ptr < buf->read_ptr) {
        return buf->read_ptr - buf->write_ptr;
      }
       else {
         return buf->buf_len_bytes - write_pos;
       }
  }
}

unsigned int _rbuf_linear_bytes_read(buffer_t *buf) {
  ASSERT_RBUF(buf);
  unsigned int read_pos = buf->read_ptr - buf->buf;
  switch(buf->state) {
    case FULL:
      return 0;
    case EMPTY:
      return buf->buf_len_bytes - read_pos;
    case NORMAL:
      if (buf->read_ptr < buf->write_ptr) {
        return buf->write_ptr - buf->read_ptr;
      }
       else {
         return buf->buf_len_bytes - read_pos;
       }
  }
}

int _set_lbuf_state(buffer_t *buf) {
  ASSERT_LBUF(buf);
  if (buf->read_ptr == buf->write_ptr) {
    buffer_reset(buf);
  }
  else if (buf->write_ptr == buf->buf + buf->buf_len_bytes) {
    buf->state = FULL;
  }
  else {
    buf->state = NORMAL;
  }
  return 0;
}

int _set_rbuf_state(buffer_t *buf, char mode) {
  ASSERT_RBUF(buf);
  if (buf->read_ptr != buf->write_ptr) {
    buf->state = NORMAL;
    return 0;
  }
  switch (mode) {
    case 'r':
      buf->state = EMPTY;
      return 0;
    case 'w':
      buf->state = FULL;
      return 0;
  }
}

unsigned int buffer_n_write_bytes(buffer_t *buf) {
  switch(buf->type) {
    case LINEAR:
      return _lbuf_n_write_bytes(buf);
    case RING:
      return _rbuf_n_write_bytes(buf);
  }
}

unsigned int buffer_n_read_bytes(buffer_t *buf) {
  switch(buf->type) {
    case LINEAR:
      return _lbuf_n_read_bytes(buf);
    case RING:
      return _rbuf_n_read_bytes(buf);
  }
}

unsigned int set_buffer_state(buffer_t *buf, char mode) {
  switch(buf->type) {
    case LINEAR:
      return _set_lbuf_state(buf);
    case RING:
      return _set_rbuf_state(buf, mode);
  }
}

int buffer_reset(buffer_t *buf) {
  buf->read_ptr = buf->buf;
  buf->write_ptr = buf->buf;
  buf->state = EMPTY;
  return 0;
}

int buffer_set_rpos(buffer_t *buf, unsigned int pos) {
  buf->read_ptr = buf->buf + pos;
  return 0;
}

int buffer_set_wpos(buffer_t *buf, unsigned int pos) {
  buf->write_ptr = buf->buf + pos;
}

int buffer_set_state(buffer_t *buf, buffer_state_t state) {
  buf->state = state;
}

/*! \brief Initialize buffer handle
* \param buf address of ``buffer_t*`` handle
* \param n_bytes number of bytes to allocate
* \param type buffer type (RING or LINEAR)
* \return exit code
*/
int buffer_init(buffer_t **buf, unsigned int n_bytes, buffer_type_t type) {
  void *raw_buffer = malloc(n_bytes);
  memset(raw_buffer, 0, n_bytes);
  *buf = (buffer_t*)malloc(sizeof(buffer_t));
  (*buf)->buf = raw_buffer;
  (*buf)->buf_len_bytes = n_bytes;
  (*buf)->read_ptr = raw_buffer;
  (*buf)->write_ptr = raw_buffer;
  (*buf)->type = type;
  (*buf)->state = EMPTY;
  (*buf)->in_use = false;
  (*buf)->ext_buf = false;
  (*buf)->r = NULL;
  return 0;
}


/*! \brief Initialize buffer handle with an already-allocated memory area
* \param buf address of ``buffer_t*`` handle
* \param n_bytes number of bytes to allocate
* \param type buffer type (RING or LINEAR)
* \param raw_buffer buffer memory location
* \return exit code
*/
int buffer_init_ext(buffer_t **buf, unsigned int n_bytes, buffer_type_t type, void *raw_buffer) {
  *buf = (buffer_t*)malloc(sizeof(buffer_t));
  (*buf)->buf = raw_buffer;
  (*buf)->buf_len_bytes = n_bytes;
  (*buf)->read_ptr = raw_buffer;
  (*buf)->write_ptr = raw_buffer;
  (*buf)->type = type;
  (*buf)->state = EMPTY;
  (*buf)->in_use = false;
  (*buf)->ext_buf = true;
  (*buf)->r = NULL;
  (*buf)->r_id = 0;
  return 0;
}

int buffer_add_recording(buffer_t *buf, recording_t *r) {
  buf->r_id = hm_recording_n_buffers(r);
  buf->r = r;
  return 0;
}

/*! \brief Free buffer memory
* \param buf address of ``buffer_t*`` handle
* \return exit code
*/
int buffer_delete(buffer_t **buf) {
  if (!((*buf)->ext_buf)) {
    free((*buf)->buf);
  }
  free(*buf);
  *buf = NULL;
  return 0;
}

#define PREPARE_BUF_WRITE(buf) \
  if (buf->in_use == true) { \
    return EAGAIN; \
  } \
  buf->in_use = true; \
  unsigned int n_bytes_available = buffer_n_write_bytes(buf); \
  if (n_bytes_available < n_bytes) { \
    return EAGAIN; \
  }

int _lbuf_write(buffer_t *dest, const void *src, unsigned int n_bytes) {
  ASSERT_LBUF(dest);
  PREPARE_BUF_WRITE(dest)
  memcpy(dest->write_ptr, src, n_bytes);
  dest->write_ptr += n_bytes;
  set_buffer_state(dest, '-');
  dest->in_use = false;
  return 0;
}

int _rbuf_write(buffer_t *dest, const void *src, unsigned int n_bytes) {
  ASSERT_RBUF(dest);
  PREPARE_BUF_WRITE(dest)
  void *linear_buf_end = dest->buf + dest->buf_len_bytes;
  unsigned int linear_bytes_available = _rbuf_linear_bytes_write(dest);
  bool split_write = (n_bytes > linear_bytes_available) ? true : false;
  if (split_write) {
    //first write to buffer
    memcpy(dest->write_ptr, src, linear_bytes_available);
    //second write to buffer
    const void *remaining_src_buf = src + linear_bytes_available;
    memcpy(dest->buf, remaining_src_buf, \
      n_bytes - linear_bytes_available);
    dest->write_ptr = dest->buf + (n_bytes - linear_bytes_available);
    set_buffer_state(dest, 'w');
  }
  else {
    memcpy(dest->write_ptr, src, n_bytes);
    dest->write_ptr += n_bytes;
    set_buffer_state(dest, 'w');
  }
  dest->in_use = false;
  return 0;
}

int buffer_write(buffer_t *dest, const void *src, unsigned int n_bytes) {
  #ifdef WITH_PROTOBUF
  if(dest->r != NULL) {
    hm_recording_write(dest->r, dest->r_id, src, n_bytes);
  }
  #endif
  switch(dest->type) {
    case LINEAR:
      return _lbuf_write(dest, src, n_bytes);
    case RING:
      return _rbuf_write(dest, src, n_bytes);
  }
}

#define PREPARE_BUF_READ(buf) \
  if (buf->in_use == true) { \
    return EAGAIN; \
  } \
  buf->in_use = true; \
  unsigned int n_bytes_available = buffer_n_read_bytes(buf); \
  if (n_bytes_available < n_bytes) { \
    return EAGAIN; \
  }

int _lbuf_read(buffer_t *src, void *dest, unsigned int n_bytes) {
  ASSERT_LBUF(src);
  PREPARE_BUF_READ(src)
  memcpy(dest, src->read_ptr, n_bytes);
  src->read_ptr += n_bytes;
  set_buffer_state(src, '-');
  src->in_use = false;
  return 0;
}

int _rbuf_read(buffer_t *src, void *dest, unsigned int n_bytes) {
  //ASSERT_RBUF(src);
  if (src->type != RING) { 
    return BAD_ARG; 
  }
  PREPARE_BUF_READ(src)
  void *linear_buf_end = src->buf + src->buf_len_bytes;
  unsigned int linear_bytes_available = _rbuf_linear_bytes_read(src);
  bool split_read = (n_bytes > linear_bytes_available) ? true : false;
  if (split_read) {
    //first read from buffer
    memcpy(dest, src->read_ptr, linear_bytes_available);
    //second read from buffer
    void *remaining_dest_buf = dest + linear_bytes_available;
    memcpy(remaining_dest_buf, src->buf, \
      n_bytes - linear_bytes_available);
    src->read_ptr = src->buf + (n_bytes - linear_bytes_available);
    set_buffer_state(src, 'r');
  }
  else {
    memcpy(dest, src->read_ptr, n_bytes);
    src->read_ptr += n_bytes;
    set_buffer_state(src, 'r');
  }
  src->in_use = false;
  return 0;
}

int buffer_read(buffer_t *src, void *dest, unsigned int n_bytes) {
  switch(src->type) {
    case LINEAR:
      return _lbuf_read(src, dest, n_bytes);
    case RING:
      return _rbuf_read(src, dest, n_bytes);
  }
}
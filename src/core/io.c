#include "intern/core.h"
#include "intern/async.h"
#include "intern/io.h"
#include "intern/buffer.h"

#include <stdlib.h>
#include <unistd.h>

IMPL(src_fd) {
  long int fd = (long int)_inputs;
  buffer_t *buf = (buffer_t*)_outputs;
  void *tmp_buf = (void*)malloc(n_bytes);
  read(fd, tmp_buf, n_bytes);
  buffer_write(buf, tmp_buf, n_bytes);
  free(tmp_buf);
  return 0;  
}
/*! \brief Source (unix FD) 
* \param fd file descriptor
* \param n_bytes number of bytes to read from fd
* \param buf buffer address
* \return exit code
*/
int src_fd(int fd, unsigned int n_bytes, void *buf) {
  buffer_t *dest = NULL;
  buffer_init_ext(&dest, n_bytes, RING, buf);
  src_fd_impl(n_bytes, (void*)(long int)fd, (void*)dest, NULL);
  buffer_delete(&dest);
  return 0;
}

async_args_t src_fd_async(int fd, unsigned int n_bytes, void *buf) {
  async_args_t async_args;
  kwargs_t kwargs;
  async_args.fn = src_fd_impl;
  async_args.inputs = (void*)(long int)fd;
  async_args.outputs = (void*)buf;
  async_args.kwargs = kwargs;
  return async_args;
}


IMPL(sink_fd) {
  buffer_t *buf = (buffer_t*)_inputs;
  long int fd = (long int)_outputs;
  void *tmp_buf = (void*)malloc(n_bytes);
  buffer_read(buf, tmp_buf, n_bytes);
  write(fd, tmp_buf, n_bytes);
  free(tmp_buf);
  return 0;  
}

/*! \brief Sink (unix FD) 
* \param fd file descriptor
* \param n_bytes number of bytes to write to fd
* \param buf buffer address
* \return exit code
*/
int sink_fd(int fd, unsigned int n_bytes, void *buf) {
  buffer_t *src = NULL;
  buffer_init_ext(&src, n_bytes, RING, buf);
  src->state = FULL;
  sink_fd_impl(n_bytes, (void*)src, (void*)(long int)fd, NULL);
  buffer_delete(&src);
  return 0;
}

async_args_t sink_fd_async(int fd, unsigned int n_bytes, void *buf) {
  async_args_t async_args;
  kwargs_t kwargs;
  async_args.fn = sink_fd_impl;
  async_args.inputs = (void*)buf;
  async_args.outputs = (void*)(long int)fd;
  async_args.kwargs = kwargs;
  return async_args;
}


IMPL(src_file) {
  FILE *f = (FILE*)_inputs;
  buffer_t *buf = (buffer_t*)_outputs;
  void *tmp_buf = (void*)malloc(n_bytes);
  fread(tmp_buf, 1, n_bytes, f);
  buffer_write(buf, tmp_buf, n_bytes);
  free(tmp_buf);
  return 0;  
}

/*! \brief Source (file) 
* \param f ``FILE*`` handle
* \param n_bytes number of bytes to read from file
* \param buf buffer address
* \return exit code
*/
int src_file(FILE *f, unsigned int n_bytes, void *buf) {
  buffer_t *dest = NULL;
  buffer_init_ext(&dest, n_bytes, RING, buf);
  src_file_impl(n_bytes, (void*)f, (void*)dest, NULL);
  buffer_delete(&dest);
  return 0;
}

async_args_t src_file_async(FILE *f, unsigned int n_bytes, void *buf) {
  async_args_t async_args;
  kwargs_t kwargs;
  async_args.fn = src_file_impl;
  async_args.inputs = (void*)f;
  async_args.outputs = (void*)buf;
  async_args.kwargs = kwargs;
  return async_args;
}


IMPL(sink_file) {
  buffer_t *buf = (buffer_t*)_inputs;
  FILE* f = (FILE*)_outputs;
  void *tmp_buf = (void*)malloc(n_bytes);
  buffer_read(buf, tmp_buf, n_bytes);
  fwrite(tmp_buf, 1, n_bytes, f);
  return 0;  
}

/*! \brief Sink (file) 
* \param f ``FILE*`` handle
* \param n_bytes number of bytes to write to file
* \param buf buffer address
* \return exit code
*/
int sink_file(FILE *f, unsigned int n_bytes, void *buf) {
  buffer_t *src = NULL;
  buffer_init_ext(&src, n_bytes, RING, buf);
  src->state = FULL;
  sink_file_impl(n_bytes, (void*)src, (void*)f, NULL);
  buffer_delete(&src);
  return 0;
}

async_args_t sink_file_async(FILE *f, unsigned int n_bytes, void *buf) {
  async_args_t async_args;
  kwargs_t kwargs;
  async_args.fn = sink_file_impl;
  async_args.inputs = (void*)buf;
  async_args.outputs = (void*)f;
  async_args.kwargs = kwargs;
  return async_args;
}

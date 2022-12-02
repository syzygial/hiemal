#include "core.h"
#include "intern/async.h"
#include "io.h"

#include <unistd.h>

IMPL(src_fd) {
  long int fd = (long int)_inputs;
  void *buf = (void*)_outputs;
  read(fd, buf, n_bytes);
  return 0;  
}
/*! \brief Source (unix FD) 
* \param fd file descriptor
* \param n_bytes number of bytes to read from fd
* \param buf buffer address
* \return exit code
*/
int src_fd(int fd, unsigned int n_bytes, void *buf) {
  kwargs_t kwargs;
  src_fd_impl(n_bytes, (void*)(long int)fd, (void*) buf, &kwargs);
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
  void *buf = (void*)_inputs;
  long int fd = (long int)_outputs;
  write(fd, buf, n_bytes);
  return 0;  
}

/*! \brief Sink (unix FD) 
* \param fd file descriptor
* \param n_bytes number of bytes to write to fd
* \param buf buffer address
* \return exit code
*/
int sink_fd(int fd, unsigned int n_bytes, void *buf) {
  kwargs_t kwargs;
  sink_fd_impl(n_bytes, (void*)buf, (void*)(long int)fd, &kwargs);
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
  void *buf = (void*)_outputs;
  fread(buf, 1, n_bytes, f);
  return 0;  
}

/*! \brief Source (file) 
* \param f ``FILE*`` handle
* \param n_bytes number of bytes to read from file
* \param buf buffer address
* \return exit code
*/
int src_file(FILE *f, unsigned int n_bytes, void *buf) {
  kwargs_t kwargs;
  src_file_impl(n_bytes, (void*)f, (void*)buf, &kwargs);
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
  void *buf = (void*)_inputs;
  FILE* f = (FILE*)_outputs;
  fwrite(buf, 1, n_bytes, f);
  return 0;  
}

/*! \brief Sink (file) 
* \param f ``FILE*`` handle
* \param n_bytes number of bytes to write to file
* \param buf buffer address
* \return exit code
*/
int sink_file(FILE *f, unsigned int n_bytes, void *buf) {
  kwargs_t kwargs;
  sink_file_impl(n_bytes, (void*)buf, (void*)f, &kwargs);
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

#ifndef _IO_H
#define _IO_H

#include "intern/buffer.h"
#include "intern/async.h"
#include <stdio.h>



int src_fd(int fd, unsigned int n_bytes, void *buf);
int sink_fd(int fd, unsigned int n_bytes, void *buf);
int src_file(FILE *f, unsigned int n_bytes, void *buf);
int sink_file(FILE *f, unsigned int n_bytes, void *buf);

async_args_t src_fd_async(int fd, buffer_t *buf);
async_args_t sink_fd_async(int fd, buffer_t *buf);
async_args_t src_file_async(FILE *f, buffer_t *buf);
async_args_t sink_file_async(FILE *f, buffer_t *buf);
#endif

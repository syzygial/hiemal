#ifndef _INTERN_IO_H
#define _INTERN_IO_H

#include "intern/buffer.h"
#include <stdio.h>



int src_fd(int fd, unsigned int n_bytes, void *buf);
int sink_fd(int fd, unsigned int n_bytes, void *buf);
int src_file(FILE *f, unsigned int n_bytes, void *buf);
int sink_file(FILE *f, unsigned int n_bytes, void *buf);
#endif

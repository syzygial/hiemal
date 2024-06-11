#ifndef _UTIL_FILE_H
#define _UTIL_FILE_H

int is_regular_file(const char *path);
int is_dir(const char *path);
int is_valid_path(const char *path);

#endif
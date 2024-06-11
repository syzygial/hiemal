#include "util/file.h"

#ifdef __unix__
#include <sys/stat.h>
#endif

int is_regular_file(const char *path) {
  struct stat s;
  if (stat(path, &s) != 0) {
    return 0;
  }
  else if ((s.st_mode & S_IFMT) != S_IFREG) {
    return 0;
  }
  return 1;
}

int is_dir(const char *path) {
  struct stat s;
  if (stat(path, &s) != 0) {
    return 0;
  }
  else if ((s.st_mode & S_IFMT) != S_IFDIR) {
    return 0;
  }
  return 1;
}

int is_valid_path(const char *path) {
  struct stat s;
  if (stat(path, &s) != 0) {
    return 0;
  }
  return 1;
}
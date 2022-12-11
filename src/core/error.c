#include <string.h>

#include "intern/error.h"

int AL_errno = 0;
char AL_errtxt[ERRTXT_LEN] = {'\0'};

char *AL_errstr(int err) {
  switch (err) {
    case BAD_HEADER:
      return "Bad file format";
    case MISSING_IMPL:
      return "Feature not implemented";
    case BUF_UNDERRUN:
      return "Buffer underrun";
    case BUF_OVERRUN:
      return "Buffer overrun";
    case BAD_ARG:
      return "Bad argument passed to function";
    default:
      return "Undefined error code";
  }
}
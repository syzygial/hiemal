#include "util/tty.h"

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#ifdef __unix__
#include <unistd.h>
#endif

#ifdef _WIN32
#define isatty(x) _isatty(x)
#endif

int printf_clr(tty_clr_t fg, tty_clr_t bg, char *fmt, ...) {
  va_list l;
  va_start(l, fmt);
  if (!isatty(STDOUT_FILENO) || (fg == DEFAULT && bg == DEFAULT)) {
    vprintf(fmt, l);
    goto done;
  }
  bool append_nl = false;
  if (fmt[strlen(fmt) - 1] == '\n') {
    fmt[strlen(fmt) - 1] = '\0';
    append_nl = true;
  }
  if (fg != DEFAULT) printf("\e[%dm", fg + 30);
  if (bg != DEFAULT) printf("\e[%dm", bg + 40);
  printf("\e[0m");
  if (append_nl) printf("\n");
  done:
  va_end(l);
  return 0;
}


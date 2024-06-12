#include <stdint.h>
#include <string.h>

#include "util/unicode.h"

#define IS_ASCII(x) ((x & 0x80) == 0)

int ascii_to_utf16(const char *src, uint16_t *res) {
  int i = 0;
  for (i = 0; i < strlen(src); i++) {
    if (!IS_ASCII(src[i])) {
      return -1;
    } 
    res[i] = (uint16_t)(src[i]);
  }
  return i;
}

int ascii_to_utf32(const char *src, uint32_t *res) {
  int i = 0;
  for (i = 0; i < strlen(src); i++) {
    if (!IS_ASCII(src[i])) {
      return -1;
    } 
    res[i] = (uint32_t)(src[i]);
  }
  return i;
}

int ascii_to_wchar(const char *src, wchar_t *res) {
  if (sizeof(wchar_t) == 2) {
    return ascii_to_utf16(src, (uint16_t*)res);
  }
  else if (sizeof(wchar_t) == 4) {
    return ascii_to_utf32(src, (uint32_t*)res);
  }
  else {
    return -1;
  }
}
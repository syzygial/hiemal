#ifndef _ERROR_H
#define _ERROR_H

#define ERRTXT_LEN 100

extern int AL_errno;
extern char AL_errtxt[ERRTXT_LEN];

#define BAD_HEADER 0x101
#define MISSING_IMPL 0x102
#define BUF_UNDERRUN 0x103
#define BUF_OVERRUN 0x104
#define BAD_ARG 0x105
char *AL_errstr(int err);

#endif
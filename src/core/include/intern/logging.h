#ifndef _INTERN_LOGGING_H
#define _INTERN_LOGGING_H

#include "api/logging.h"

int hm_log_err(const char *msg, const char *fn_name);
int hm_log_warn(const char *msg, const char *fn_name);
int hm_log_info(const char *msg, const char *fn_name);
int hm_log_debug(const char *msg, const char *fn_name);
int hm_log_verbose(const char *msg, const char *fn_name);

#endif

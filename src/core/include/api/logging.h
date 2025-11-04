#ifndef _LOGGING_H
#define _LOGGING_H

#define LOG_LVL_ERR 1
#define LOG_LVL_WARN 2
#define LOG_LVL_INFO 3
#define LOG_LVL_DEBUG 4
#define LOG_LVL_VERBOSE 5

extern unsigned int log_level;

unsigned int hm_set_loglvl(unsigned int lvl);
unsigned int hm_get_loglvl();

#endif
#include <stdio.h>
#include <string.h>

#include "intern/logging.h"

unsigned int log_level = LOG_LVL_WARN;



unsigned int hm_set_loglvl(unsigned int lvl) {
  log_level = lvl;
  return log_level;
}

unsigned int hm_get_loglvl() { 
  return log_level;
}

int hm_log_msg(unsigned int lvl, char *msg) {
  if (log_level >= lvl) {
    printf("%s\n", msg);
  }
  return 0;
}

int hm_log_err(char *msg) {
  return hm_log_msg(LOG_LVL_ERR, strcat("[ERROR]: ", msg));
}

int hm_log_warn(char *msg) {
  return hm_log_msg(LOG_LVL_WARN, strcat("[WARNING]: ", msg));
}

int hm_log_info(char *msg) {
  return hm_log_msg(LOG_LVL_INFO, strcat("[INFO]: ", msg));
}

int hm_log_debug(char *msg) {
  return hm_log_msg(LOG_LVL_DEBUG, strcat("[DEBUG]: ", msg));
}

int hm_log_verbose(char *msg) {
  return hm_log_msg(LOG_LVL_VERBOSE, strcat("[VERBOSE]: ", msg));
}



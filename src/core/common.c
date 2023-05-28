#include "intern/common.h"

#include <ctype.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>

struct hm_type {
  HM_TYPE_HEAD
};

struct hm_list_node {
  HM_LIST_NODE_HEAD(hm_list_node_t)
};

struct hm_list {
  HM_LIST_HEAD(hm_list_node_t)
};

bool is_list (hm_type_t *obj) {
  struct hm_type *_obj = (struct hm_type*)obj;
  return (_obj->magic == HM_LIST_MAGIC) ? true : false;
}

bool is_list_node (hm_type_t *obj) {
  struct hm_type *_obj = (struct hm_type*)obj;
  return (_obj->magic == HM_LIST_NODE_MAGIC) ? true : false;
}

int hm_list_insert(hm_list_t *list, hm_list_node_t *node, unsigned int index) {
  struct hm_list *_list = (struct hm_list*)list;
  struct hm_list_node *_node = (struct hm_list_node*)node;
  if (index == -1) index = _list->n_items;
  if (_list->n_items < index) return -1;
  struct hm_list_node **node_itr = (struct hm_list_node**)&(_list->head);
  struct hm_list_node **node_prev = node_itr;

  int i = 0;
  for (i = 0; i < index; i++) {
    node_prev = node_itr;
    node_itr = (struct hm_list_node**)&((*node_itr)->next);
  }

  _node->next = *node_itr;
  _node->prev = *node_prev;
  *node_itr = _node;
  _list->n_items++;
  return 0;
}

int hm_list_append(hm_list_t *list, hm_list_node_t *node) {
  return hm_list_insert(list, node, -1);
}

int hm_list_itr(hm_list_t *list, list_node_fn *itr_fn) {
  struct hm_list *_list = (struct hm_list*)list;
  struct hm_list_node *node_itr = _list->head;
  while (node_itr != NULL) {
    (*itr_fn)(node_itr);
    node_itr = node_itr->next;
  }
  return 0;
}

int hm_list_remove(hm_list_t *list, unsigned int index) {
  struct hm_list *_list = (struct hm_list*)list;
}

int hm_list_delete(hm_list_t *list, list_node_fn *free_fn) {
  struct hm_list_node *node_itr = ((struct hm_list*)list)->head;
  struct hm_list_node *node_next = node_itr;

  while (node_itr != NULL) {
    node_next = node_itr->next;
    (free_fn) ? (*free_fn)(node_itr) : free(node_itr);
    node_itr = node_next;
  }
  free(list);
  return 0;
}


int isvarchar(int c) {
  return (isalnum(c) || c == '_') ? 1 : 0;
}

char *next_arg(char *const str, char d, char *d_opt, int *arglen, bool *optional) {
  SET_DEFAULT_ARG(d, ';', '\0');
  SET_DEFAULT_ARG(d_opt, "()", NULL);
  char *next;
  next = str;
  if (!isvarchar(*next)) {
    while(!isvarchar(*next)) {
      if (*next == d_opt[0]) *optional = true;
      else if (*next == d_opt[1]) *optional = false;
      next++;
    }
    *arglen = -1;
    return next;
  }
  bool last_arg = true;
  while(isvarchar(*next) && *next != '\0' && *next != d) next++;
  *arglen = (int)(next - str);
  while(!isvarchar(*next) && *next != '\0') {
    if(*next == d_opt[0]) *optional = true;
    else if (*next == d_opt[1]) *optional = false;
    next++;
  }
  char *end = next;
  while(*end != '\0') {
    if (isvarchar(*end)) last_arg = false;
    end++;
  }
  return last_arg ? NULL : next;
}

int kwargs_unpack(kwargs_t *kwargs, char *fmt, ...) {
  char *fmt_itr = fmt;
  char *fmt_itr_prev = fmt_itr;
  int arglen = 0;
  bool opt = false;
  bool opt_prev = opt;
  char *key = NULL;
  char **key_itr = kwargs->arg_names;
  int n_keys = kwargs->argc;
  bool found_key = false;
  int i;

  void **arg_itr = NULL;

  va_list vars;
  va_start(vars, fmt);
  while (fmt_itr != NULL) {
    fmt_itr_prev = fmt_itr;
    opt_prev = opt;
    
    fmt_itr = next_arg(fmt_itr, ';', NULL, &arglen, &opt);
    if (arglen < 1) continue;
    arg_itr = va_arg(vars, void**);
    key = strndup(fmt_itr_prev, arglen);
    found_key = false;
    for (i = 0; i < n_keys; i++) {
      if (strcmp(key, kwargs->arg_names[i]) == 0) {
        *arg_itr = (kwargs->args)[i];
        found_key = true;
        break;
      }
    }
    if (!found_key && !opt_prev) {
      free(key);
      return -1; // missing required key
    }
    free(key);
  }
  va_end(vars);
  return 0;
}
#include "intern/common.h"
#include "intern/math.h"

#include <ctype.h>
#include <string.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define HM_TYPE_ID(magic) (magic & 0xFFFFFFFF)
#define HM_SUBTYPE_ID(magic) ((magic & ((uint64_t)(0xFFFFFFFF) << 32)) >> 32)

struct hm_type {
  HM_TYPE_HEAD
  uint8_t data[];
};

struct hm_list_node {
  HM_LIST_NODE_HEAD(hm_list_node_t)
  uint8_t data[];
};

struct hm_list_refnode {
  HM_LIST_NODE_HEAD(struct hm_list_refnode)
  hm_list_node_t *node;
};

struct hm_list {
  HM_LIST_HEAD(hm_list_node_t)
};

struct hm_array {
  HM_ARRAY_HEAD(void)
};

int default_node_free(hm_list_node_t *node) {
  free(node);
  return 0;
}

list_node_fn *default_node_free_fn = default_node_free;

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
  if (HM_SUBTYPE_ID(_list->magic) == 0) {
    _node->next = *node_itr;
    _node->prev = *node_prev;
    *node_itr = _node;
  }
  else if (HM_SUBTYPE_ID(_list->magic) == HM_LIST_REFNODE_MAGIC) {
    struct hm_list_refnode *new_refnode = (struct hm_list_refnode*)malloc(sizeof(struct hm_list_refnode));
    HM_REFLIST_NODE_INIT(new_refnode);
    new_refnode->node = node;
    new_refnode->next = (struct hm_list_refnode*)(*node_itr);
    new_refnode->prev = (struct hm_list_refnode*)(*node_prev);
    *node_itr = (struct hm_list_node*)new_refnode;
  }
  _list->n_items++;
  return 0;
}

int hm_list_clear(hm_list_t *list) {
  struct hm_list *_list = (struct hm_list*)list;
  struct hm_list_node *node_itr = _list->head;
  struct hm_list_node *node_next = node_itr;

  while (node_itr != NULL) {
    node_next = node_itr->next;
    if (node_itr->free_fn != NULL) (node_itr->free_fn)(node_itr);
    node_itr = node_next;
  }
  _list->n_items = 0;
  return 0;
}

int hm_list_copy_array(hm_list_t *list, hm_array_t *arr, int flags) {
  struct hm_list *_list = (struct hm_list*)list;
  struct hm_array *_arr = (struct hm_array*)arr;
  if (HM_FLAG_SET(flags, COPY_OVERWRITE)) {
    hm_list_clear(list);
  }
  struct hm_list_node *new_node = NULL;
  int i = 0;
  for (i = 0; i < _arr->n_items; i++) {
    new_node = (hm_list_node_t*)malloc(offsetof(struct hm_list_node, data) + _arr->item_size);
    strncpy(new_node->data, _arr->buf + (i*_arr->item_size), _arr->item_size);
    hm_list_append(list, new_node);
  }
  return 0;
}

hm_list_node_t* hm_list_at(hm_list_t *list, unsigned int index) {
  struct hm_list *_list = (struct hm_list*)list;
  struct hm_list_node *node_itr = (struct hm_list_node*)(_list->head);
  if (index == -1) {
    index = _list->n_items;
  }
  if (_list->n_items < index) return NULL;
  int i = 0;
  for (i = 0; i < index; i++) {
    node_itr = node_itr->next;
  }
    if (HM_SUBTYPE_ID(_list->magic) == HM_LIST_REFNODE_MAGIC) return ((struct hm_list_refnode*)node_itr)->node;
    else return node_itr;
}

hm_list_node_t* hm_list_node_extract(hm_list_node_t *node) {
  struct hm_list_node *_node = (struct hm_list_node*)node;
  if (HM_SUBTYPE_ID(_node->magic) == HM_LIST_REFNODE_MAGIC) return ((struct hm_list_refnode*)node)->node;
  else return node;
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

hm_list_node_t *hm_list_find(hm_list_t *list, list_node_cmp_fn *find_fn, void *userdata) {
  struct hm_list *_list = (struct hm_list*)list;
  struct hm_list_node *node_itr = _list->head;
  while (node_itr != NULL) {
    if ((*find_fn)(node_itr, userdata) == 0) {
      return node_itr;
    }
    node_itr = node_itr->next;
  }
  return NULL;
}

int hm_list_remove(hm_list_t *list, unsigned int index, list_node_fn *free_fn) {
  struct hm_list *_list = (struct hm_list*)list;
  if (index == -1) index = _list->n_items - 1;
  if (_list->n_items < index) return -1;
  struct hm_list_node **node_itr = (struct hm_list_node**)&(_list->head);
  struct hm_list_node **node_prev = node_itr;

  int i = 0;
  for (i = 0; i < index; i++) {
    node_prev = node_itr;
    node_itr = (struct hm_list_node**)&((*node_itr)->next);
  }
  struct hm_list_node *old_node = *node_itr;
  *node_itr = (struct hm_list_node*)(old_node->next);
  if (*node_itr != NULL)(*node_itr)->prev = *node_prev;
  (free_fn) ? (*free_fn)(old_node) : free(old_node);
  _list->n_items--;
  return 0;
}

int hm_list_remove_where(hm_list_t *list, list_node_cmp_fn *cmp_fn, list_node_fn *free_fn, void *userdata) {
  struct hm_list *_list = (struct hm_list*)list;
  struct hm_list_node **node_itr = (struct hm_list_node**)&(_list->head);
  struct hm_list_node **node_itr_next = NULL;
  struct hm_list_node **node_prev = node_itr;
  struct hm_list_node *old_node = NULL;
  int i = 0;
  while (*node_itr != NULL) {
    node_prev = node_itr;
    old_node = *node_itr;
    node_itr = (struct hm_list_node**)&(old_node->next);
    if((*cmp_fn)(old_node, userdata) == 0) {
      *node_prev = (struct hm_list_node*)(old_node->next);
      if (*node_prev != NULL) (*node_prev)->prev = *node_prev;
      (free_fn) ? (*free_fn)(old_node) : free(old_node);
      _list->n_items--;
    }
    i++;
  }
  return 0;
}

int hm_list_delete(hm_list_t *list) {
  hm_list_clear(list);
  free(list);
  return 0;
}


int hm_array_resize(hm_array_t *arr, unsigned int n_items) {
  struct hm_array *_arr = (struct hm_array*)arr;
  if (_arr->n_items_alloc < n_items) {
    _arr->buf = realloc(_arr->buf, 2*n_items*_arr->item_size);
    _arr->n_items_alloc = n_items;
  }
  return 0;
}

int hm_array_copy_raw(hm_array_t *arr, void *buf, unsigned int n_items) {
  struct hm_array *_arr = (struct hm_array*)arr;
  hm_array_resize(arr, n_items);
  _arr->n_items = n_items;
  strncpy(_arr->buf, buf, n_items*_arr->item_size);
  return 0;
}

int hm_array_clear(hm_array_t *arr) {
  struct hm_array *_arr = (struct hm_array*)arr;
  free(_arr->buf);
  _arr->buf = NULL;
  _arr->n_items = 0;
  _arr->n_items_alloc = 0;
  return 0;
}

int hm_array_copy_list(hm_array_t *arr, hm_list_t *list, int flags) {
  struct hm_array *_arr = (struct hm_array*)arr;
  struct hm_list *_list = (struct hm_list*)list;
  if (HM_FLAG_SET(flags, COPY_OVERWRITE)) {
    hm_array_clear(arr);
  }
  void **list_data = (void**)malloc(_list->n_items * sizeof(void*));
  uint64_t *list_data_len = (uint64_t*)malloc(_list->n_items * _list->n_items);
  int i = 0;
  int get_list_data(hm_list_node_t *node) {
    struct hm_list_node *_node = (struct hm_list_node*)node;
    list_data[i] = _node->data;
    list_data_len[i] = _node->data_size;
    i++;
    return 0;
  }
  hm_list_itr(list, get_list_data);
  i = 0;
  int ii = 0; // running sum of list node sizes
  uint64_t new_arr_len = u64sum(list_data_len, _list->n_items);
  // TODO: add coherence check re. non-uniform lists/arrays
  void *new_arr_data = (void*)malloc(new_arr_len);
  for (i = 0; i < _list->n_items; i++) {
    strncpy(new_arr_data + ii, list_data[i], list_data_len[i]);
    ii += list_data_len[i];
  }
  hm_array_concat_raw(arr, new_arr_data, _list->n_items);
  free(list_data_len);
  free(list_data);
  return 0;
}

void* hm_array_at(hm_array_t *arr, unsigned int index) {
  struct hm_array *_arr = (struct hm_array*)arr;
  if (index < _arr->n_items) {
    return (void*)(_arr->buf) + index * _arr->item_size;
  }
  else {
    return NULL;
  }
}

int hm_array_concat(hm_array_t *arr1, hm_array_t *arr2) {
  struct hm_array *_arr1 = (struct hm_array*)arr1;
  struct hm_array *_arr2 = (struct hm_array*)arr2;
  int new_size = _arr1->n_items + _arr2->n_items;
  hm_array_resize(arr1, new_size);
  memcpy((char*)(_arr1->buf) + _arr1->n_items*_arr1->item_size, _arr2->buf, _arr2->n_items * _arr2->item_size);
  _arr1->n_items = new_size;
  return 0;
}

int hm_array_concat_raw(hm_array_t *arr, void *data, unsigned int n_items) {
  struct hm_array *_arr1 = (struct hm_array*)arr;
  struct hm_array *_new_arr = (struct hm_array*)malloc(sizeof(struct hm_array));
  HM_ARRAY_INIT_LIKE(_new_arr, _arr1)
  hm_array_resize(_new_arr, n_items);
  _new_arr->n_items = n_items;
  memcpy(_new_arr->buf, data, n_items*_new_arr->item_size);
  hm_array_concat(arr, _new_arr);
  free(_new_arr);
  return 0;
}

int hm_array_delete(hm_array_t *arr, array_item_fn *free_fn) {
  struct hm_array *_arr = (struct hm_array*)arr;
  if (free_fn != NULL) {
    int i = 0;
    for (i = 0; i < _arr->n_items; i++) free_fn(_arr->buf + i);
  }
  free(arr);
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

int xasprintf(char **str, char *fmt, ...) {
  va_list vars, vars_copy;
  va_start(vars, fmt);
  va_copy(vars_copy, vars);
  int n_bytes = vsnprintf(NULL, 0, fmt, vars);
  *str = (char*)malloc(n_bytes);
  vsprintf(*str, fmt, vars_copy);
  va_end(vars);
  va_end(vars_copy);
  return 0;
}
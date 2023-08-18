#include "test_common.h"
#include "intern/common.h"

#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <math.h>

TEST(next_arg) {
  bool opt = false;
  int arglen = 0;
  char *test_str = "(a;a_order;)b;b_order;";
  char *str_itr = test_str;
  ASSERT_TRUE((str_itr = next_arg(test_str, ';', NULL, &arglen, &opt)) == (test_str + 1));
  ASSERT_TRUE(arglen == -1);
  ASSERT_TRUE(opt == true);
  
  ASSERT_TRUE((str_itr = next_arg(test_str + 1, ';', NULL, &arglen, &opt)) == (test_str + 3));
  ASSERT_TRUE(arglen == 1);
  ASSERT_TRUE(opt == true);
  
  ASSERT_TRUE((str_itr = next_arg(test_str + 3, ';', NULL, &arglen, &opt)) == (test_str + 12));
  ASSERT_TRUE(arglen == 7);
  ASSERT_TRUE(opt == false);
  
  ASSERT_TRUE((str_itr = next_arg(test_str + 12, ';', NULL, &arglen, &opt)) == (test_str + 14));
  ASSERT_TRUE(arglen == 1);
  ASSERT_TRUE(opt == false);
  
  ASSERT_TRUE((str_itr = next_arg(test_str + 14, ';', NULL, &arglen, &opt)) == NULL);
  ASSERT_TRUE(arglen == 7);
  ASSERT_TRUE(opt == false);
  
  return TEST_SUCCESS;
}

TEST(kwargs_unpack) {
  kwargs_t kwargs;
  double_t _b[] = {1.2, 3.2, -1.4};
  unsigned long int _b_order = 3;
  kwargs.argc = 2;
  char *arg_names[] = {"b", "b_order"};
  void *args[] = {(void*)_b, (void*)_b_order};
  kwargs.arg_names = arg_names;
  kwargs.args = args;

  double_t *a = NULL;
  double_t *b = NULL;
  unsigned long int a_order = 0;
  unsigned long int b_order = 0;
  int rc = kwargs_unpack(&kwargs, "(a;a_order;)b;b_order;", &a, &a_order, &b, &b_order);
  ASSERT_TRUE(rc == 0);
  ASSERT_TRUE(a == NULL);
  ASSERT_TRUE(a_order == 0);
  ASSERT_TRUE(b == _b);
  ASSERT_TRUE(b_order == 3);
  return TEST_SUCCESS;
}

int _node_even(hm_list_node_t *node, void *userdata) {
  struct test_node
  {
    HM_LIST_NODE_HEAD(struct test_node)
    int dat;
  };
  struct test_node *_node = (struct test_node*)node;
  return (_node->dat % 2 == 0) ? 0 : 1;
}

TEST(hm_list) {

  struct test_node
  {
    HM_LIST_NODE_HEAD(struct test_node)
    int dat;
  };

  struct test_list {
    HM_LIST_HEAD(struct test_node)
  };
  
  struct test_node *a, *b, *c, *d, *e;
  struct test_list *l;

  a = (struct test_node*)malloc(sizeof(struct test_node));
  b = (struct test_node*)malloc(sizeof(struct test_node));
  c = (struct test_node*)malloc(sizeof(struct test_node));
  d = (struct test_node*)malloc(sizeof(struct test_node));
  e = (struct test_node*)malloc(sizeof(struct test_node));
  l = (struct test_list*)malloc(sizeof(struct test_list));

  HM_LIST_NODE_INIT(a);
  HM_LIST_NODE_INIT(b);
  HM_LIST_NODE_INIT(c);
  HM_LIST_NODE_INIT(d);
  HM_LIST_NODE_INIT(e);
  HM_LIST_INIT(l);

  a->dat = 3;
  b->dat = 4;
  c->dat = 5;
  d->dat = 6;
  e->dat = 7;

  hm_list_append(l, a);
  hm_list_append(l, b);
  hm_list_insert(l, c, 1);
  hm_list_insert(l, d, 0);
  hm_list_insert(l, e, 0);
  hm_list_remove(l, 0, NULL);
  hm_list_remove(l, -1, NULL);
  hm_list_remove_where(l, _node_even, NULL, NULL);
  

  ASSERT_TRUE(l->n_items == 2);
  ASSERT_TRUE(l->head->dat == 3);
  ASSERT_TRUE(l->head->next->dat == 5);

  hm_list_delete(l, NULL);
  
  return TEST_SUCCESS;
}

TEST(hm_array) {
  struct test_array {
    HM_ARRAY_HEAD(int)
  };
  
  int buf1_raw[] = {-2,-1,0,1,2};
  int buf2_raw[] = {3,4,5,6};
  int buf_concat[] = {-2,-1,0,1,2,3,4,5,6};
  unsigned int buf1_items = sizeof(buf1_raw) / sizeof(int);
  unsigned int buf2_items = sizeof(buf2_raw) / sizeof(int);
  unsigned int buf_concat_items = sizeof(buf_concat) / sizeof(int);
  double tol = 1e-5;

  struct test_array *arr1 = (struct test_array*)malloc(sizeof(struct test_array));
  HM_ARRAY_INIT(arr1, int)
  arr1->buf = (int*)malloc(sizeof(int));
  hm_array_copy_raw(arr1, buf1_raw, buf1_items);
  struct test_array *arr2 = (struct test_array*)malloc(sizeof(struct test_array));
  HM_ARRAY_INIT(arr2, int)
  arr2->buf = (int*)malloc(sizeof(int));
  hm_array_copy_raw(arr2, buf2_raw, buf2_items);

  hm_array_concat(arr1, arr2);
  ASSERT_TRUE(arr1->n_items == buf_concat_items)
  ASSERT_TRUE(strncmp((char*)(arr1->buf), (char*)(buf_concat), buf_concat_items*sizeof(int)) == 0)

  hm_array_delete(arr1, NULL);
  hm_array_delete(arr2, NULL);

  return TEST_SUCCESS;
}
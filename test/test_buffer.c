#include <stddef.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdio.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "test_common.h"
#include "intern/buffer.h"

// helper functions
static inline void _set_rbuf_ptrs(buffer_t *rbuf, \
    unsigned int rpos, unsigned int wpos) {
  void *buf_start = rbuf->buf;
  rbuf->read_ptr = buf_start + rpos;
  rbuf->write_ptr = buf_start + wpos;
}

TEST(ring_buffer_init) {
  int rc;
  const unsigned int rbuf_n_bytes = 0x1000;
  buffer_t *rbuf_test = NULL;
  rc = buffer_init(&rbuf_test, rbuf_n_bytes, RING);
  ASSERT_TRUE(rc == 0);
  ASSERT_FALSE(rbuf_test == NULL);
  ASSERT_FALSE(rbuf_test->buf == NULL);
  ASSERT_TRUE(rbuf_test->buf_len_bytes == 0x1000);
  // tear down
  buffer_delete(&rbuf_test);
  return TEST_SUCCESS;
}

// TODO: try to access freed memory and fail the test if 
// SIGSEGV doesn't occur
TEST(buffer_delete) {
  //set up
  int rc;
  const unsigned int rbuf_n_bytes = 0x1000;
  buffer_t *rbuf_test = NULL;
  buffer_init(&rbuf_test, rbuf_n_bytes, RING);
  rc = buffer_delete(&rbuf_test);
  ASSERT_TRUE(rc == 0);
  ASSERT_TRUE(rbuf_test == NULL);
  return TEST_SUCCESS;
}

TEST(rbuf_reset) {
  // set up
  const unsigned int rbuf_n_bytes = 0x2000;
  buffer_t *rbuf_test = NULL;
  buffer_init(&rbuf_test, rbuf_n_bytes, RING);
  srand(time(NULL));
  unsigned int RBUF_READ_POS = rand() % rbuf_n_bytes;
  unsigned int RBUF_WRITE_POS = rand() % rbuf_n_bytes;
  _set_rbuf_ptrs(rbuf_test, RBUF_READ_POS, RBUF_WRITE_POS);
  if (RBUF_READ_POS != RBUF_WRITE_POS) {
    rbuf_test->state = NORMAL;
  }

  //test
  buffer_reset(rbuf_test);
  ASSERT_TRUE(rbuf_test->read_ptr == rbuf_test->buf);
  ASSERT_TRUE(rbuf_test->write_ptr == rbuf_test->buf);
  ASSERT_TRUE(rbuf_test->state == EMPTY);
  // tear down
  buffer_delete(&rbuf_test);
  return TEST_SUCCESS;
}

TEST(rbuf_rw_arithmetic) {
  //set up
  unsigned int RBUF_READ_POS, RBUF_WRITE_POS;
  unsigned int rbuf_n_bytes = 0x2000;
  buffer_t *rbuf_test = NULL;
  buffer_init(&rbuf_test, rbuf_n_bytes, RING);
  rbuf_test->state = NORMAL;
  srand(time(NULL));

  // case 1: read ptr < write_ptr
  RBUF_READ_POS = rand() % (rbuf_n_bytes - 1);
  RBUF_WRITE_POS = rand() % (rbuf_n_bytes - (RBUF_READ_POS + 1)) \
    + (RBUF_READ_POS + 1);
  _set_rbuf_ptrs(rbuf_test, RBUF_READ_POS, RBUF_WRITE_POS);
  ASSERT_TRUE(_rbuf_n_read_bytes(rbuf_test) == \
    (RBUF_WRITE_POS - RBUF_READ_POS));
  ASSERT_TRUE(_rbuf_n_write_bytes(rbuf_test) == \
    (rbuf_n_bytes + RBUF_READ_POS - RBUF_WRITE_POS));

  // case 2: read ptr > write_ptr
  RBUF_WRITE_POS = rand() % (rbuf_n_bytes - 1);
  RBUF_READ_POS = rand() % (rbuf_n_bytes - (RBUF_WRITE_POS + 1)) \
    + (RBUF_WRITE_POS + 1);
  _set_rbuf_ptrs(rbuf_test, RBUF_READ_POS, RBUF_WRITE_POS);
  ASSERT_TRUE(_rbuf_n_read_bytes(rbuf_test) == \
    (rbuf_n_bytes + RBUF_WRITE_POS - RBUF_READ_POS));
  ASSERT_TRUE(_rbuf_n_write_bytes(rbuf_test) == \
    (RBUF_READ_POS - RBUF_WRITE_POS));  

  // case 3: read ptr == write ptr (empty)
  RBUF_WRITE_POS = rand() % rbuf_n_bytes;
  RBUF_READ_POS = RBUF_WRITE_POS;
  _set_rbuf_ptrs(rbuf_test, RBUF_READ_POS, RBUF_WRITE_POS);
  rbuf_test->state = EMPTY;
  ASSERT_TRUE(_rbuf_n_read_bytes(rbuf_test) == 0);
  ASSERT_TRUE(_rbuf_n_write_bytes(rbuf_test) == rbuf_n_bytes);  

  // case 4: read_ptr == write_ptr (full)
  RBUF_WRITE_POS = rand() % rbuf_n_bytes;
  RBUF_READ_POS = RBUF_WRITE_POS;
  _set_rbuf_ptrs(rbuf_test, RBUF_READ_POS, RBUF_WRITE_POS);
  rbuf_test->state = FULL;
  ASSERT_TRUE(_rbuf_n_read_bytes(rbuf_test) == rbuf_n_bytes);
  ASSERT_TRUE(_rbuf_n_write_bytes(rbuf_test) == 0);  

  // tear down
  buffer_delete(&rbuf_test);
  return TEST_SUCCESS;
} 

// TODO: make this test more robust (different randomly-generated cases, etc.)
TEST(rbuf_write) {
  //set up
  buffer_t *rbuf_test = NULL;
  buffer_init(&rbuf_test, 5, RING);
  _set_rbuf_ptrs(rbuf_test, 2, 3);
  rbuf_test->state = NORMAL;
  int rc;
  const unsigned char data[5] = {1,2,3,4,5};
  rc = buffer_write(rbuf_test, data, 5);
  ASSERT_TRUE(rc == 4);
  const unsigned char rbuf_ref[5] = {3,4,0,1,2};
  ASSERT_TRUE(memcmp((char*)(rbuf_test->buf),rbuf_ref, 5) == 0);
  ASSERT_TRUE(rbuf_test->state == FULL);
  // tear down
  buffer_delete(&rbuf_test);
  return TEST_SUCCESS;
}

TEST(rbuf_read) {
  //set up
  buffer_t *rbuf_test = NULL;
  buffer_init(&rbuf_test, 5, RING);
  _set_rbuf_ptrs(rbuf_test, 3, 2);
  rbuf_test->state = NORMAL;
  int rc;
  const unsigned char rbuf_ref[5] = {3,4,0,1,2};
  memcpy(rbuf_test->buf, rbuf_ref, 5);
  //test
  char data_buf[4] = {'\0'};
  
  rc = buffer_read(rbuf_test, data_buf, 8);
  ASSERT_TRUE(rc == 4);
  const unsigned char data_ref[4] = {1,2,3,4};
  ASSERT_TRUE(memcmp(data_buf,data_ref, 4) == 0);
  ASSERT_TRUE(rbuf_test->state == EMPTY);

  //tear down
  buffer_delete(&rbuf_test);
  return TEST_SUCCESS;
}

TEST(buf_view) {
  //set up
  buffer_t *rbuf_test = NULL;
  buffer_init(&rbuf_test, 8, RING);
  _set_rbuf_ptrs(rbuf_test, 0, 6);
  rbuf_test->state = NORMAL;
  const unsigned char rbuf_ref[8] = {1,2,3,4,5,6,7,8};
  memcpy(rbuf_test->buf, rbuf_ref, 8);
  int rc;
  char data_buf[3] = {'\0'};

  rc = buffer_view(rbuf_test, data_buf, 2, 3);
  const unsigned char data_ref[3] = {3,4,5};
  ASSERT_TRUE(memcmp(data_buf, data_ref, 3) == 0);
  //tear down
  buffer_delete(&rbuf_test);
  return TEST_SUCCESS;
}
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#include "intern/io.h"
#include "test_common.h"
#include "intern/async.h"
#include "mock/mock_async.h"

TEST(src_fd) {
  // set up
  FILE *tmp_file = tmpfile();
  unsigned int n_bytes = 5;
  char test_data[] = "12345";
  fwrite(test_data, 1, n_bytes, tmp_file);
  rewind(tmp_file);
  int test_fd = fileno(tmp_file);

  // test
  int rc;
  char buf[5];
  ASSERT_TRUE(test_fd > 0);
  rc = src_fd(test_fd, n_bytes, buf);
  ASSERT_TRUE(rc == n_bytes);
  ASSERT_TRUE(memcmp(buf, test_data, n_bytes) == 0);

  // tear down
  fclose(tmp_file);
  return TEST_SUCCESS;
}

TEST(sink_fd) {
  // set up
  FILE *tmp_file = tmpfile();
  int test_fd = fileno(tmp_file);
  unsigned int n_bytes = 5;
  char test_data[] = "12345";

  // test
  int rc;
  rc = sink_fd(test_fd, n_bytes, test_data);
  ASSERT_TRUE(rc == n_bytes);
  rewind(tmp_file);
  char buf[5];
  fread(buf, 1, n_bytes, tmp_file);
  ASSERT_TRUE(memcmp(buf, test_data, n_bytes) == 0);

  // tear down
  fclose(tmp_file);
  return TEST_SUCCESS;
}

TEST(src_file) {
  // set up
  FILE *tmp_file = tmpfile();
  unsigned int n_bytes = 5;
  char test_data[] = "12345";
  fwrite(test_data, 1, n_bytes, tmp_file);
  rewind(tmp_file);

  // test
  int rc;
  char buf[5];
  rc = src_file(tmp_file, n_bytes, buf);
  ASSERT_TRUE(rc == n_bytes);
  ASSERT_TRUE(memcmp(buf, test_data, n_bytes) == 0);

  // tear down
  fclose(tmp_file);
  return TEST_SUCCESS;
}

TEST(sink_file) {
  // set up
  FILE *tmp_file = tmpfile();
  unsigned int n_bytes = 5;
  char test_data[] = "12345";

  // test
  int rc;
  rc = sink_file(tmp_file, n_bytes, test_data);
  ASSERT_TRUE(rc == n_bytes);
  rewind(tmp_file);
  char buf[5];
  fread(buf, 1, n_bytes, tmp_file);
  ASSERT_TRUE(memcmp(buf, test_data, n_bytes) == 0);

  // tear down
  fclose(tmp_file);
  return TEST_SUCCESS;
}

TEST(fd_async) {
  // setup
  unsigned int n_bytes = 5*sizeof(double_t);
  double_t input_data[5] = {1.1, 2.2, 3.3, 4.4, 5.5};
  double_t output_data_buf[5];
  double_t output_data_ref[5] = {4.4, 8.8, 13.2, 17.6, 22};
  FILE* tmp_src = tmpfile();
  FILE* tmp_dest = tmpfile();
  int fd_src = fileno(tmp_src);
  int fd_dest = fileno(tmp_dest);
  fwrite(input_data, 1, n_bytes, tmp_src);
  rewind(tmp_src);
  buffer_t *src = NULL;
  buffer_t *buf = NULL;
  buffer_t *dest = NULL;
  buffer_init(&src, n_bytes, RING);
  buffer_init(&buf, n_bytes, RING);
  buffer_init(&dest, n_bytes, RING);
  int rc;
  double_t tol = 1e-5;
  async_handle_t *h = NULL;
  async_loop_init(&h);
  async_loop_add_fn(h, src_fd_async(fd_src, src));
  async_loop_add_fn(h, times_two_async(src, buf));
  async_loop_add_fn(h, times_two_async(buf, dest));
  async_loop_add_fn(h, sink_fd_async(fd_dest, dest));
  async_loop_dispatch(h);
  struct stat s_dest;
  fstat(fd_dest, &s_dest);
  while(s_dest.st_size != n_bytes) {
    fstat(fd_dest, &s_dest);
  }
  async_loop_stop(h);
  rewind(tmp_dest);
  fread(output_data_buf, 1, n_bytes, tmp_dest);
  ASSERT_ALMOST_EQUAL_1D(output_data_buf, output_data_ref, 5, tol);

  // teardown
  fclose(tmp_src);
  fclose(tmp_dest);
  async_loop_delete(&h);
  buffer_delete(&src);
  buffer_delete(&buf);
  buffer_delete(&dest);  
  return TEST_SUCCESS;
}

TEST(file_async) {
  // setup
  unsigned int n_bytes = 5*sizeof(double_t);
  double_t input_data[5] = {1.1, 2.2, 3.3, 4.4, 5.5};
  double_t output_data_buf[5];
  double_t output_data_ref[5] = {4.4, 8.8, 13.2, 17.6, 22};
  FILE* tmp_src = tmpfile();
  FILE* tmp_dest = tmpfile();
  fwrite(input_data, 1, n_bytes, tmp_src);
  rewind(tmp_src);
  buffer_t *src = NULL;
  buffer_t *buf = NULL;
  buffer_t *dest = NULL;
  buffer_init(&src, n_bytes, RING);
  buffer_init(&buf, n_bytes, RING);
  buffer_init(&dest, n_bytes, RING);
  int rc;
  double_t tol = 1e-5;
  async_handle_t *h = NULL;
  async_loop_init(&h);
  async_loop_add_fn(h, src_file_async(tmp_src, src));
  async_loop_add_fn(h, times_two_async(src, buf));
  async_loop_add_fn(h, times_two_async(buf, dest));
  async_loop_add_fn(h, sink_file_async(tmp_dest, dest));
  async_loop_dispatch(h);
  struct stat s_dest;
  fstat(fileno(tmp_dest), &s_dest);
  while(s_dest.st_size != n_bytes) {
    fflush(tmp_dest);
    fstat(fileno(tmp_dest), &s_dest);
  }
  async_loop_stop(h);
  rewind(tmp_dest);
  fread(output_data_buf, 1, n_bytes, tmp_dest);
  ASSERT_ALMOST_EQUAL_1D(output_data_buf, output_data_ref, 5, tol);

  // teardown
  fclose(tmp_src);
  fclose(tmp_dest);
  async_loop_delete(&h);
  buffer_delete(&src);
  buffer_delete(&buf);
  buffer_delete(&dest);  
  return TEST_SUCCESS;
}
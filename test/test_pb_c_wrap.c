#include <stddef.h>

#include "hm_protobuf.h"
#include "test_common.h"

#ifdef __cplusplus
#error this file is not meant to be compiled as a c++ file
#endif

TEST(buffer_pb_create) {
  int rc;
  pb_buffer_t *pb_test = NULL;
  rc = _hm_pb_buffer_create(&pb_test);
  ASSERT_TRUE(rc == 0);
  ASSERT_TRUE(pb_test != NULL);
  rc = _hm_pb_buffer_delete(&pb_test);
  ASSERT_TRUE(rc == 0);
  ASSERT_TRUE(pb_test == NULL);
  return TEST_SUCCESS;
}
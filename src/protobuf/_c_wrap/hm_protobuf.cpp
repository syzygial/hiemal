#include <string>
#include <fstream>
#include <algorithm>

#include <google/protobuf/repeated_ptr_field.h>

#include "hm_protobuf.h"

#include "buffer.pb.h"

// create C entry point for interacting with protocol buffers
extern "C" {

int _hm_pb_buffer_create(pb_buffer_t **buf_pb) {
  hiemal::buffer *buf = new hiemal::buffer;
  *buf_pb = (pb_buffer_t*)buf;
  return 0;
}

int _hm_pb_buffer_delete(pb_buffer_t **buf_pb) {
  hiemal::buffer *buf = (hiemal::buffer*)(*buf_pb);
  delete buf;
  *buf_pb = NULL;
  return 0;
}

int _hm_pb_buffer_write(pb_buffer_t *buf_pb, const void *data, unsigned int n_bytes) {
  hiemal::buffer *buf = (hiemal::buffer*)buf_pb;
  hiemal::buf_snapshot *s = new hiemal::buf_snapshot;
  std::string data_str((char*)data, n_bytes);
  s->set_raw_data(data_str);
  google::protobuf::RepeatedPtrField<hiemal::buf_snapshot>* \
    buf_data_ptr = buf->mutable_buf_data();
  buf_data_ptr->Add(std::move(*s));
  delete s;
  return 0;
}

int _hm_pb_buffer_save(pb_buffer_t *buf_pb, const char *filename) {
  hiemal::buffer *buf = (hiemal::buffer*)buf_pb;
  std::ofstream os(filename, std::ios::binary);
  buf->SerializeToOstream(&os);
  return 0;
}

} // extern "C"

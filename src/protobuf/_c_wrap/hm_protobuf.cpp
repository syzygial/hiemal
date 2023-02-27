#include <string>
#include <fstream>
#include <algorithm>

#include <google/protobuf/repeated_ptr_field.h>

#include "hm_protobuf.h"

#include "buffer.pb.h"

// create C entry point for interacting with protocol buffers
extern "C" {

int _hm_pb_buflist_create(pb_buflist_t **buflist_pb) {
  hiemal::buf_list *buflist = new hiemal::buf_list;
  *buflist_pb = (pb_buflist_t*)buflist;
  return 0;
}

int _hm_pb_buflist_delete(pb_buflist_t **buflist_pb) {
  hiemal::buf_list *buflist = (hiemal::buf_list*)(*buflist_pb);
  delete buflist;
  *buflist_pb = NULL;
  return 0;
}

int _hm_pb_buflist_add_buffer(pb_buflist_t *buflist_pb) {
  hiemal::buf_list *buflist = (hiemal::buf_list*)buflist_pb;
  google::protobuf::RepeatedPtrField<hiemal::buffer>* \
    buflist_ptr = buflist->mutable_buffers();
    buflist_ptr->Add();
  return 0;
}

int _hm_pb_buffer_write(pb_buflist_t *buflist_pb, unsigned int r_id, \
    const void *data, unsigned int n_bytes) {
  hiemal::buf_list *buflist = (hiemal::buf_list*)buflist_pb;
  hiemal::buffer *buf = buflist->mutable_buffers(r_id);
  std::string data_str((char*)data, n_bytes);
  google::protobuf::RepeatedPtrField<hiemal::buf_snapshot>* \
    buf_data_ptr = buf->mutable_buf_data();
  hiemal::buf_snapshot *new_snapshot = buf_data_ptr->Add();
  new_snapshot->set_raw_data(data_str);
  return 0;
}

int _hm_pb_buflist_save(pb_buflist_t *buflist_pb, const char *filename) {
  hiemal::buf_list *buflist = (hiemal::buf_list*)buflist_pb;
  std::ofstream f(filename, std::ios::binary);
  buflist->SerializeToOstream(&f);
  return 0;
}

} // extern "C"

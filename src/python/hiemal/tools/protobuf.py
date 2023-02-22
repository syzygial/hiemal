from hiemal.backend.protobuf import buffer_pb2
import numpy as np

def buffer_pb_info(buf):
  if isinstance(buf, str):
    with open(buf, 'rb') as f:
      buf = buffer_pb2.buffer()
      buf.ParseFromString(f.read())
  fs = buf.metadata.fs
  bit_depth = buf.metadata.bit_depth
  buf_size = buf.metadata.buf_size
  print("Sampling frequency (Hz): {}".format(fs))
  print("Bit depth (bits/sample): {}".format(bit_depth))
  print("Buffer size (bytes): {}".format(buf_size))
  print("")

  for snapshot in buf.buf_data:
    write_time = snapshot.rel_time
    data = np.frombuffer(snapshot.raw_data, dtype=np.uint8)
    print("Write ({:.3f}s, {} bytes):".format(write_time, len(data)))
    repr(data)
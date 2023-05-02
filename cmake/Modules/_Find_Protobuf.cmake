# WIP

include(FindPackageHandleStandardArgs)
find_program(Protobuf_PROTOC_EXECUTABLE protoc)
find_package_handle_standard_args(Protobuf DEFAULT_MSG PROTOC_EXECUTABLE)
get_filename_component(Protobuf_ROOT ${Protobuf_PROTOC_EXECUTABLE}/../.. ABSOLUTE)
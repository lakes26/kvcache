load("@rules_proto//proto:defs.bzl", "proto_library")

cc_library(
    name = "kv_proto",
    deps = [":kv_proto_cc_proto_lib"],
    visibility = ["//visibility:public"],
)

cc_proto_library(
    name = "kv_proto_cc_proto_lib",
    deps = [":kv_proto_proto_lib"],
)

proto_library(
  name = "kv_proto_proto_lib",
  srcs = ["kv.proto"],
)

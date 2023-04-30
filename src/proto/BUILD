load("@rules_proto//proto:defs.bzl", "proto_library")
load("@rules_cc//cc:defs.bzl", "cc_proto_library")
load("@com_github_grpc_grpc//bazel:cc_grpc_library.bzl", "cc_grpc_library")

cc_grpc_library(
    name = "kv_cc_grpc",
    srcs = [":kv_proto"],
    grpc_only = True,
    deps = [":kv_cc_proto"],
    visibility = ['//visibility:public'],
)

cc_proto_library(
    name = "kv_cc_proto",
    deps = [":kv_proto"],
)

proto_library(
  name = "kv_proto",
  srcs = ["kv.proto"],
)

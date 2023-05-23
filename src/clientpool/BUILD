
cc_library(
  name = "clientpool",
  visibility = ["//visibility:public"],
  srcs = [
    "clientpool.cc",
    "clientpool.h",
  ],
  deps = [
    "//src/shardmap:shardmap",
    "@com_google_absl//absl/synchronization",
    "//src/proto:kv_cc_grpc",
    "@com_github_grpc_grpc//:grpc++",
  ]
)


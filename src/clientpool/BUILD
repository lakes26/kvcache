
cc_library(
  name = "clientpool",
  visibility = ["//visibility:public"],
  srcs = [
    "clientpool.cc",
    "clientpool.h",
  ],
  deps = [
    "//shardmap:shardmap",
    "@com_google_absl//absl/synchronization",
    "//proto:kv_cc_grpc",
    "@com_github_grpc_grpc//:grpc++",
  ]
)



cc_library(
  name = "shardmap",
  srcs = [
    "shardmap.cc",
    "file_shardmap.cc",
  ],
  hdrs = [
    "shardmap.h"
  ],
  visibility = ["//visibility:public"],
  deps = [
      "//proto:kv_cc_grpc",
      "@com_github_grpc_grpc//:grpc++",
  ],
)


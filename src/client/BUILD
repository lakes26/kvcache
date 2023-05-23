
cc_library(
  name = "client",
  visibility = ["//visibility:public"],
  srcs = [
    "client.cc",
  ],
  hdrs = [
    "client.h",
  ],
  deps = [
      "//src/proto:kv_cc_grpc",
      "@com_github_grpc_grpc//:grpc++",
      "//src/util:util",
      "//src/shardmap:shardmap",
      "//src/clientpool:clientpool",
      "@com_github_google_glog//:glog",
  ],
)


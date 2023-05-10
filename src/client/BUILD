
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
      "//proto:kv_cc_grpc",
      "@com_github_grpc_grpc//:grpc++",
      "//util:util",
      "//shardmap:shardmap",
      "//clientpool:clientpool",
      "@com_github_google_glog//:glog",
  ],
)


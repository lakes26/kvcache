load("@io_bazel_rules_docker//cc:image.bzl", "cc_image")

cc_library(
  name = "server",
  visibility = ["//visibility:public"],
  srcs = [
    "server.cc",
    "shard.cc",
    "shard.h",
  ],
  hdrs = [
    "server.h",
  ],
  deps = [
      "//src/proto:kv_cc_grpc",
      "@com_github_grpc_grpc//:grpc++",
      "@com_google_absl//absl/synchronization",
      "@com_google_absl//absl/time",
      "//src/shardmap:shardmap",
      "//src/util:util",
  ],
)

cc_image(
   name = "server_image",
   binary = "server",
)

load("@io_bazel_rules_docker//cc:image.bzl", "cc_image")

cc_binary(
  name = "server",
  srcs = ["server.cc"],
  deps = [
      "//proto:kv_cc_grpc",
      "@com_github_grpc_grpc//:grpc++",
  ],
)

cc_image(
   name = "server_image",
   binary = ":server",
)

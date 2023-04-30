cc_binary(
  name = "server",
  srcs = ["server.cc"],
  defines = ["BAZEL_BUILD"],
  deps = [
      "//proto:kv_cc_grpc",
      "@com_github_grpc_grpc//:grpc++",
  ],
  copts = ["-std=c++20"],
)



cc_test(
  name = "test",
  visibility = ["//visibility:public"],
  srcs = [
    "test_setup.cc",
    "test_setup.h",
  ],
  deps = [
      "@com_github_grpc_grpc//:grpc++",
      "//shardmap:shardmap",
      "//server:server",
      "//client:client",
      "//clientpool:clientpool",
      "@com_google_googletest//:gtest_main",
  ],
)

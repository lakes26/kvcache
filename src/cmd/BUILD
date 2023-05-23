
cc_binary(
  name = "client",
  srcs = [
    "client.cc",
  ],
  deps = [
      "//src/client:client",
      "@com_google_absl//absl/flags:flag",
      "@com_google_absl//absl/flags:parse",
      "@com_github_google_glog//:glog",
      "//src/shardmap:shardmap",
      "//src/clientpool:clientpool",
  ],
)

cc_binary(
  name = "server",
  srcs = [
    "server.cc",
  ],
  deps = [
      "//src/server:server",
      "@com_google_absl//absl/flags:flag",
      "@com_google_absl//absl/flags:parse",
      "@com_github_grpc_grpc//:grpc++",
      "@com_github_google_glog//:glog",
      "@com_github_nlohmann_json//:json",
  ],
)


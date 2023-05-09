#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "server/server.h"
#include <grpc++/grpc++.h>
#include <glog/logging.h>
#include <string>
#include "nlohmann/json.hpp"

ABSL_FLAG(std::string, shardmap, "",
          "Path to a JSON file which describes the shard map");
ABSL_FLAG(std::string, node, "",
          "Name of the node (must match in shard map file)");

void RunServer() {
  
  std::string shardMap = absl::GetFlag(FLAGS_shardmap);
  std::string nodeName = absl::GetFlag(FLAGS_node);

  if (shardMap.length() == 0 || nodeName.length() == 0) {
    LOG(FATAL) << "--shardmap and --node are required" << std::endl;
  }



  std::string server_address("0.0.0.0:50051");
  KvServerImpl service = KvServerImpl(NULL);
  grpc::ServerBuilder builder;

  // Listen on the given address without any authentication mechanism.
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  // Register "service" as the instance through which we'll communicate with
  // clients. In this case it corresponds to an *synchronous* service.
  builder.RegisterService(&service);
  // Finally assemble the server.
  std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;
  // Wait for the server to shutdown. Note that some other thread must be
  // responsible for shutting down the server for this call to ever return.
  server->Wait();
}

int main(int argc, char** argv) {
  absl::ParseCommandLine(argc, argv);
  google::InitGoogleLogging(argv[0]);
  google::InstallFailureSignalHandler();
  RunServer();
  return 0;
}

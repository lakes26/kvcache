#include <csignal>
#include <fstream>
#include <streambuf>
#include <iostream>
#include <string>

#include <grpc++/grpc++.h>
#include <glog/logging.h>

#include "src/shardmap/shardmap.h"
#include "src/server/server.h"
#include "absl/flags/parse.h"
#include "absl/flags/flag.h"

ABSL_FLAG(std::string, shardmap, "",
          "Path to a JSON file which describes the shard map");
ABSL_FLAG(std::string, node, "",
          "Name of the node (must match in shard map file)");

std::unique_ptr<grpc::Server> server;

void RunServer() {
  
  std::string shardmap_filename = absl::GetFlag(FLAGS_shardmap);
  std::string nodeName = absl::GetFlag(FLAGS_node);

  // validate presence of flags
  if (shardmap_filename.length() == 0 || nodeName.length() == 0) {
    LOG(FATAL) << "--shardmap and --node are required" << std::endl;
  }
 
  // create shard map object from file be deserializing json
  FileShardMap fileSm = FileShardMap(shardmap_filename);
  fileSm.applyFromFile(); 

  //check if nodename is in the map
  auto nodeMap = fileSm.shardMap.nodes();
  auto it = nodeMap.find(nodeName);
  if (it == nodeMap.end()) {
    LOG(FATAL) << "--node must be in the shard map" << std::endl;
  }
  
  // construct server address from the node info
  NodeInfo nodeInfo = it->second; 
  std::string server_address = nodeInfo.address + ":" + std::to_string(nodeInfo.port);

  KvServerImpl service = KvServerImpl(nodeName, &fileSm.shardMap);

  grpc::ServerBuilder builder;
  // Listen on the given address without any authentication mechanism.
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());

  // Register "service" as the instance through which we'll communicate with
  // clients. In this case it corresponds to an *synchronous* service.
  builder.RegisterService(&service);

  // Finally assemble the server.
  server = builder.BuildAndStart();
  LOG(INFO) << "Server listening on " << server_address << std::endl;

  // Wait for the server to shutdown. Note that some other thread must be
  // responsible for shutting down the server for this call to ever return.
  server->Wait();
}

void signalHandler(int signum) {
  server->Shutdown();
}

int main(int argc, char** argv) {
  absl::ParseCommandLine(argc, argv);
  google::InitGoogleLogging(argv[0]);
  google::InstallFailureSignalHandler();
  signal(SIGINT, signalHandler);

  RunServer();
  return 0;
}


#include <iostream>
#include <memory>
#include <string>

#include <grpc++/grpc++.h>

#ifdef BAZEL_BUILD
#include "proto/kv.grpc.pb.h"
#else
#include "proto/kv.grpc.pb.h"
#endif

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using kv::GetRequest;
using kv::GetResponse;
using kv::SetRequest;
using kv::SetResponse;
using kv::DeleteRequest;
using kv::DeleteResponse;
using kv::GetShardContentsRequest;
using kv::GetShardContentsResponse;
using kv::Kv;

class KvServerImpl final : public Kv::Service {
  
  Status Get(
    ServerContext* context,
    const GetRequest* request,
    GetResponse* response
  ) override {
    return Status::OK;
  }
 
  Status Set(
    ServerContext* context,
    const SetRequest* request,
    SetResponse* response
  ) override {
    return Status::OK;
  }

  Status Delete(
    ServerContext* context,
    const DeleteRequest* request,
    DeleteResponse* response
  ) override {
    return Status::OK;
  }
  
  Status GetShardContents(
    ServerContext* context,
    const GetShardContentsRequest* request,
    GetShardContentsResponse* response
  ) override {
    return Status::OK;
  }
};
 
void RunServer() {
  std::string server_address("0.0.0.0:50051");
  KvServerImpl service;
  ServerBuilder builder;

  // Listen on the given address without any authentication mechanism.
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  // Register "service" as the instance through which we'll communicate with
  // clients. In this case it corresponds to an *synchronous* service.
  builder.RegisterService(&service);
  // Finally assemble the server.
  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;
  // Wait for the server to shutdown. Note that some other thread must be
  // responsible for shutting down the server for this call to ever return.
  server->Wait();
}

int main(int argc, char** argv) {
  RunServer();
  return 0;
}

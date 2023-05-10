#include <iostream>

#include <grpc++/grpc++.h>
#include "util/util.h"
#include "absl/synchronization/mutex.h"

#ifdef BAZEL_BUILD
#include "proto/kv.grpc.pb.h"
#include "proto/kv.pb.h"
#else
#include "proto/kv.grpc.pb.h"
#include "proto/kv.pb.h"
#endif

#include "client.h"

#include "shardmap/shardmap.h"
#include "clientpool/clientpool.h"

using kv::GetRequest;
using kv::GetResponse;

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using grpc::Channel;
using grpc::ClientContext;

using kv::GetRequest;
using kv::GetResponse;
using kv::SetRequest;
using kv::SetResponse;
using kv::DeleteRequest;
using kv::DeleteResponse;
using kv::Kv;


KvClient::KvClient(ShardMap* shardMap, ClientPool* clientPool) {
  this->shardMap = shardMap;
  this->clientPool = clientPool;
}

std::string KvClient::Get(const std::string key) {
  GetRequest request;
  request.set_key(key);
  GetResponse response;
  ClientContext context;
  return "test_response";
}

void KvClient::Set(const std::string key, const std::string value, absl::Duration ttl) {
;
}

void KvClient::Delete(const std::string key) {
;
}

/*
int main(int argc, char** argv) {

  auto channel = grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials());
  KvClient client = KvClient(NULL, NULL);
  client.Get("test");

  std::cout << GetShardForKey("this is the key", 300) << "\n";

  return 0;
}
*/



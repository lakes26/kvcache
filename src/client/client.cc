#include <iostream>
#include <stdexcept>

#include <grpc++/grpc++.h>
#include "util/util.h"
#include "absl/synchronization/mutex.h"

#include "proto/kv.grpc.pb.h"
#include "proto/kv.pb.h"

#include "client.h"

#include "shardmap/shardmap.h"
#include "clientpool/clientpool.h"

#include <algorithm>
#include <random>
#include <vector>

#include <glog/logging.h>


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

  std::random_device rd;
  std::mt19937 g(rd());

  int assignedShardId = GetShardForKey(key, this->shardMap->numShards());
  std::vector<std::string> nodes = this->shardMap->nodesForShard(assignedShardId);

  std::shuffle(nodes.begin(), nodes.end(), g);

  if(nodes.size() == 0) {
    LOG(ERROR) << "no nodes hosing shard: " << std::to_string(assignedShardId) << std::endl;
  }

  bool wasFound = false;
  std::string retString = "";
  for (const auto& node : nodes) {

    // get stub from the client pool
    std::shared_ptr<Kv::Stub> stub;
    try {
      stub = this->clientPool->GetClient(node);
    } catch (std::exception e) {
      LOG(ERROR) << "Clientpool unable to return a stub" << e.what() << std::endl; 
      continue;
    }

    // make grpc call using the stub
    Status status;
    try {
      status = stub->Get(&context, request, &response);
    } catch (std::exception e) {
      LOG(ERROR) << "GRPC call failed: " << e.what() << std::endl; 
      continue;
    }

    if (status.ok()) {
      if (response.was_found()) {
        wasFound = true;
        retString = response.value();
        break;
      }
    }
  }
  if(!wasFound) {
    throw std::runtime_error("The value in question was not found");
  }

  return retString;
}

void KvClient::Set(const std::string key, const std::string value, absl::Duration ttl) {
  SetRequest request;
  request.set_key(key);
  request.set_value(value);

  int64_t ttlMs = ttl / absl::Milliseconds(1);
  request.set_ttl_ms(ttlMs);

  SetResponse response;
  ClientContext context;

  int assignedShardId = GetShardForKey(key, this->shardMap->numShards());
  std::vector<std::string> nodes = this->shardMap->nodesForShard(assignedShardId);

  if(nodes.size() == 0) {
    LOG(ERROR) << "no nodes hosing shard: " << std::to_string(assignedShardId) << std::endl;
  }

  grpc::CompletionQueue cq;

  std::vector<grpc::ClientContext> clientContexts(nodes.size());
  std::vector<std::shared_ptr<Kv::Stub>> clients(nodes.size());
  std::vector<std::unique_ptr<grpc::ClientAsyncResponseReader<kv::SetResponse>>> rpcs(nodes.size());
  std::vector<grpc::Status> statuses(nodes.size());
  std::vector<kv::SetResponse> responses(nodes.size());

  for (int64_t i = 0; i < nodes.size(); i++) {
    clients[i] = this->clientPool->GetClient(nodes[i]);
    rpcs[i] = clients[i]->PrepareAsyncSet(&clientContexts[i], request, &cq);
    rpcs[i]->StartCall();
    rpcs[i]->Finish(&responses[i], &statuses[i], (void *) i);
  }

  for (int64_t i = 0; i < nodes.size(); i++) {
    void* got_tag;
    bool ok = false;
    cq.Next(&got_tag, &ok);
    if (ok) {
      int64_t id = (int64_t) got_tag;
      if  (statuses[id].ok()) {
        std::cout << "call: " << std::to_string(id) << " recieved status ok from server" << std::endl;
      }
    }

  }

}

void KvClient::Delete(const std::string key) {
  DeleteRequest request;
  request.set_key(key);
  ClientContext context;

  int assignedShardId = GetShardForKey(key, this->shardMap->numShards());
  std::vector<std::string> nodes = this->shardMap->nodesForShard(assignedShardId);

  if(nodes.size() == 0) {
    LOG(ERROR) << "no nodes hosing shard: " << std::to_string(assignedShardId) << std::endl;
  }

  grpc::CompletionQueue cq;

  std::vector<grpc::ClientContext> clientContexts(nodes.size());
  std::vector<std::shared_ptr<Kv::Stub>> clients(nodes.size());
  std::vector<std::unique_ptr<grpc::ClientAsyncResponseReader<kv::DeleteResponse>>> rpcs(nodes.size());
  std::vector<grpc::Status> statuses(nodes.size());
  std::vector<kv::DeleteResponse> responses(nodes.size());

  for (int64_t i = 0; i < nodes.size(); i++) {
    clients[i] = this->clientPool->GetClient(nodes[i]);
    rpcs[i] = clients[i]->PrepareAsyncDelete(&clientContexts[i], request, &cq);
    rpcs[i]->StartCall();
    rpcs[i]->Finish(&responses[i], &statuses[i], (void *) i);
  }

  for (int64_t i = 0; i < nodes.size(); i++) {
    void* got_tag;
    bool ok = false;
    cq.Next(&got_tag, &ok);
    if (ok) {
      int64_t id = (int64_t) got_tag;
      if  (statuses[id].ok()) {
        std::cout << "call: " << std::to_string(id) << " recieved status ok from server" << std::endl;
      }
    }
  }
}


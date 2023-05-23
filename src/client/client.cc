#include "client.h"

#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <random>
#include <vector>
#include <tuple>

#include <grpc++/grpc++.h>
#include <glog/logging.h>

#include "absl/synchronization/mutex.h"
#include "src/clientpool/clientpool.h"
#include "src/shardmap/shardmap.h"
#include "src/proto/kv.grpc.pb.h"
#include "src/proto/kv.pb.h"
#include "src/util/util.h"

namespace kvclient {

KvClient::KvClient(ShardMap* shardMap, ClientPool* clientPool) {
  this->shardMap = shardMap;
  this->clientPool = clientPool;
}

std::tuple<std::string, bool, grpc::Status> KvClient::Get(const std::string key) {
  kv::GetRequest request;
  request.set_key(key);
  kv::GetResponse response;
  grpc::ClientContext context;

  std::random_device rd;
  std::mt19937 g(rd());

  int assignedShardId = GetShardForKey(key, this->shardMap->numShards());
  std::vector<std::string> nodes = this->shardMap->nodesForShard(assignedShardId);

  std::shuffle(nodes.begin(), nodes.end(), g);

  if(nodes.size() == 0) {
    LOG(ERROR) << "no nodes hosing shard: " << std::to_string(assignedShardId) << std::endl;
    return std::make_tuple("", false, grpc::Status(grpc::NOT_FOUND, "no nodes hosting that shard"));
  }

  grpc::Status lastErr;
  grpc::Status status;
  for (const auto& node : nodes) {

    // get stub from the client pool
    std::shared_ptr<kv::Kv::Stub> stub;
    try {
      stub = this->clientPool->GetClient(node);
    } catch (std::exception e) {
      LOG(ERROR) << "Clientpool unable to return a stub" << e.what() << std::endl; 
      continue;
    }

    // make grpc call using the stub
    status = stub->Get(&context, request, &response);
    if (status.ok()) {
      return std::make_tuple(response.value(), response.was_found(), grpc::Status::OK);
    } else {
      lastErr = status;
    }
  }
  return std::make_tuple("", false, lastErr);
}

grpc::Status KvClient::Set(const std::string key, const std::string value, absl::Duration ttl) {
  kv::SetRequest request;
  request.set_key(key);
  request.set_value(value);

  int64_t ttlMs = ttl / absl::Milliseconds(1);
  request.set_ttl_ms(ttlMs);

  kv::SetResponse response;
  grpc::ClientContext context;

  int assignedShardId = GetShardForKey(key, this->shardMap->numShards());
  std::vector<std::string> nodes = this->shardMap->nodesForShard(assignedShardId);

  if(nodes.size() == 0) {
    LOG(ERROR) << "no nodes hosing shard: " << std::to_string(assignedShardId) << std::endl;
  }

  grpc::CompletionQueue cq;

  std::vector<grpc::ClientContext> clientContexts(nodes.size());
  std::vector<std::shared_ptr<kv::Kv::Stub>> clients(nodes.size());
  std::vector<std::unique_ptr<grpc::ClientAsyncResponseReader<kv::SetResponse>>> rpcs(nodes.size());
  std::vector<grpc::Status> statuses(nodes.size());
  std::vector<kv::SetResponse> responses(nodes.size());

  for (int64_t i = 0; i < nodes.size(); i++) {
    clients[i] = this->clientPool->GetClient(nodes[i]);
    rpcs[i] = clients[i]->PrepareAsyncSet(&clientContexts[i], request, &cq);
    rpcs[i]->StartCall();
    rpcs[i]->Finish(&responses[i], &statuses[i], (void *) i);
  }

  grpc::Status status = grpc::Status::OK;
  for (int64_t i = 0; i < nodes.size(); i++) {
    void* got_tag;
    bool ok = false;
    cq.Next(&got_tag, &ok);
    if (ok) {
      int64_t id = (int64_t) got_tag;
      if  (!statuses[id].ok()) {
        status = statuses[id];
      }
    }
  }
  return status;
}

grpc::Status KvClient::Delete(const std::string key) {
  kv::DeleteRequest request;
  request.set_key(key);
  grpc::ClientContext context;

  int assignedShardId = GetShardForKey(key, this->shardMap->numShards());
  std::vector<std::string> nodes = this->shardMap->nodesForShard(assignedShardId);

  if(nodes.size() == 0) {
    LOG(ERROR) << "no nodes hosing shard: " << std::to_string(assignedShardId) << std::endl;
  }

  grpc::CompletionQueue cq;

  std::vector<grpc::ClientContext> clientContexts(nodes.size());
  std::vector<std::shared_ptr<kv::Kv::Stub>> clients(nodes.size());
  std::vector<std::unique_ptr<grpc::ClientAsyncResponseReader<kv::DeleteResponse>>> rpcs(nodes.size());
  std::vector<grpc::Status> statuses(nodes.size());
  std::vector<kv::DeleteResponse> responses(nodes.size());

  for (int64_t i = 0; i < nodes.size(); i++) {
    clients[i] = this->clientPool->GetClient(nodes[i]);
    rpcs[i] = clients[i]->PrepareAsyncDelete(&clientContexts[i], request, &cq);
    rpcs[i]->StartCall();
    rpcs[i]->Finish(&responses[i], &statuses[i], (void *) i);
  }

  grpc::Status status = grpc::Status::OK;
  for (int64_t i = 0; i < nodes.size(); i++) {
    void* got_tag;
    bool ok = false;
    cq.Next(&got_tag, &ok);
    if (ok) {
      int64_t id = (int64_t) got_tag;
      if  (!statuses[id].ok()) {
        status = statuses[id];
      }
    }
  }
  return status;
}

} // namespace kvclient

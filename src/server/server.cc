
#include <grpc++/grpc++.h>
#include "proto/kv.grpc.pb.h"

#include <vector>
#include <string>
#include <tuple>
#include <atomic>
#include <thread>

#include "absl/time/time.h"
#include "util/util.h"
#include "shardmap/shardmap.h"
#include "server.h"
#include "shard.h"

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


KvServerImpl::KvServerImpl(std::string nodeName, ShardMap* shardMap) {
  this->nodeName = nodeName;
  this->shardMap = shardMap;
  this->shards = std::vector<KvShard>(shardMap->numShards());
  this->isRunning.store(true);

  this->janitor = std::thread(&KvServerImpl::ttlJanitor, this);
}

KvServerImpl::~KvServerImpl() {
  this->isRunning.store(false);
  this->janitor.join();
  std::cout << "Janitor Safely Ended" << std::endl;
}
 
// Get Section -------------------------------
Status KvServerImpl::Get(
  ServerContext* context,
  const GetRequest* request,
  GetResponse* response
) {
  if (request->key() == "") {
    return Status(grpc::INVALID_ARGUMENT, "The empty key is not allowed");
  }

  int id = GetShardForKey(request->key(), shardMap->numShards());
  if (!isShardHosted(id)) {
    return Status(grpc::NOT_FOUND, "The shard is not hosted on this node");
  }

  auto tup = this->shards[id].Get(request->key());
  response->set_value(std::get<0>(tup));
  response->set_was_found(std::get<1>(tup));

  return Status::OK;
}

// SET Section -------------------------------
Status KvServerImpl::Set(
  ServerContext* context,
  const SetRequest* request,
  SetResponse* response
) {
  if (request->key() == "") {
    return Status(grpc::INVALID_ARGUMENT, "The empty key is not allowed");
  }

  int id = GetShardForKey(request->key(), shardMap->numShards());
  if (!isShardHosted(id)) {
    return Status(grpc::NOT_FOUND, "The shard is not hosted on this node");
  }

  this->shards[id].Set(request->key(), request->value(), request->ttl_ms());

  return Status::OK;
}


// DELETE Section -----------------------------
Status KvServerImpl::Delete(
  ServerContext* context,
  const DeleteRequest* request,
  DeleteResponse* response
) {

  if (request->key() == "") {
    return Status(grpc::INVALID_ARGUMENT, "The empty key is not allowed");
  }

  int id = GetShardForKey(request->key(), shardMap->numShards());
  if (!isShardHosted(id)) {
    return Status(grpc::NOT_FOUND, "The shard is not hosted on this node");
  }

  this->shards[id].Delete(request->key());
  return Status::OK;
}

Status KvServerImpl::GetShardContents(
  ServerContext* context,
  const GetShardContentsRequest* request,
  GetShardContentsResponse* response
) {
  return Status::OK;
}

// TTL Section ------------------------------ 

void KvServerImpl::ttlJanitor() {
  absl::Time timeToClean = absl::Now();
  while(this->isRunning.load() == true) {
    if (absl::Now() > timeToClean) {
      for (int i = 0; i < this->shardMap->numShards(); i++) {
        this->shards[i].Clean();
      }
      timeToClean = absl::Now();
    }
    absl::SleepFor(absl::Milliseconds(10));
  }
}

bool KvServerImpl::isShardHosted(int id) {
  std::vector<int> v = this->shardMap->shardsForNode(this->nodeName);
  if (std::find(v.begin(), v.end(), id) != v.end()) {
    return true;
  } else {
    return false;
  }
}


 

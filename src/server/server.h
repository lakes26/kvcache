#ifndef KV_SERVER_IMPL_H
#define KV_SERVER_IMPL_H

#include "shardmap/shardmap.h"
#include "absl/synchronization/mutex.h"
#include "shard.h"
#include <grpc++/grpc++.h>
#include "proto/kv.grpc.pb.h"
#include <string>
#include <vector>
#include <atomic>
#include <thread>

class KvServerImpl final : public kv::Kv::Service {
private:
    std::string nodeName;
    ShardMap*   shardMap;

    std::vector<KvShard> shards;
    absl::Mutex mu;

    std::atomic<bool> isRunning;
    std::thread janitor;

    void ttlJanitor();
    bool isShardHosted(int id);

public: 
  KvServerImpl(std::string nodeName, ShardMap* shardMap);
  ~KvServerImpl();
  grpc::Status Get( grpc::ServerContext* context, const kv::GetRequest* request, kv::GetResponse* response) override; 
  grpc::Status Set( grpc::ServerContext* context, const kv::SetRequest* request, kv::SetResponse* response) override;
  grpc::Status Delete( grpc::ServerContext* context, const kv::DeleteRequest* request, kv::DeleteResponse* response) override;  
  grpc::Status GetShardContents( grpc::ServerContext* context, const kv::GetShardContentsRequest* request, kv::GetShardContentsResponse* response) override;
};

#endif

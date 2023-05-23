#ifndef SRC_CLIENT_CLIENT_H
#define SRC_CLIENT_CLIENT_H

#include <grpc++/grpc++.h>
#include "absl/time/time.h"

#include <string.h>

#include "src/clientpool/clientpool.h"
#include "src/shardmap/shardmap.h"

namespace kvclient {
  
class KvClient {
 public:
  KvClient(ShardMap* shardMap, ClientPool* clientPool);

  std::tuple<std::string, bool, grpc::Status> Get(const std::string key);
  grpc::Status Set(const std::string key, const std::string value, absl::Duration ttl);
  grpc::Status Delete(const std::string key);

 private:
  ShardMap* shardMap;
  ClientPool* clientPool;
};

} //namespace kvclient

#endif

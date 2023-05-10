#ifndef KVCLIENT_H
#define KVCLIENT_H

#include <grpc++/grpc++.h>
#include "absl/time/time.h"

#include <string.h>

#include "clientpool/clientpool.h"
#include "shardmap/shardmap.h"


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

#endif

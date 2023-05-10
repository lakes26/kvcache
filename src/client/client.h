#ifndef KVCLIENT_H
#define KVCLIENT_H

#include "absl/time/time.h"
#include "shardmap/shardmap.h"
#include "clientpool/clientpool.h"

#include <string.h>

class KvClient {
public:
    KvClient(ShardMap* shardMap, ClientPool* clientPool);
    std::string Get(const std::string key);
    void Set(const std::string key, const std::string value, absl::Duration ttl);
    void Delete(const std::string key);
private:
    ShardMap* shardMap;
    ClientPool* clientPool;
};

#endif

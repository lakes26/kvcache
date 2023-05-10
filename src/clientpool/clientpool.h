#ifndef CLIENTPOOL_H
#define CLIENTPOOL_H


#include "shardmap/shardmap.h"
#include "proto/kv.grpc.pb.h"

class ClientPool {
private:
    ShardMap* shardMap;
    std::map<std::string, std::shared_ptr<kv::Kv::Stub>> clients;
    absl::Mutex mu;
public:
    ClientPool(ShardMap* shardMap);
    std::shared_ptr<kv::Kv::Stub> GetClient(std::string nodeName);
};

#endif

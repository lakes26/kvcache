
#include "shardmap/shardmap.h"
#include <map>
#include <string>
#include "server/server.h"
#include "clientpool/clientpool.h"
#include "client/client.h"
#include <grpc++/grpc++.h>


class TestSetup {
  public:
    ShardMap* shardMap;
    nodes     std:map<std::string, KvServerImpl>;
    clientPool  ClientPool;
    kv  KvClient;
    ctx grpc::ClientContext;

    TestSetup MakeTestSetup(ShardMapState smState);


}



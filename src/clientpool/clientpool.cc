
#include <grpc++/grpc++.h>
#include "proto/kv.grpc.pb.h"
#include "absl/synchronization/mutex.h"

#include "proto/kv.pb.h"
#include "shardmap/shardmap.h"
#include "clientpool.h"

#include <string>
#include <memory>
#include <stdexcept>


using kv::Kv;

using namespace grpc;

std::shared_ptr<Kv::Stub> makeConnection(const std::string& addr) {
    ChannelArguments args;
    std::shared_ptr<ChannelCredentials> creds = InsecureChannelCredentials();
    std::shared_ptr<Channel> channel = CreateCustomChannel(addr, creds, args);

    return Kv::NewStub(channel);
}

ClientPool::ClientPool(ShardMap* shardMap) {
    this->shardMap = shardMap;
}

std::shared_ptr<Kv::Stub> ClientPool::GetClient(std::string nodeName) {

    this->mu.ReaderLock();
    auto it = clients.find(nodeName);
    this->mu.ReaderUnlock();

    if (it != clients.end()) {
      return it->second;
    }

    this->mu.Lock();

    it = this->clients.find(nodeName);
    
    if (it != clients.end()) {
        this->mu.Unlock();
        return it->second;
    }



    auto nodeInfoMap = this->shardMap->nodes();
    auto it2 = nodeInfoMap.find(nodeName);

    if (it2 == nodeInfoMap.end()) {
        this->mu.Unlock();
        throw std::runtime_error("no node by that name in the shardmap"); 
    }

    NodeInfo nodeInfo = it2->second;
    std::string address = nodeInfo.address + ":" + std::to_string(nodeInfo.port);
    std::shared_ptr<Kv::Stub> stub = makeConnection(address);

    this->clients[nodeName] = stub;
    this->mu.Unlock();
    return stub; 
}


#ifndef SHARDMAP_H
#define SHARDMAP_H

#include <map>
#include <vector>

struct NodeInfo {
    std::string address;
    int32_t port;
};

/*
 * Internal state of the ShardMap, which may be updated as
 * shards are moved around.
 */
struct ShardMapState {
    std::map<std::string, NodeInfo> nodes;
    std::map<int, std::vector<std::string>> shardsToNodes;
    int numShards;
};


class ShardMap {
private:
    ShardMapState state;    
public:
    ShardMap();
    ShardMap(ShardMapState state);
    ShardMapState* getState();
    std::map<std::string, NodeInfo>& nodes();
    int numShards();
    std::vector<int> shardsForNode(const std::string& nodeName);
    std::vector<std::string> nodesForShard(int shard);
    void applyFromFile(const std::string filename);
};


class FileShardMap {
public:
    ShardMap shardMap;
    std::string filename;

    FileShardMap(const std::string& filename);
    void applyFromFile(); 
};

#endif

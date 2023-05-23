#include <streambuf>
#include <iostream>
#include <fstream>
#include <string>

#include "nlohmann/json.hpp"
#include "shardmap.h"

using nlohmann::json;

FileShardMap::FileShardMap(const std::string& filename) : filename(filename) {
    ShardMap shardMap;
}

void FileShardMap::applyFromFile() {
    std::ifstream file(this->filename);
    std::string data((std::istreambuf_iterator<char>(file)),
                     std::istreambuf_iterator<char>());

    auto j = json::parse(data);

    ShardMapState smState;
    smState.numShards = j["numShards"].get<int>();
    for (auto& [nodeName, nodeData] : j["nodes"].items()) {
        NodeInfo info;
        info.address = nodeData["address"].get<std::string>();
        info.port = nodeData["port"].get<int>();
        smState.nodes[nodeName] = info;
    }
    for (auto& [shardNum, nodeNames] : j["shards"].items()) {
        std::vector<std::string> nodes;
        for (auto& nodeName : nodeNames) {
            nodes.push_back(nodeName.get<std::string>());
        }
        smState.shardsToNodes[std::stoi(shardNum)] = nodes;
    }
    shardMap = ShardMap(smState);     
}


#include <string>
#include <fstream>
#include <streambuf>
#include <iostream>

#include "shardmap.h"

FileShardMap::FileShardMap(const std::string& filename) : filename(filename) {
    ShardMap shardMap;
}

void FileShardMap::applyFromFile() {
    std::ifstream file(filename);
    std::string data((std::istreambuf_iterator<char>(file)),
                     std::istreambuf_iterator<char>());

    ShardMapState smState;
    /*
    try {
        smState = json::parse(data).get<ShardMapState>();
    } catch (const std::exception& e) {
        std::cerr << "Error parsing JSON: " << e.what() << std::endl;
        return;
    }

    shardMap = ShardMap(smState);
    */
}

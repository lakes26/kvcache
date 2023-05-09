#ifndef FILE_SHARDMAP_H
#define FILE_SHARDMAP_H

#include <string>

class FileShardMap {
public:
    ShardMap ShardMap;
    std::string filename;

    FileShardMap(const std::string& filename);
    void applyFromFile(); 
};

#endif

#include <cstdint>
#include <string>
#include <functional>

int GetShardForKey(const std::string& key, int numShards) {
    std::hash<std::string> hasher;
    uint32_t hash = hasher(key);
    return (hash % numShards) + 1;
}

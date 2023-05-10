#ifndef KVSHARD_H
#define KVSHARD_H

#include <string>
#include <tuple>
#include <map>
#include "absl/synchronization/mutex.h"
#include <cstdint>

struct CacheRecord {
  std::string value;
  absl::Time  expirationTime;
};
  

class KvShard {
private:
    absl::Mutex mu;
    std::map<std::string, CacheRecord> data;

public:
    KvShard();
    std::tuple<std::string, bool> Get(const std::string key);
    void Set(const std::string key, const std::string value, int64_t ttlMs);
    void Delete(const std::string key);
};

#endif

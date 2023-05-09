#ifndef KVSHARD_H
#define KVSHARD_H

#include <string>
#include <map>
#include "absl/time/time.h"
#include "absl/synchronization/mutex.h"

struct CacheRecord {
  std::string value;
  absl::Time  expirationTime;
};
  

class KvShard {
private:
    absl::Mutex                     mu;
    std::map<std::string, CacheRecord>   data;

public:
    KvShard();
    std::string Get(const std::string key);
    void Set(const std::string key, const std::string value, absl::Duration ttl);
    void Delete(const std::string key);
};

#endif

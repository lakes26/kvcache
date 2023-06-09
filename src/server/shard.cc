#include "shard.h"

#include <cstdint>
#include <string>
#include <tuple>
#include <map>

#include "absl/synchronization/mutex.h"
#include "absl/time/time.h"

KvShard::KvShard() {
  ;
}

std::tuple<std::string, bool> KvShard::Get(const std::string key){
  
  this->mu.ReaderLock();
  auto it = this->data.find(key);
  
  // key not found
  if (it == this->data.end()) {
    this->mu.ReaderUnlock();
    return std::make_tuple("", false);
  }

  CacheRecord record = it->second;
  
  // value is expired
  if(absl::Now() > record.expirationTime) {
    this->mu.ReaderUnlock();
    return std::make_tuple("", false);
  }

  this->mu.ReaderUnlock();
  return std::make_tuple(record.value, true);
}

void KvShard::Set(const std::string key, const std::string value, int64_t ttl) {
  absl::Time expiration_time = absl::Now() + absl::Milliseconds(ttl);
  this->mu.Lock();

  CacheRecord record = {value, expiration_time};
  this->data[key] = record;
  this->mu.Unlock(); 
}

void KvShard::Delete(const std::string key){
  this->mu.Lock();
  this->data.erase(key);
  this->mu.Unlock(); 
}

void KvShard::Clean() {
  this->mu.Lock();
 
  absl::Time currentTime = absl::Now();

  std::map<std::string, CacheRecord> newData;
  for (auto [key, value] : this->data) {
    if(currentTime < value.expirationTime) {
      newData[key] = value;
    }
  }
  this->data = newData;

  this->mu.Unlock();
}

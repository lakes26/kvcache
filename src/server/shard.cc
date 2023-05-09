#include <string.h>
#include "shard.h"


KvShard::KvShard() {
  ;
}

std::string KvShard::Get(const std::string key){
  this->mu.ReaderLock();
  auto it = this->data.find(key);
  if (it == this->data.end()) {
    // TODO:Throw Exception
    this->mu.ReaderUnlock();
    return "";
  }

  CacheRecord record = it->second;
  
  //TODO compare time to expiration time throw exception
  if(false) {
    this->mu.ReaderUnlock();
    return "";
  }
  
  this->mu.ReaderUnlock();
  return record.value;
}

void KvShard::Set(const std::string key, const std::string value, absl::Duration ttl){
  ;
}

void KvShard::Delete(const std::string key){
  ;
}

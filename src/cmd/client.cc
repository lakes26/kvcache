
#include <glog/logging.h>
#include "src/shardmap/shardmap.h"
#include "src/clientpool/clientpool.h"
#include "src/client/client.h"
#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include <string>
#include <vector>
#include <tuple>


ABSL_FLAG(std::string, shardmap, "",
          "Path to a JSON file which describes the shard map");

void usage() {
  LOG(FATAL) << "Usage: client [get|set|delete] key [value] [ttl]" << std::endl;
}

void set(std::string key, std::string value, std::string ttl) {
    std::cout << "set" << std::endl; 
}

void get(std::string key) {
    std::cout << "get" << std::endl;
}

void deletekey(std::string key) {
    std::cout << "delete" << std::endl;
}

int main(int argc, char** argv) {
  absl::ParseCommandLine(argc, argv);
  google::InitGoogleLogging(argv[0]);
  google::InstallFailureSignalHandler();

  std::string shardmap_filename = absl::GetFlag(FLAGS_shardmap);

  // assure that the shardmap file is there
  if (shardmap_filename.length() == 0) {
    LOG(FATAL) << "--shardmap is required" << std::endl;
  }

  // Extract the positional arguments
  std::vector<std::string> positional_args;
  for (int i = 1; i < argc; ++i) {
    if (argv[i][0] != '-') {
      positional_args.push_back(argv[i]);
    }
  }
  
  if (positional_args.size() < 2) {
    usage();
  }

  // create shard map and client pool
  FileShardMap fileSm = FileShardMap(shardmap_filename);
  fileSm.applyFromFile(); 
  ClientPool clientPool = ClientPool(&fileSm.shardMap);

  // build the KvClient
  KvClient client = KvClient(&fileSm.shardMap, &clientPool);

  std::string subcmd = positional_args[0];
  std::string key = positional_args[1];

  grpc::Status status;

  // SET BRANCH
  if (subcmd == "set" && positional_args.size() == 4) {
    std::string value = positional_args[2];
    int64_t ttlMs;
    absl::Duration ttl;
    try {
      ttlMs = std::stoll(positional_args[3]);
      ttl = absl::Milliseconds(ttlMs);
    } catch (const std::exception& e) {
      LOG(FATAL) << "Expected long int value for ttlMs" << std::endl;
    }

    status = client.Set(key, value, ttl); 
    if(!status.ok()) {
      LOG(ERROR) << "Error setting value on all nodes hosting shard" << std::endl;
    }

  // GET BRANCH
  } else if (subcmd == "get" && positional_args.size() == 2) {
    auto tup = client.Get(key); 

    std::string value = std::get<0>(tup);
    bool was_found = std::get<1>(tup);
    status = std::get<2>(tup);

    if(!status.ok()) {
      LOG(ERROR) << "Error getting value" << std::endl;   
    } else if (!was_found) {
      LOG(ERROR) << "No value set for key" << std::endl;   
    } else {
      std::cout << "value: " << value << std::endl;
    }

  // DELETE BRANCH
  } else if (subcmd == "delete" && positional_args.size() == 2) {
    status = client.Delete(key); 
    if(!status.ok()) {
      LOG(ERROR) << "Error deleting value on all nodes hosting shard" << std::endl;
    }
  // DEFAULT BRANCH
  } else {
    usage();
  }
  return 0;
}


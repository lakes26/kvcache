
#include <glog/logging.h>
#include "shardmap/shardmap.h"
#include "clientpool/clientpool.h"
#include "client/client.h"
#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include <string>
#include <vector>


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

  // SET BRANCH
  if (subcmd == "set" && positional_args.size() == 4) {
    std::string value = positional_args[2];
    int ttlMs;
    absl::Duration ttl;
    try {
      ttlMs = std::stoi(positional_args[3]);
      ttl = absl::Milliseconds(ttlMs);
    } catch (const std::exception& e) {
      LOG(FATAL) << "Expected int value for ttlMs" << std::endl;
    }

    try {
      client.Set(key, value, ttl); 
    } catch (const std::exception& e) {
      LOG(ERROR) << "Error setting value: " << e.what() << std::endl;
    }

  // GET BRANCH
  } else if (subcmd == "get" && positional_args.size() == 2) {
    std::string value;
    try {
      value = client.Get(key); 
      std::cout << "value: " << value << std::endl;
    } catch (const std::exception& e) {
      LOG(ERROR) << "Error getting value: " << e.what() << std::endl;
    }

  // DELETE BRANCH
  } else if (subcmd == "delete" && positional_args.size() == 2) {
    try {
      client.Delete(key); 
      std::cout << "value: " << std::endl;
    } catch (const std::exception& e) {
      LOG(ERROR) << "Error deleting value: " << e.what() << std::endl;
    }
  // DEFAULT BRANCH
  } else {
    usage();
  }

  return 0;
}




# kvcache

This is my implementation of a key value cache. 

## Building

There are 2 binary targets to this build: //cmd:server and //cmd:client

I was building for darwin_arm64 but I'm fairly confident bazel will build for whatever machine the binary is being built
on unless specified otherwise.
The build on my local machine was done with Clang
I am using bazel version 6.0.0 as noted in the src/.bazelversion file which is automatically aquired if you have bazelisk.
I am building for C++20 as noted in the src/.bazelrc file: build --action_env=BAZEL_CXXOPTS="-std=c++20"

From the src directory (with the WORKSPACE file), Build each by running:

$ bazel build //cmd:server 
$ bazel build //cmd:client

These builds might take a couple of minutes to complete the first time.


## Running

From the src directory run a server by running:

$ ./bazel-bin/cmd/server --shardmap=[path/to/shardmap.json] --node=[nodeName]

Both flags are manditory and the program will exit if they are not provided.
The shardmap flag is to provide a filename for a json shardmap file. There are example files in src/shardmaps/
The node flag is to provide the nodeName for the server. The nodename must be present in the shardMap.

The only way to run a cluster of servers is manaully by launching each node individually.

From the src directory, run a client command by running:

$ ./bazel-bin/cmd/client --shardmap=[path/to/shardmap.json] [get|set|delete] key [value] [ttl]

the flag is manditory and the program will exit if it is not provided.
The shardmap flag is to provide a filename for a json shardmap file. There are example files in src/shardmaps/


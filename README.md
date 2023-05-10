# kvcache

This is my implementation of a key value cache. 

## Building

There are 2 binary targets to this build: //cmd:server and //cmd:client
I was building for darwin_arm64 but I'm fairly confident bazel will build for whatever machine the binary is being built
on unless specified otherwise.


From the src directory (with the WORKSPACE file), Build each by running:

$ bazel build //cdm::server 
$ bazel build //cdm::client

These builds might take a couple of minutes to complete.


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


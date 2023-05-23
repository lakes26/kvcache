#include "shardmap.h"

#include <vector>
#include <string>
#include <atomic>
#include <map>
#include <set>


using namespace std;

/*
 * Whether a ShardMapState is valid.
 */
bool IsValid(const ShardMapState& smState) {
    if (smState.shardsToNodes.size() > smState.numShards) {
        return false;
    }
    for (const auto& entry : smState.shardsToNodes) {
        const int shard = entry.first;
        const vector<string>& nodes = entry.second;
        // shard must be 1..NumShards
        if (shard < 1 || shard > smState.numShards) {
            return false;
        }
        set<string> nodeSet;
        for (const auto& node : nodes) {
            // node must be exist
            if (smState.nodes.count(node) == 0) {
                return false;
            }
            // cannot have duplicate nodes for a shard
            if (nodeSet.count(node) > 0) {
                return false;
            }
            nodeSet.insert(node);
        }
    }
    return true;
}


/*
 * Start of the ShardMap Implementation
 */

ShardMap::ShardMap() {
    this->state = ShardMapState{
        .numShards = 1,
        .shardsToNodes = std::map<int, std::vector<std::string>>(),
        .nodes = std::map<std::string, NodeInfo>()
    };
}

ShardMap::ShardMap(ShardMapState newState) {
    this->state = newState;
}
/*
 * Gets a pointer to the latest internal state via a thread-safe atomic load.
 */
ShardMapState* ShardMap::getState() {
    return &(this->state);
}

/*
 * Gets the set of nodes for the entire cluster keyed by nodeName.
 * To access a single node, use Nodes()[nodeName].
 */
map<string, NodeInfo>& ShardMap::nodes() {
    return getState()->nodes;
}

/*
 * Gets the total number of shards for all keys in the cluster. Note that
 * shards may not necessarily be assigned to any nodes in failure cases.
 *
 * You may assume that NumShards() does not ever change.
 */
int ShardMap::numShards() {
    return getState()->numShards;
}

/*
 * Gets the set of integer shards assigned to a given node (by node name).
 */
vector<int> ShardMap::shardsForNode(const string& nodeName) {
    ShardMapState* state = getState();
    vector<int> shards;
    for (const auto& entry : state->shardsToNodes) {
        const int shard = entry.first;
        const vector<string>& nodes = entry.second;
        for (const auto& node : nodes) {
            if (node == nodeName) {
                shards.push_back(shard);
            }
        }
    }
    return shards;
}

/*
 * Gets the set of nodes that currently host a given shard, returned
 * by node name. If you need full information, use the node name
 * as a key to query the Nodes() map.
 */
vector<string> ShardMap::nodesForShard(int shard) {
    ShardMapState* state = getState();
    auto it = state->shardsToNodes.find(shard);
    if (it == state->shardsToNodes.end()) {
        return vector<string>();
    }
    return it->second;
}


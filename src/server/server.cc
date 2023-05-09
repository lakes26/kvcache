
#include <iostream>
#include <memory>
#include <string>

#include <grpc++/grpc++.h>

#ifdef BAZEL_BUILD
#include "proto/kv.grpc.pb.h"
#else
#include "proto/kv.grpc.pb.h"
#endif

#include "server.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using kv::GetRequest;
using kv::GetResponse;
using kv::SetRequest;
using kv::SetResponse;
using kv::DeleteRequest;
using kv::DeleteResponse;
using kv::GetShardContentsRequest;
using kv::GetShardContentsResponse;
using kv::Kv;


KvServerImpl::KvServerImpl(ShardMap* shardMap) {
  this->shardMap = shardMap;
}
 
Status KvServerImpl::Get(
  ServerContext* context,
  const GetRequest* request,
  GetResponse* response
) {
  return Status::OK;
}

Status KvServerImpl::Set(
  ServerContext* context,
  const SetRequest* request,
  SetResponse* response
) {
  return Status::OK;
}

Status KvServerImpl::Delete(
  ServerContext* context,
  const DeleteRequest* request,
  DeleteResponse* response
) {
  return Status::OK;
}

Status KvServerImpl::GetShardContents(
  ServerContext* context,
  const GetShardContentsRequest* request,
  GetShardContentsResponse* response
) {
  return Status::OK;
}
 

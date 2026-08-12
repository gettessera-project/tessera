// Copyright (c) 2026 tessera core
// See COPYING for license.

#ifndef TESSERA_INTERFACES_RPC_H
#define TESSERA_INTERFACES_RPC_H

#include <memory>
#include <string>

class UniValue;

namespace node {
struct NodeContext;
} // namespace node

namespace interfaces {
//! Interface giving clients ability to emulate HTTP RPC calls.
class Rpc
{
public:
    virtual ~Rpc() = default;
    virtual UniValue executeRpc(UniValue request, std::string url, std::string user) = 0;
};

//! Return implementation of Rpc interface.
std::unique_ptr<Rpc> MakeRpc(node::NodeContext& node);

} // namespace interfaces

#endif // TESSERA_INTERFACES_RPC_H
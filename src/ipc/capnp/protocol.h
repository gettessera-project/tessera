// Copyright (c) 2026 tessera core
// See COPYING for license.

#ifndef TESSERA_IPC_CAPNP_PROTOCOL_H
#define TESSERA_IPC_CAPNP_PROTOCOL_H

#include <memory>

namespace ipc {
class Protocol;
namespace capnp {
std::unique_ptr<Protocol> MakeCapnpProtocol();
} // namespace capnp
} // namespace ipc

#endif // TESSERA_IPC_CAPNP_PROTOCOL_H

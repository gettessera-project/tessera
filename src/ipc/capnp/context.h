// Copyright (c) 2026 tessera core
// See COPYING for license.

#ifndef TESSERA_IPC_CAPNP_CONTEXT_H
#define TESSERA_IPC_CAPNP_CONTEXT_H

#include <ipc/context.h>

namespace ipc {
namespace capnp {
//! Cap'n Proto context struct. Generally the parent ipc::Context struct should
//! be used instead of this struct to give all IPC protocols access to
//! application state, so there aren't unnecessary differences between IPC
//! protocols. But this specialized struct can be used to pass capnp-specific
//! function and object types to capnp hooks.
struct Context : ipc::Context
{
};
} // namespace capnp
} // namespace ipc

#endif // TESSERA_IPC_CAPNP_CONTEXT_H

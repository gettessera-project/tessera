// Copyright (c) 2026 tessera core
// See COPYING for license.

#ifndef TESSERA_IPC_EXCEPTION_H
#define TESSERA_IPC_EXCEPTION_H

#include <stdexcept>

namespace ipc {
//! Exception class thrown when a call to remote method fails due to an IPC
//! error, like a socket getting disconnected.
class Exception : public std::runtime_error
{
public:
    using std::runtime_error::runtime_error;
};
} // namespace ipc

#endif // TESSERA_IPC_EXCEPTION_H
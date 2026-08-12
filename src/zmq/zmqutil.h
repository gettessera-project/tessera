// Copyright (c) 2026 tessera core
// See COPYING for license.

#ifndef TESSERA_ZMQ_ZMQUTIL_H
#define TESSERA_ZMQ_ZMQUTIL_H

#include <string>

void zmqError(const std::string& str);

/** Prefix for unix domain socket addresses (which are local filesystem paths) */
const std::string ADDR_PREFIX_IPC = "ipc://"; // used by libzmq, example "ipc:///root/path/to/file"

#endif // TESSERA_ZMQ_ZMQUTIL_H

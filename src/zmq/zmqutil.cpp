// Copyright (c) 2026 tessera core
// See COPYING for license.

#include <zmq/zmqutil.h>

#include <util/log.h>

#include <zmq.h>

#include <cerrno>
#include <string>

void zmqError(const std::string& str)
{
    LogDebug(BCLog::ZMQ, "Error: %s, msg: %s\n", str, zmq_strerror(errno));
}

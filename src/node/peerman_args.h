// Copyright (c) 2026 tessera core
// See COPYING for license.

#ifndef TESSERA_NODE_PEERMAN_ARGS_H
#define TESSERA_NODE_PEERMAN_ARGS_H

#include <net_processing.h>

class ArgsManager;

namespace node {
void ApplyArgsManOptions(const ArgsManager& argsman, PeerManager::Options& options);
} // namespace node

#endif // TESSERA_NODE_PEERMAN_ARGS_H
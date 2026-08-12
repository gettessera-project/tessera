// Copyright (c) 2026 tessera core
// See COPYING for license.

#ifndef TESSERA_NODE_BLOCKMANAGER_ARGS_H
#define TESSERA_NODE_BLOCKMANAGER_ARGS_H

#include <node/blockstorage.h>
#include <util/result.h>

class ArgsManager;

namespace node {
[[nodiscard]] util::Result<void> ApplyArgsManOptions(const ArgsManager& args, BlockManager::Options& opts);
} // namespace node

#endif // TESSERA_NODE_BLOCKMANAGER_ARGS_H
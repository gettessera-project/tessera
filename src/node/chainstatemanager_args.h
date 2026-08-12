// Copyright (c) 2026 tessera core
// See COPYING for license.

#ifndef TESSERA_NODE_CHAINSTATEMANAGER_ARGS_H
#define TESSERA_NODE_CHAINSTATEMANAGER_ARGS_H

#include <util/result.h>
#include <validation.h>

class ArgsManager;

/** -par default (number of script-checking threads, 0 = auto) */
static constexpr int DEFAULT_SCRIPTCHECK_THREADS{0};

namespace node {
[[nodiscard]] util::Result<void> ApplyArgsManOptions(const ArgsManager& args, ChainstateManager::Options& opts);
} // namespace node

#endif // TESSERA_NODE_CHAINSTATEMANAGER_ARGS_H
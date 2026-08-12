// Copyright (c) 2026 tessera core
// See COPYING for license.

#ifndef TESSERA_KERNEL_BLOCKMANAGER_OPTS_H
#define TESSERA_KERNEL_BLOCKMANAGER_OPTS_H

#include <dbwrapper.h>
#include <kernel/notifications_interface.h>
#include <util/fs.h>

#include <cstdint>

class CChainParams;

namespace kernel {

static constexpr bool DEFAULT_XOR_BLOCKSDIR{true};

/**
 * An options struct for `BlockManager`, more ergonomically referred to as
 * `BlockManager::Options` due to the using-declaration in `BlockManager`.
 */
struct BlockManagerOpts {
    const CChainParams& chainparams;
    bool use_xor{DEFAULT_XOR_BLOCKSDIR};
    uint64_t prune_target{0};
    bool fast_prune{false};
    const fs::path blocks_dir;
    Notifications& notifications;
    DBParams block_tree_db_params;
};

} // namespace kernel

#endif // TESSERA_KERNEL_BLOCKMANAGER_OPTS_H

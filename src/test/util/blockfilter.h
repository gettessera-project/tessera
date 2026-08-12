// Copyright (c) 2026 tessera core
// See COPYING for license.

#ifndef TESSERA_TEST_UTIL_BLOCKFILTER_H
#define TESSERA_TEST_UTIL_BLOCKFILTER_H

#include <blockfilter.h>

class CBlockIndex;
namespace node {
class BlockManager;
}

bool ComputeFilter(BlockFilterType filter_type, const CBlockIndex& block_index, BlockFilter& filter, const node::BlockManager& blockman);

#endif // TESSERA_TEST_UTIL_BLOCKFILTER_H

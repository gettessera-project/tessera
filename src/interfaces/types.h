// Copyright (c) 2026 tessera core
// See COPYING for license.

#ifndef TESSERA_INTERFACES_TYPES_H
#define TESSERA_INTERFACES_TYPES_H

#include <uint256.h>

namespace interfaces {

//! Hash/height pair to help track and identify blocks.
struct BlockRef {
    uint256 hash;
    int height = -1;
};

} // namespace interfaces

#endif // TESSERA_INTERFACES_TYPES_H
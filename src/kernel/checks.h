// Copyright (c) 2026 tessera core
// See COPYING for license.

#ifndef TESSERA_KERNEL_CHECKS_H
#define TESSERA_KERNEL_CHECKS_H

#include <util/result.h>

namespace kernel {

struct Context;

/**
 *  Ensure a usable environment with all necessary library support.
 */
[[nodiscard]] util::Result<void> SanityChecks(const Context&);
} // namespace kernel

#endif // TESSERA_KERNEL_CHECKS_H
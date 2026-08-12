// Copyright (c) 2026 tessera core
// See COPYING for license.

#include <kernel/context.h>

#include <random.h>

#include <mutex>

namespace kernel {
Context::Context()
{
    static std::once_flag globals_initialized{};
    std::call_once(globals_initialized, []() {
        // Tessera has no secp256k1 (ML-DSA) and a single SHA3 implementation,
        // so there is no ECC start or SHA256 auto-detection here — just the RNG.
        RandomInit();
    });
}
} // namespace kernel

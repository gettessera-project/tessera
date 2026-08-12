// Copyright (c) 2026 tessera core
// See COPYING for license.

#include <pubkey.h>

extern "C" {
#include "api.h"
}

#include <cstddef>
#include <cstdint>
#include <span>

bool IsKnownScheme(SigScheme s)
{
    switch (s) {
        case SigScheme::ML_DSA_44:
            return true;
    }
    return false;
}

size_t SchemePublicKeySize(SigScheme s)
{
    switch (s) {
        case SigScheme::ML_DSA_44:
            return PQCLEAN_MLDSA44_CLEAN_CRYPTO_PUBLICKEYBYTES;
    }
    return 0;
}

size_t SchemeSignatureSize(SigScheme s)
{
    switch (s) {
        case SigScheme::ML_DSA_44:
            return PQCLEAN_MLDSA44_CLEAN_CRYPTO_BYTES;
    }
    return 0;
}

size_t SchemeSecretKeySize(SigScheme s)
{
    switch (s) {
        case SigScheme::ML_DSA_44:
            return PQCLEAN_MLDSA44_CLEAN_CRYPTO_SECRETKEYBYTES;
    }
    return 0;
}

int64_t SchemeSigOpCost(SigScheme s)
{
    switch (s) {
        case SigScheme::ML_DSA_44:
            return 1;
    }
    return 0;  // unknown scheme: contributes nothing (rejected without a verify)
}

bool CPubKey::Verify(const uint256& hash, std::span<const unsigned char> sig) const
{
    if (!IsValid())
        return false;
    switch (GetScheme()) {
        case SigScheme::ML_DSA_44:
            // PQClean enforces the exact signature length internally and returns
            // 0 on a valid signature; the public-key length is gated by IsValid()
            // (GetLen() requires the scheme's exact 1312-byte key, as the
            // conformance suite requires).
            return PQCLEAN_MLDSA44_CLEAN_crypto_sign_verify(
                       sig.data(), sig.size(), hash.begin(), hash.size(), RawKey().data()) == 0;
    }
    return false;
}

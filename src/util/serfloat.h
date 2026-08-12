// Copyright (c) 2026 tessera core
// See COPYING for license.

#ifndef TESSERA_UTIL_SERFLOAT_H
#define TESSERA_UTIL_SERFLOAT_H

#include <cstdint>

/* Encode a double using the IEEE 754 binary64 format. All NaNs are encoded as x86/ARM's
 * positive quiet NaN with payload 0. */
uint64_t EncodeDouble(double f) noexcept;
/* Reverse operation of DecodeDouble. DecodeDouble(EncodeDouble(f))==f unless isnan(f). */
double DecodeDouble(uint64_t v) noexcept;

#endif // TESSERA_UTIL_SERFLOAT_H

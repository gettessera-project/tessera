// Copyright (c) 2026 tessera core
// See COPYING for license.

#ifndef TESSERA_RANDOMENV_H
#define TESSERA_RANDOMENV_H

#include <crypto/sha3_512.h>

/** Gather non-cryptographic environment data that changes over time. */
void RandAddDynamicEnv(SHA3_512& hasher);

/** Gather non-cryptographic environment data that does not change over time. */
void RandAddStaticEnv(SHA3_512& hasher);

#endif // TESSERA_RANDOMENV_H

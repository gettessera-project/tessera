// Copyright (c) 2026 tessera core
// See COPYING for license.

#ifndef TESSERA_RPC_MEMPOOL_H
#define TESSERA_RPC_MEMPOOL_H

class CTxMemPool;
class UniValue;

/** Mempool information to JSON */
UniValue MempoolInfoToJSON(const CTxMemPool& pool);

/** Mempool to JSON */
UniValue MempoolToJSON(const CTxMemPool& pool, bool verbose = false, bool include_mempool_sequence = false);

#endif // TESSERA_RPC_MEMPOOL_H
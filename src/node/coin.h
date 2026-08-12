// Copyright (c) 2026 tessera core
// See COPYING for license.

#ifndef TESSERA_NODE_COIN_H
#define TESSERA_NODE_COIN_H

#include <map>

class COutPoint;
class Coin;

namespace node {
struct NodeContext;

/**
 * Look up unspent output information. Returns coins in the mempool and in the
 * current chain UTXO set. Iterates through all the keys in the map and
 * populates the values.
 *
 * @param[in] node The node context to use for lookup
 * @param[in,out] coins map to fill
 */
void FindCoins(const node::NodeContext& node, std::map<COutPoint, Coin>& coins);
} // namespace node

#endif // TESSERA_NODE_COIN_H
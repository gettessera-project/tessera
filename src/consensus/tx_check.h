// Copyright (c) 2026 tessera core
// See COPYING for license.

#ifndef TESSERA_CONSENSUS_TX_CHECK_H
#define TESSERA_CONSENSUS_TX_CHECK_H

/**
 * Context-independent transaction checking code that can be called outside the
 * tessera server and doesn't depend on chain or mempool state. Transaction
 * verification code that does call server functions or depend on server state
 * belongs in tx_verify.h/cpp instead.
 */

class CTransaction;
class TxValidationState;

bool CheckTransaction(const CTransaction& tx, TxValidationState& state);

#endif // TESSERA_CONSENSUS_TX_CHECK_H

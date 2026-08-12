// Copyright (c) 2026 tessera core
// See COPYING for license.

#ifndef TESSERA_WALLET_LOAD_H
#define TESSERA_WALLET_LOAD_H

#include <string>
#include <vector>

class ArgsManager;
class CScheduler;

namespace wallet {
struct WalletContext;

//! Responsible for reading and validating the -wallet arguments and verifying
//! the wallet database. Returns false on error.
bool VerifyWallets(WalletContext& context);

//! Load wallet databases. Returns false on error.
bool LoadWallets(WalletContext& context);

//! Complete startup of wallets (kick off any background tasks).
void StartWallets(WalletContext& context, CScheduler& scheduler);

//! Flush all wallets in preparation for shutdown.
void FlushWallets(WalletContext& context);

//! Stop all wallets. Wallets will be flushed first.
void StopWallets(WalletContext& context);

//! Close all wallets.
void UnloadWallets(WalletContext& context);

} // namespace wallet

#endif // TESSERA_WALLET_LOAD_H

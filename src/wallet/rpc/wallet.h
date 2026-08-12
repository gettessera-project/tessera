// Copyright (c) 2026 tessera core
// See COPYING for license.

#ifndef TESSERA_WALLET_RPC_WALLET_H
#define TESSERA_WALLET_RPC_WALLET_H

#include <span>

class CRPCCommand;

namespace wallet {
std::span<const CRPCCommand> GetWalletRPCCommands();
} // namespace wallet

#endif // TESSERA_WALLET_RPC_WALLET_H

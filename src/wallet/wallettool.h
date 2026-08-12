// Copyright (c) 2026 tessera core
// See COPYING for license.

#ifndef TESSERA_WALLET_WALLETTOOL_H
#define TESSERA_WALLET_WALLETTOOL_H

#include <string>

class ArgsManager;

namespace wallet {
namespace WalletTool {

bool ExecuteWalletToolFunc(const ArgsManager& args, const std::string& command);

} // namespace WalletTool
} // namespace wallet

#endif // TESSERA_WALLET_WALLETTOOL_H

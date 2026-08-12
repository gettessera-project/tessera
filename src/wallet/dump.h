// Copyright (c) 2026 tessera core
// See COPYING for license.

#ifndef TESSERA_WALLET_DUMP_H
#define TESSERA_WALLET_DUMP_H

#include <util/fs.h>

#include <string>
#include <vector>

struct bilingual_str;
class ArgsManager;

namespace wallet {
class WalletDatabase;

bool DumpWallet(const ArgsManager& args, WalletDatabase& db, bilingual_str& error);
bool CreateFromDump(const ArgsManager& args, const std::string& name, const fs::path& wallet_path, bilingual_str& error, std::vector<bilingual_str>& warnings);
} // namespace wallet

#endif // TESSERA_WALLET_DUMP_H

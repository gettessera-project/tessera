// Copyright (c) 2026 tessera core
// See COPYING for license.

#ifndef TESSERA_UTIL_CHAINTYPE_H
#define TESSERA_UTIL_CHAINTYPE_H

#include <optional>
#include <string>
#include <string_view>

enum class ChainType {
    MAIN,
    TESTNET,
    REGTEST,
};

std::string ChainTypeToString(ChainType chain);

std::optional<ChainType> ChainTypeFromString(std::string_view chain);

#endif // TESSERA_UTIL_CHAINTYPE_H

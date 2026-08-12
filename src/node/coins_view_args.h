// Copyright (c) 2026 tessera core
// See COPYING for license.

#ifndef TESSERA_NODE_COINS_VIEW_ARGS_H
#define TESSERA_NODE_COINS_VIEW_ARGS_H

class ArgsManager;
struct CoinsViewOptions;

namespace node {
void ReadCoinsViewArgs(const ArgsManager& args, CoinsViewOptions& options);
} // namespace node

#endif // TESSERA_NODE_COINS_VIEW_ARGS_H
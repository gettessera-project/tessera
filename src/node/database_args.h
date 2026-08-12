// Copyright (c) 2026 tessera core
// See COPYING for license.

#ifndef TESSERA_NODE_DATABASE_ARGS_H
#define TESSERA_NODE_DATABASE_ARGS_H

class ArgsManager;
struct DBOptions;

namespace node {
void ReadDatabaseArgs(const ArgsManager& args, DBOptions& options);
} // namespace node

#endif // TESSERA_NODE_DATABASE_ARGS_H
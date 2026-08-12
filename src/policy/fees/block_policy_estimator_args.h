// Copyright (c) 2026 tessera core
// See COPYING for license.

#ifndef TESSERA_POLICY_FEES_BLOCK_POLICY_ESTIMATOR_ARGS_H
#define TESSERA_POLICY_FEES_BLOCK_POLICY_ESTIMATOR_ARGS_H

#include <util/fs.h>

class ArgsManager;

/** @return The fee estimates data file path. */
fs::path FeeestPath(const ArgsManager& argsman);

#endif // TESSERA_POLICY_FEES_BLOCK_POLICY_ESTIMATOR_ARGS_H
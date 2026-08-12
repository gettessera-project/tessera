// Copyright (c) 2026 tessera core
// See COPYING for license.

#ifndef TESSERA_NODE_ABORT_H
#define TESSERA_NODE_ABORT_H

#include <atomic>
#include <functional>

struct bilingual_str;

namespace node {
class Warnings;
void AbortNode(const std::function<bool()>& shutdown_request, std::atomic<int>& exit_status, const bilingual_str& message, node::Warnings* warnings);
} // namespace node

#endif // TESSERA_NODE_ABORT_H
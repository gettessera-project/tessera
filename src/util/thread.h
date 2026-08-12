// Copyright (c) 2026 tessera core
// See COPYING for license.

#ifndef TESSERA_UTIL_THREAD_H
#define TESSERA_UTIL_THREAD_H

#include <functional>
#include <string_view>

namespace util {
/**
 * A wrapper for do-something-once thread functions.
 */
void TraceThread(std::string_view thread_name, std::function<void()> thread_func);

} // namespace util

#endif // TESSERA_UTIL_THREAD_H

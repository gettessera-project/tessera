// Copyright (c) 2026 tessera core
// See COPYING for license.

#ifndef TESSERA_UTIL_EXCEPTION_H
#define TESSERA_UTIL_EXCEPTION_H

#include <exception>
#include <string_view>

void PrintExceptionContinue(const std::exception* pex, std::string_view thread_name);

#endif // TESSERA_UTIL_EXCEPTION_H

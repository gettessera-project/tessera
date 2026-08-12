// Copyright (c) 2026 tessera core
// See COPYING for license.

#ifndef TESSERA_TEST_UTIL_JSON_H
#define TESSERA_TEST_UTIL_JSON_H

#include <univalue.h>

#include <string_view>

UniValue read_json(std::string_view jsondata);

#endif // TESSERA_TEST_UTIL_JSON_H

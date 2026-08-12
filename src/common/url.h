// Copyright (c) 2026 tessera core
// See COPYING for license.

#ifndef TESSERA_COMMON_URL_H
#define TESSERA_COMMON_URL_H

#include <string>
#include <string_view>

/* Decode a URL.
 *
 * Notably this implementation does not decode a '+' to a ' '.
 */
std::string UrlDecode(std::string_view url_encoded);

/* Encode a URL. */
std::string UrlEncode(std::string_view str);

#endif // TESSERA_COMMON_URL_H

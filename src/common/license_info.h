// Copyright (c) 2026 tessera core
// See COPYING for license.

#ifndef TESSERA_COMMON_LICENSE_INFO_H
#define TESSERA_COMMON_LICENSE_INFO_H

#include <string>

std::string CopyrightHolders(const std::string& strPrefix);

/** Returns licensing information (for -version) */
std::string LicenseInfo();

#endif // TESSERA_COMMON_LICENSE_INFO_H

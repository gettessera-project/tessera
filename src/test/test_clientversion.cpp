// Copyright (c) 2026 tessera core
// See COPYING for license.
//
// clientversion (version constants + BIP14 user-agent formatting) tests.
// Standalone for now (own main); folds into the test framework when it lands.

#include <clientversion.h>

#include <cstdio>
#include <string>
#include <vector>

namespace {
int g_fail = 0;
void Check(const char* name, bool ok)
{
    std::printf("[%s] %s\n", ok ? "PASS" : "FAIL", name);
    if (!ok) ++g_fail;
}
} // namespace

int main()
{
    Check("CLIENT_VERSION == 0.1.0 packed", CLIENT_VERSION == 100); // 10000*0 + 100*1 + 0
    Check("UA_NAME", UA_NAME == "Tessera");
    Check("CLIENT_NAME", std::string(CLIENT_NAME) == "Tessera");
    Check("FormatFullVersion contains 0.1.0", FormatFullVersion().find("0.1.0") != std::string::npos);
    Check("FormatSubVersion", FormatSubVersion("Tessera", CLIENT_VERSION, {}) == "/Tessera:0.1.0/");
    Check("FormatSubVersion with comments",
          FormatSubVersion("Tessera", CLIENT_VERSION, {"a", "b"}) == "/Tessera:0.1.0(a; b)/");

    std::printf("%s\n", g_fail == 0 ? "ALL PASS" : "FAILURES");
    return g_fail == 0 ? 0 : 1;
}

// Copyright (c) 2026 tessera core
// See COPYING for license.

#include <interfaces/ipc.h>

#include <memory>

namespace interfaces {
std::unique_ptr<Ipc> MakeIpc(const char* exe_name, const char* process_argv0, Init& init)
{
    return {};
}
} // namespace interfaces

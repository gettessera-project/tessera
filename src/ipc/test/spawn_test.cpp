// Copyright (c) 2026 tessera core
// See COPYING for license.
//
// Multiprocess integration smoke: spawn the tessera-node binary as an IPC
// server child (tessera-node -ipcfd <fd>) and call interfaces::Init across the
// process boundary — makeEcho() then Echo::echo(). Proves the whole vertical:
// ipc::Process::spawn + the Cap'n Proto Protocol + the TesseraNodeInit server.
//
// Not a ctest: spawn() execs ./tessera-node next to argv[0], so this must be
// run from the build directory with the tessera-node binary present. Built only
// under ENABLE_IPC (unix).

#include <interfaces/echo.h>
#include <interfaces/init.h>
#include <ipc/capnp/protocol.h>
#include <ipc/process.h>
#include <ipc/protocol.h>
#include <util/fs.h>

#include <cstdio>
#include <memory>
#include <string>

int main(int argc, char* argv[])
{
    const std::string msg{"hello across processes"};

    std::unique_ptr<ipc::Process> process{ipc::MakeProcess()};
    std::unique_ptr<ipc::Protocol> protocol{ipc::capnp::MakeCapnpProtocol()};

    int pid{0};
    int fd{process->spawn("tessera-node", fs::PathFromString(argv[0]), pid)};
    std::printf("spawned tessera-node pid %i\n", pid);

    std::unique_ptr<interfaces::Init> init{protocol->connect(fd, "tessera-spawn-test")};
    std::unique_ptr<interfaces::Echo> echo{init->makeEcho()};
    std::string result{echo->echo(msg)};
    std::printf("echo across process: \"%s\"\n", result.c_str());

    echo.reset();
    init.reset();
    int status{process->waitSpawned(pid)};
    std::printf("child exit status: %i\n", status);

    bool ok{result == msg};
    std::printf("%s\n", ok ? "SPAWN ROUND-TRIP PASS" : "SPAWN ROUND-TRIP FAIL");
    return ok ? 0 : 1;
}

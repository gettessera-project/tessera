# Copyright (c) 2026 tessera core
# See COPYING for license.

# The multiprocess IPC library: the Cap'n Proto protocol + process framework,
# plus the schemas compiled to capnp + proxy C++ by mpgen (target_capnp_sources,
# from the vendored libmultiprocess). EXCLUDE_FROM_ALL — pulled in by the
# binaries that link it.
add_library(tessera_ipc STATIC EXCLUDE_FROM_ALL
  capnp/mining.cpp
  capnp/protocol.cpp
  interfaces.cpp
  process.cpp
)

target_capnp_sources(tessera_ipc ${CMAKE_SOURCE_DIR}/src
  capnp/common.capnp
  capnp/echo.capnp
  capnp/init.capnp
  capnp/mining.capnp
  capnp/rpc.capnp
  IMPORT_PATHS ${CMAKE_SOURCE_DIR}/src/ipc/libmultiprocess/include
)

target_link_libraries(tessera_ipc
  PRIVATE
    core_interface
    tessera_node
    Libmultiprocess::multiprocess
)

# IPC round-trip test: drives the multiprocess runtime in-process (pipe +
# socketpair + unix socket) and the type-conversion hooks via a FooInterface
# schema. A standalone test main (Tessera style), built and registered only
# here — the whole file is reached only under ENABLE_IPC (unix).
add_executable(test_ipc
  test/ipc_test.cpp
  test/test_ipc.cpp
)
target_capnp_sources(test_ipc ${CMAKE_SOURCE_DIR}/src
  test/ipc_test.capnp
  IMPORT_PATHS ${CMAKE_SOURCE_DIR}/src/ipc/libmultiprocess/include
)
target_link_libraries(test_ipc
  PRIVATE
    core_interface
    tessera_ipc
    tessera_node
    Libmultiprocess::multiprocess
)
add_test(NAME test_ipc COMMAND test_ipc)

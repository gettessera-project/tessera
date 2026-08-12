# Copyright (c) 2026 tessera core
# See COPYING for license.
#
# Library decomposition mirroring Bitcoin Core's src/CMakeLists.txt: the former
# monolithic tessera_core is split into a clean dependency DAG of static libs
#   crypto  <-  util  <-  consensus  <-  common  <-  node
#                                   wallet  <-  common
#                                   cli (standalone)
# All carry the shared flags/includes via the core_interface INTERFACE library
# (defined in the top-level CMakeLists). The build config header is generated
# next to this file.

include(AddWindowsResources)
include(InstallBinaryComponent)

# univalue — the JSON library (vendored under src/univalue), used by every layer
# that speaks RPC/JSON.
add_subdirectory(univalue)

# clientversion — depends on the generated tessera-build-info.h.
add_library(tessera_clientversion STATIC EXCLUDE_FROM_ALL
  clientversion.cpp
)
target_link_libraries(tessera_clientversion PRIVATE core_interface)
add_dependencies(tessera_clientversion generate_build_info)

add_subdirectory(crypto)
add_subdirectory(util)

# Consensus-critical code (block/tx validation primitives, script interpreter).
add_library(tessera_consensus STATIC EXCLUDE_FROM_ALL
  arith_uint256.cpp
  consensus/merkle.cpp
  consensus/tx_check.cpp
  hash.cpp
  primitives/block.cpp
  primitives/transaction.cpp
  pubkey.cpp
  script/interpreter.cpp
  script/script.cpp
  script/script_error.cpp
  uint256.cpp
)
target_link_libraries(tessera_consensus
  PRIVATE
    core_interface
    tessera_crypto
  PUBLIC
    # pubkey.cpp verifies ML-DSA-44 (FIPS 204) signatures in CHECKSIG.
    mldsa44
)

if(WITH_ZMQ)
  add_subdirectory(zmq)
endif()

# Higher-level functionality shared by the executables and libraries.
add_library(tessera_common STATIC EXCLUDE_FROM_ALL
  addresstype.cpp
  bech32.cpp
  chain.cpp
  chainparams.cpp
  chainparamsbase.cpp
  coins.cpp
  common/args.cpp
  common/bloom.cpp
  common/config.cpp
  common/init.cpp
  common/interfaces.cpp
  common/license_info.cpp
  common/messages.cpp
  common/netif.cpp
  common/settings.cpp
  common/signmessage.cpp
  common/system.cpp
  common/url.cpp
  compressor.cpp
  core_io.cpp
  deploymentinfo.cpp
  init/common.cpp
  kernel/chainparams.cpp
  key.cpp
  key_io.cpp
  merkleblock.cpp
  net_permissions.cpp
  net_types.cpp
  netaddress.cpp
  netbase.cpp
  outputtype.cpp
  policy/feerate.cpp
  policy/policy.cpp
  pow.cpp
  protocol.cpp
  rpc/rawtransaction_util.cpp
  rpc/request.cpp
  rpc/util.cpp
  scheduler.cpp
  script/sign.cpp
  script/signingprovider.cpp
  script/solver.cpp
)
target_link_libraries(tessera_common
  PRIVATE
    core_interface
    tessera_consensus
    tessera_util
    univalue
  PUBLIC
    $<$<PLATFORM_ID:Windows>:ws2_32>
)

if(ENABLE_WALLET)
  add_subdirectory(wallet)

  # tessera-wallet — offline wallet tool (no running node).
  add_executable(tessera-wallet
    tessera-wallet.cpp
    wallet/wallettool.cpp
  )
  target_link_libraries(tessera-wallet
    core_interface
    tessera_wallet
    tessera_common
    tessera_util
  )
endif()

# P2P and RPC server functionality used by tesserad and tessera-qt.
add_library(tessera_node STATIC EXCLUDE_FROM_ALL
  addrdb.cpp
  addrman.cpp
  banman.cpp
  blockencodings.cpp
  blockfilter.cpp
  consensus/tx_verify.cpp
  dbwrapper.cpp
  deploymentstatus.cpp
  flatfile.cpp
  headerssync.cpp
  httprpc.cpp
  httpserver.cpp
  i2p.cpp
  index/base.cpp
  index/blockfilterindex.cpp
  index/coinstatsindex.cpp
  index/txindex.cpp
  index/txospenderindex.cpp
  init.cpp
  kernel/chain.cpp
  kernel/checks.cpp
  kernel/coinstats.cpp
  kernel/context.cpp
  kernel/cs_main.cpp
  kernel/disconnected_transactions.cpp
  kernel/mempool_removal_reason.cpp
  mapport.cpp
  net.cpp
  net_processing.cpp
  netgroup.cpp
  node/abort.cpp
  node/blockmanager_args.cpp
  node/blockstorage.cpp
  node/caches.cpp
  node/chainstate.cpp
  node/chainstatemanager_args.cpp
  node/coin.cpp
  node/coins_view_args.cpp
  node/connection_types.cpp
  node/context.cpp
  node/database_args.cpp
  node/eviction.cpp
  node/interface_ui.cpp
  node/interfaces.cpp
  node/kernel_notifications.cpp
  node/mempool_args.cpp
  node/mempool_persist.cpp
  node/mempool_persist_args.cpp
  node/mini_miner.cpp
  node/miner.cpp
  node/mining_args.cpp
  node/peerman_args.cpp
  node/timeoffsets.cpp
  node/transaction.cpp
  node/txdownloadman_impl.cpp
  node/txorphanage.cpp
  node/txreconciliation.cpp
  node/utxo_snapshot.cpp
  node/warnings.cpp
  noui.cpp
  policy/ephemeral_policy.cpp
  policy/fees/block_policy_estimator.cpp
  policy/fees/block_policy_estimator_args.cpp
  policy/packages.cpp
  policy/rbf.cpp
  policy/settings.cpp
  policy/truc_policy.cpp
  private_broadcast.cpp
  rest.cpp
  rpc/blockchain.cpp
  rpc/fees.cpp
  rpc/mempool.cpp
  rpc/mining.cpp
  rpc/net.cpp
  rpc/node.cpp
  rpc/output_script.cpp
  rpc/rawtransaction.cpp
  rpc/server.cpp
  rpc/server_util.cpp
  rpc/signmessage.cpp
  rpc/txoutproof.cpp
  script/sigcache.cpp
  torcontrol.cpp
  txdb.cpp
  txgraph.cpp
  txmempool.cpp
  txrequest.cpp
  v2cipher.cpp
  validation.cpp
  validationinterface.cpp
  versionbits.cpp
  $<$<TARGET_EXISTS:tessera_wallet>:wallet/init.cpp>
)
target_link_libraries(tessera_node
  PRIVATE
    core_interface
    tessera_common
    tessera_util
    $<TARGET_NAME_IF_EXISTS:tessera_zmq>
    leveldb
    minisketch
    univalue
  # Public: txmempool.h includes <boost/multi_index/...>, so every consumer of
  # this library's headers needs the include path too. As PRIVATE it only ever
  # worked because the host keeps Boost in a default search path.
  PUBLIC
    Boost::headers
)

# tesserad — the headless node daemon. Links its own MakeNodeInit
# (init/tesserad.cpp, no IPC); the IPC tessera-node below links a different one.
add_executable(tesserad tesserad.cpp init/tesserad.cpp)
add_windows_resources(tesserad tesserad.rc)
target_link_libraries(tesserad
  core_interface
  tessera_node
  $<TARGET_NAME_IF_EXISTS:tessera_wallet>
)

# Multiprocess IPC (unix only). The tessera-node binary is tesserad with the
# IPC-enabled MakeNodeInit; the stub MakeIpc is folded into the node lib otherwise.
if(ENABLE_IPC)
  # Build the vendored libmultiprocess (defines the Libmultiprocess::multiprocess
  # and ::mpgen targets and the MP_INCLUDE_DIR property), then load its
  # target_capnp_sources() helper. The helper is shipped for downstream
  # find_package() consumers, so for this vendored (add_subdirectory) build it
  # must be include()'d explicitly before src/ipc uses it.
  add_subdirectory(ipc/libmultiprocess EXCLUDE_FROM_ALL)
  include(${CMAKE_CURRENT_SOURCE_DIR}/ipc/libmultiprocess/cmake/TargetCapnpSources.cmake)
  add_subdirectory(ipc)
  add_executable(tessera-node tesserad.cpp init/tessera-node.cpp)
  target_link_libraries(tessera-node core_interface tessera_node tessera_ipc $<TARGET_NAME_IF_EXISTS:tessera_wallet>)
  add_executable(ipc_spawn_test EXCLUDE_FROM_ALL ipc/test/spawn_test.cpp)
  add_dependencies(ipc_spawn_test tessera-node)
  target_link_libraries(ipc_spawn_test PRIVATE core_interface tessera_node tessera_ipc)
else()
  target_sources(tessera_node PRIVATE ipc/stub.cpp)
endif()

# RPC command-line client library + the tessera-cli executable.
add_library(tessera_cli STATIC EXCLUDE_FROM_ALL
  compat/stdin.cpp
  rpc/client.cpp
)
target_link_libraries(tessera_cli PUBLIC core_interface univalue)

add_executable(tessera-cli tessera-cli.cpp)
add_windows_resources(tessera-cli tessera-cli.rc)
target_link_libraries(tessera-cli core_interface tessera_cli tessera_common tessera_node tessera_util)

# tessera-tx — offline raw-transaction tool.
add_executable(tessera-tx tessera-tx.cpp)
add_windows_resources(tessera-tx tessera-tx.rc)
target_link_libraries(tessera-tx core_interface tessera_common tessera_util univalue)

# tessera-util — offline block-header utility: grind the SHA3-256d proof-of-work
# of a hex block header (and netmagic). The Tessera analogue of bitcoin-util.
add_executable(tessera-util tessera-util.cpp)
add_windows_resources(tessera-util tessera-util.rc)
target_link_libraries(tessera-util core_interface tessera_common tessera_consensus tessera_util univalue)

# tessera-miner — standalone CPU miner. Talks to a tesserad over JSON-RPC
# (getblocktemplate/submitblock) and grinds the node's SHA3-256d PoW on all
# cores, bit-identical to the node's own miner. The node's generatetoaddress is
# single-threaded and gives up after a million tries, so it cannot serve as one.
add_executable(tessera-miner tessera-miner.cpp)
add_windows_resources(tessera-miner tessera-miner.rc)
target_link_libraries(tessera-miner
  core_interface
  tessera_node
  tessera_common
  tessera_util
  univalue
)

# Windows: embed the UTF-8 active-code-page manifest on tessera-wallet too.
if(WIN32 AND TARGET tessera-wallet)
  add_windows_resources(tessera-wallet tessera-wallet.rc)
endif()

# Qt GUI (tessera-qt). Opt-in (-DENABLE_GUI=ON); links tessera_node directly.
if(ENABLE_GUI)
  find_package(Qt MODULE REQUIRED)
  add_subdirectory(qt)
endif()

# Test fixtures (always available for benches/fixture tests), the unit tests,
# and the benchmarks each live in their own subdirectory CMakeLists, mirroring
# Bitcoin Core's src/{test/util,test,bench} layout.
add_subdirectory(test/util)
add_subdirectory(test)
add_subdirectory(bench)

# tools/mine_genesis — offline genesis miner (built on demand). Tessera-specific
# (no Core analog), so it stays at the src level.
add_executable(mine_genesis EXCLUDE_FROM_ALL ${CMAKE_SOURCE_DIR}/tools/mine_genesis.cpp)
target_link_libraries(mine_genesis PRIVATE core_interface tessera_node)

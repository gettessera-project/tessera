# Copyright (c) 2026 tessera core
# See COPYING for license.

# Wallet functionality used by tesserad and tessera-wallet executables.
add_library(tessera_wallet STATIC EXCLUDE_FROM_ALL
  coincontrol.cpp
  coinselection.cpp
  context.cpp
  crypter.cpp
  db.cpp
  dump.cpp
  feebumper.cpp
  fees.cpp
  interfaces.cpp
  load.cpp
  receive.cpp
  rpc/addresses.cpp
  rpc/backup.cpp
  rpc/coins.cpp
  rpc/encrypt.cpp
  rpc/signmessage.cpp
  rpc/spend.cpp
  rpc/transactions.cpp
  rpc/util.cpp
  rpc/wallet.cpp
  spend.cpp
  sqlite.cpp
  transaction.cpp
  wallet.cpp
  walletdb.cpp
  walletutil.cpp
)
# Locate SQLite rather than naming the library bare. A bare "sqlite3" links
# on a native build only because the header happens to sit in the default
# include path; a cross-build has no such luck and fails on sqlite3.h.
find_path(SQLite3_INCLUDE_DIR NAMES sqlite3.h REQUIRED)
find_library(SQLite3_LIBRARY NAMES sqlite3 REQUIRED)

target_link_libraries(tessera_wallet
  PRIVATE
    core_interface
    tessera_common
    ${SQLite3_LIBRARY}
    univalue
    Boost::headers
)
target_include_directories(tessera_wallet PRIVATE ${SQLite3_INCLUDE_DIR})

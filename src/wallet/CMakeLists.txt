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
target_link_libraries(tessera_wallet
  PRIVATE
    core_interface
    tessera_common
    sqlite3
    univalue
    Boost::headers
)

# Copyright (c) 2026 tessera core
# See COPYING for license.

add_library(tessera_util STATIC EXCLUDE_FROM_ALL
  asmap.cpp
  batchpriority.cpp
  bytevectorhash.cpp
  chaintype.cpp
  check.cpp
  exception.cpp
  feefrac.cpp
  fs.cpp
  fs_helpers.cpp
  hasher.cpp
  moneystr.cpp
  rbf.cpp
  readwritefile.cpp
  serfloat.cpp
  signalinterrupt.cpp
  sock.cpp
  strencodings.cpp
  string.cpp
  syserror.cpp
  thread.cpp
  threadinterrupt.cpp
  threadnames.cpp
  time.cpp
  tokenpipe.cpp
  ../logging.cpp
  ../random.cpp
  ../randomenv.cpp
  ../streams.cpp
  ../sync.cpp
)

target_link_libraries(tessera_util
  PRIVATE
    core_interface
    tessera_clientversion
    tessera_crypto
    $<$<PLATFORM_ID:Windows>:ws2_32>
    $<$<PLATFORM_ID:Windows>:iphlpapi>
    $<$<PLATFORM_ID:Windows>:bcrypt>
)

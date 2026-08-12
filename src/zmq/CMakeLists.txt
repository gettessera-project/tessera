# Copyright (c) 2026 tessera core
# See COPYING for license.

add_library(tessera_zmq STATIC EXCLUDE_FROM_ALL
  zmqabstractnotifier.cpp
  zmqnotificationinterface.cpp
  zmqpublishnotifier.cpp
  zmqrpc.cpp
  zmqutil.cpp
)
target_compile_definitions(tessera_zmq
  INTERFACE
    ENABLE_ZMQ=1
)
target_link_libraries(tessera_zmq
  PRIVATE
    core_interface
    univalue
    zeromq
)

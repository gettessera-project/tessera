# Copyright (c) 2026 tessera core
# See COPYING for license.

# The Qt Widgets GUI library (tessera-qt). Mirrors Bitcoin Core's src/qt: a
# static tesseraqt lib parsed for translatable strings and linked into the
# tessera-qt executable, which links tessera_core directly (no RPC). Sources
# are added here as they are ported, bottom-up.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS forms)
set(CMAKE_AUTORCC OFF)

# Icon/animation/font resources (res/, mapped to :/icons/* etc. by tessera.qrc).
qt6_add_resources(TESSERA_QRC tessera.qrc)

# Translations: tessera_locale.qrc bundles the .qm files that locale/CMakeLists.txt
# compiles from the .ts via lrelease into the binary dir, so the qrc is copied next
# to them (configure_file) and bundled from there (mapped to :/translations/<code>).
configure_file(tessera_locale.qrc tessera_locale.qrc USE_SOURCE_PERMISSIONS COPYONLY)
qt6_add_resources(TESSERA_LOCALE_QRC ${CMAKE_CURRENT_BINARY_DIR}/tessera_locale.qrc)

add_library(tesseraqt STATIC EXCLUDE_FROM_ALL
  ${TESSERA_QRC}
  ${TESSERA_LOCALE_QRC}
  addresstablemodel.cpp
  addresstablemodel.h
  addressbookpage.cpp
  addressbookpage.h
  askpassphrasedialog.cpp
  askpassphrasedialog.h
  bantablemodel.cpp
  bantablemodel.h
  clientmodel.cpp
  clientmodel.h
  csvmodelwriter.cpp
  csvmodelwriter.h
  editaddressdialog.cpp
  editaddressdialog.h
  tesseraaddressvalidator.cpp
  tesseraaddressvalidator.h
  tesseraamountfield.cpp
  tesseraamountfield.h
  tesseraunits.cpp
  tesseraunits.h
  guiconstants.h
  tessera.cpp
  tessera.h
  tesseragui.cpp
  tesseragui.h
  initexecutor.cpp
  initexecutor.h
  winshutdownmonitor.cpp
  winshutdownmonitor.h
  freespacechecker.cpp
  freespacechecker.h
  intro.cpp
  intro.h
  modaloverlay.cpp
  modaloverlay.h
  notificator.cpp
  notificator.h
  splashscreen.cpp
  splashscreen.h
  trafficgraphwidget.cpp
  trafficgraphwidget.h
  utilitydialog.cpp
  utilitydialog.h
  guiutil.cpp
  guiutil.h
  networkstyle.cpp
  networkstyle.h
  openuridialog.cpp
  openuridialog.h
  optionsdialog.cpp
  optionsdialog.h
  optionsmodel.cpp
  optionsmodel.h
  peertablemodel.cpp
  peertablemodel.h
  peertablesortproxy.cpp
  peertablesortproxy.h
  platformstyle.cpp
  platformstyle.h
  qvalidatedlineedit.cpp
  qvalidatedlineedit.h
  qvaluecombobox.cpp
  qvaluecombobox.h
  qrimagewidget.cpp
  qrimagewidget.h
  rpcconsole.cpp
  rpcconsole.h
  receivecoinsdialog.cpp
  receivecoinsdialog.h
  receiverequestdialog.cpp
  receiverequestdialog.h
  recentrequeststablemodel.cpp
  recentrequeststablemodel.h
  sendcoinsrecipient.h
  coincontroltreewidget.cpp
  coincontroltreewidget.h
  coincontroldialog.cpp
  coincontroldialog.h
  sendcoinsdialog.cpp
  sendcoinsdialog.h
  sendcoinsentry.cpp
  sendcoinsentry.h
  signverifymessagedialog.cpp
  signverifymessagedialog.h
  transactiondescdialog.cpp
  transactiondescdialog.h
  transactionfilterproxy.cpp
  transactionfilterproxy.h
  transactionoverviewwidget.cpp
  transactionoverviewwidget.h
  overviewpage.cpp
  overviewpage.h
  transactionview.cpp
  transactionview.h
  transactiondesc.cpp
  transactiondesc.h
  transactionrecord.cpp
  transactionrecord.h
  transactiontablemodel.cpp
  transactiontablemodel.h
  createwalletdialog.cpp
  createwalletdialog.h
  walletcontroller.cpp
  walletcontroller.h
  walletframe.cpp
  walletframe.h
  walletview.cpp
  walletview.h
  walletmodel.cpp
  walletmodel.h
  walletmodeltransaction.cpp
  walletmodeltransaction.h
)

target_compile_definitions(tesseraqt
  PUBLIC
    QT_NO_KEYWORDS
    QT_USE_QSTRINGBUILDER
    # Tessera always builds the wallet into tessera_core, so the GUI's
    # wallet code paths (WalletModel, wallet tab, load-wallet hooks) are
    # unconditionally active.
    ENABLE_WALLET
)

target_include_directories(tesseraqt
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>
)

target_link_libraries(tesseraqt
  PUBLIC
    Qt6::Widgets
  PRIVATE
    core_interface
    tessera_node
    tessera_wallet
)

if(WIN32)
  # guiutil.cpp uses PathRemoveFileSpecW from the Windows shell-path API.
  target_link_libraries(tesseraqt PRIVATE shlwapi)
endif()

# Compile the .ts translations to .qm via lrelease (depends on tesseraqt).
add_subdirectory(locale)

# ---------------------------------------------------------------------------
# The tessera-qt executable. main.cpp holds the entry point (G_TRANSLATION_FUN
# + GuiMain) and init/tessera-qt.cpp provides MakeGuiInit (the GUI's in-process
# node init). qt6_add_executable sets the WIN32/macOS bundle properties and links
# the Qt entry-point library (which calls our dllexport'd main on Windows).
qt6_add_executable(tessera-qt
  main.cpp
  ${CMAKE_SOURCE_DIR}/src/init/tessera-qt.cpp
)
target_link_libraries(tessera-qt
  PRIVATE
    core_interface
    tesseraqt
    tessera_node
    tessera_wallet
    # rpcconsole.cpp uses RPCConvertValues from the CLI lib (rpc/client.cpp).
    tessera_cli
)

# Build as a Windows GUI-subsystem app (no console window). Qt's entry-point
# library supplies WinMain, which calls our dllexport'd main (see MAIN_FUNCTION).
set_target_properties(tessera-qt PROPERTIES WIN32_EXECUTABLE TRUE)

if(WIN32)
  # Embed the UTF-8 active-code-page manifest, as the other binaries do.
  # OBJECT_DEPENDS so windres re-embeds the icons when the .ico files change
  # (the .rc ICON statements are not tracked as dependencies otherwise).
  set_source_files_properties(${CMAKE_SOURCE_DIR}/src/tessera-qt.rc
    PROPERTIES INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/src
    OBJECT_DEPENDS "${CMAKE_SOURCE_DIR}/src/qt/res/icons/tessera.ico;${CMAKE_SOURCE_DIR}/src/qt/res/icons/tessera_testnet.ico;${CMAKE_SOURCE_DIR}/src/qt/res/icons/tessera_regtest.ico")
  target_sources(tessera-qt PRIVATE ${CMAKE_SOURCE_DIR}/src/tessera-qt.rc)
endif()

# ---------------------------------------------------------------------------
# tessera-gui — the multiprocess GUI: same GuiMain, but its MakeGuiInit
# (init/tessera-gui.cpp) connects to a separate tessera-node process over IPC
# instead of running the node in-process. IPC is unix-only (ENABLE_IPC is forced
# off on Windows), so this binary only builds on unix-likes with the GUI enabled.
if(ENABLE_IPC)
  qt6_add_executable(tessera-gui
    main.cpp
    ${CMAKE_SOURCE_DIR}/src/init/tessera-gui.cpp
  )
  target_link_libraries(tessera-gui
    PRIVATE
      core_interface
      tesseraqt
      tessera_node
      tessera_wallet
      tessera_cli
      tessera_ipc
  )
endif()

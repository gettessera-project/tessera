# Copyright (c) 2026 tessera core
# See COPYING for license.

include(ts_files.cmake)
qt6_add_lrelease(TS_FILES ${ts_files} LRELEASE_TARGET tesseraqt_lrelease OPTIONS -silent)
add_dependencies(tesseraqt tesseraqt_lrelease)

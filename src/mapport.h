// Copyright (c) 2026 tessera core
// See COPYING for license.

#ifndef TESSERA_MAPPORT_H
#define TESSERA_MAPPORT_H

static constexpr bool DEFAULT_NATPMP = false;

void StartMapPort(bool enable);
void InterruptMapPort();
void StopMapPort();

#endif // TESSERA_MAPPORT_H

// Copyright (c) 2026 tessera core
// See COPYING for license.

#ifndef TESSERA_COMPAT_STDIN_H
#define TESSERA_COMPAT_STDIN_H

struct NoechoInst {
    NoechoInst();
    ~NoechoInst();
};

#define NO_STDIN_ECHO() NoechoInst _no_echo

bool StdinTerminal();
bool StdinReady();

#endif // TESSERA_COMPAT_STDIN_H

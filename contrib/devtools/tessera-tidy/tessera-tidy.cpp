// Copyright (c) 2026 tessera core
// See COPYING for license.

#include "nontrivial-threadlocal.h"

#include <clang-tidy/ClangTidyModule.h>

class TesseraModule final : public clang::tidy::ClangTidyModule
{
public:
    void addCheckFactories(clang::tidy::ClangTidyCheckFactories& CheckFactories) override
    {
        CheckFactories.registerCheck<tessera::NonTrivialThreadLocal>("tessera-nontrivial-threadlocal");
    }
};

static clang::tidy::ClangTidyModuleRegistry::Add<TesseraModule>
    X("tessera-module", "Adds tessera checks.");

volatile int TesseraModuleAnchorSource = 0;

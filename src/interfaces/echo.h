// Copyright (c) 2026 tessera core
// See COPYING for license.

#ifndef TESSERA_INTERFACES_ECHO_H
#define TESSERA_INTERFACES_ECHO_H

#include <memory>
#include <string>

namespace interfaces {
//! Simple string echoing interface for testing.
class Echo
{
public:
    virtual ~Echo() = default;

    //! Echo provided string.
    virtual std::string echo(const std::string& echo) = 0;
};

//! Return implementation of Echo interface.
std::unique_ptr<Echo> MakeEcho();
} // namespace interfaces

#endif // TESSERA_INTERFACES_ECHO_H
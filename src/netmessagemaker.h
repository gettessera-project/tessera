// Copyright (c) 2026 tessera core
// See COPYING for license.

#ifndef TESSERA_NETMESSAGEMAKER_H
#define TESSERA_NETMESSAGEMAKER_H

#include <net.h>
#include <serialize.h>

namespace NetMsg {
    template <typename... Args>
    CSerializedNetMsg Make(std::string msg_type, Args&&... args)
    {
        CSerializedNetMsg msg;
        msg.m_type = std::move(msg_type);
        VectorWriter{msg.data, 0, std::forward<Args>(args)...};
        return msg;
    }
} // namespace NetMsg

#endif // TESSERA_NETMESSAGEMAKER_H
// Copyright (c) 2026 tessera core
// See COPYING for license.

#ifndef TESSERA_INDEX_DISKTXPOS_H
#define TESSERA_INDEX_DISKTXPOS_H

#include <flatfile.h>
#include <serialize.h>

struct CDiskTxPos : public FlatFilePos
{
    uint32_t nTxOffset{0}; // after header

    SERIALIZE_METHODS(CDiskTxPos, obj)
    {
        READWRITE(AsBase<FlatFilePos>(obj), VARINT(obj.nTxOffset));
    }

    CDiskTxPos(const FlatFilePos& blockIn, uint32_t nTxOffsetIn) : FlatFilePos{blockIn.nFile, blockIn.nPos}, nTxOffset{nTxOffsetIn} {
    }

    CDiskTxPos() = default;
};

#endif // TESSERA_INDEX_DISKTXPOS_H
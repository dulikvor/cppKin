#pragma once

#include "Core/src/Enumeration.h"

class EncodingTypes
{
public:
    enum Enumeration
    {
        Thrift,
        ByteStream,
        Json
    };
    ENUMERATION(EncodingTypes)
};

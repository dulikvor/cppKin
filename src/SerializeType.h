#pragma once

#include "Core/src/Enumeration.h"

class SerializeType
{
public:
    enum Enumeration
    {
        Thrift,
        ByteStream
    };
    ENUMERATION(SerializeType)
};

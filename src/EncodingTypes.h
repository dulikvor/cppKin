#pragma once

#include "Core/src/Enumeration.h"

namespace cppkin
{
    class EncodingType
    {
    public:
        enum Enumeration
        {
            Thrift,
            Json
        };
        ENUMERATION(EncodingType)
    };
}

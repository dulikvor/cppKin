#pragma once

#include "Core/src/Enumeration.h"
#include "Core/src/Export.h"

namespace cppkin
{
    class A_EXPORT EncodingType
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

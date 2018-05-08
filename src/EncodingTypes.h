#pragma once

#include "Core/src/Enumeration.h"
#include "Export.h"

namespace cppkin
{
    class CPPKIN_EXPORT EncodingType
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

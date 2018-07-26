#pragma once

#include "core/Enumeration.h"
#include "Export.h"

namespace cppkin
{
    class CPPKIN_EXPORT TransportType
    {
    public:
        enum Enumeration
        {
            Stub,
            Scribe,
            Http
        };
        ENUMERATION(TransportType)
    };
}

#pragma once

#include "Core/src/Enumeration.h"
#include "Export.h"
#if defined(WIN32)
#pragma warning (disable : 4251)
#endif

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

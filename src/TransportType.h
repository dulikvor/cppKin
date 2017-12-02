#pragma once

#include "Core/src/Enumeration.h"
#include "Core\src\Export.h"
#if defined(WIN32)
#pragma warning (disable : 4251)
#endif

namespace cppkin
{
	class A_EXPORT TransportType
    {
    public:
        enum Enumeration
        {
            Scribe
        };
        ENUMERATION(TransportType)
    };
}

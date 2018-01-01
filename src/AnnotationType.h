#pragma once

#include "Core/src/Enumeration.h"
#include "Core/src/Export.h"

namespace cppkin
{
    class A_EXPORT AnnotationType
    {
    public:
        enum Enumeration
        {
            Simple,
            Binary
        };
    ENUMERATION(AnnotationType)
    };
}

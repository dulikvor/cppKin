#pragma once

#include "Core/src/Enumeration.h"

namespace cppkin
{
    class AnnotationType
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

#pragma once

#include "Export.h"
#include "core/GeneralParams.h"

namespace cppkin
{
    typedef core::GeneralParams CppkinParams;
    CPPKIN_EXPORT void Init(const core::GeneralParams& params);
}
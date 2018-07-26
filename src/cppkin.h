#pragma once

#include <memory>
#include "Export.h"
#include "core/GeneralParams.h"
#include "Control.h"
#include "ConfigParams.h"
#include "ConfigTags.h"
#include "SpanContainer.h"
#include "Trace.h"
#include "Span.h"

namespace cppkin {
    typedef core::GeneralParams CppkinParams;

    CPPKIN_EXPORT void PushSpan(Span& span);
    CPPKIN_EXPORT Span* const PopSpan();
}

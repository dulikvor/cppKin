#pragma once

#include <memory>
#include "Export.h"
#include "core/GeneralParams.h"
#include "ConfigTags.h"
#include "Trace.h"
#include "Span.h"
#if defined(WIN32)
#pragma warning( push )
#pragma warning (disable : 4251)
#endif

namespace cppkin {
    typedef core::GeneralParams CppkinParams;

    CPPKIN_EXPORT void Init(const core::GeneralParams& params);
    CPPKIN_EXPORT void Stop();
    //Using the tls
    CPPKIN_EXPORT void PushSpan(Span& span);
    CPPKIN_EXPORT Span& TopSpan();
    CPPKIN_EXPORT void PopSpan();
    CPPKIN_EXPORT bool IsContainerEmpty();
}
#if defined(WIN32)
#pragma warning( pop )
#endif

#pragma once

#include <memory>
#include <string>
#include "Export.h"
#include "Span.h"
#if defined(WIN32)
#pragma warning( push )
#pragma warning (disable : 4251)
#endif

namespace cppkin
{
    class span_impl;

    class CPPKIN_EXPORT Trace : public Span
    {
    public:
        Trace(const char *operationName, const char* value = Annotation::Value::SERVER_RECEIVE);
    };
}
#if defined(WIN32)
#pragma warning( pop )
#endif
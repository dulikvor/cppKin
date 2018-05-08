#pragma once

#include <memory>
#include <string>
#include "Export.h"
#include "Span.h"

namespace cppkin
{
    class span_impl;

    class CPPKIN_EXPORT Trace : public Span
    {
    public:
        Trace(const char *operationName, const char* value = Annotation::Value::SERVER_RECEIVE);
    };
}

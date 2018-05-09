#pragma once

#include <cstdint>
#include <memory>
#include "Annotation.h"
#include "Export.h"

namespace cppkin
{
    class Trace;
    class span_impl;

    class CPPKIN_EXPORT Span
    {
    public:
        Span CreateSpan(const char* operationName, const char* value = Annotation::Value::SERVER_RECEIVE) const;
        void AddAnnotation(const char* value);
        void AddAnnotation(const char* value, int_fast64_t timeStamp);
        void Submit();
        bool IsSampled() const;
    protected:
        Span(const char* operationName, uint_fast64_t traceId, bool sampled);
        Span(const char* operationName, uint_fast64_t traceId, uint_fast64_t parentId, bool sampled);

    protected:
        std::shared_ptr<span_impl> m_span;
    };
}

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
        Span CreateSpan(const char* operationName, const char* value = Annotation::Value::SERVER_RECEIVE);
        void AddAnnotation(const char* value);
        void Submit(const char* value = Annotation::Value::CLIENT_SEND);
    protected:
        Span(const char* operationName, uint_fast64_t traceId, bool sampled);
        Span(const char* operationName, uint_fast64_t traceId, uint_fast64_t parentId, bool sampled);

    private:
        std::shared_ptr<span_impl> m_span;
    };
}

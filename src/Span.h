#pragma once

#include <cstdint>
#include <memory>
#include "span_impl.h"
#include "Annotation.h"
#include "Export.h"
#if defined(WIN32)
#pragma warning( push )
#pragma warning (disable : 4251)
#endif

namespace cppkin
{
    class Trace;

    class CPPKIN_EXPORT Span
    {
    public:
        Span();
        Span CreateSpan(const char* operationName, const char* value = Annotation::Value::SERVER_RECEIVE) const;
        void Join(const char* operationName, uint_fast64_t traceId, uint_fast64_t parentId, uint_fast64_t id, bool sampled);
        void Join(const std::string& b3format);
        void AddAnnotation(const char* value);
        void AddAnnotation(const char* value, int_fast64_t timeStamp);
        void Submit(const char* value = Annotation::Value::SERVER_SEND);
        bool IsSampled() const;
        std::string GetHeaderB3Format() const;
        const span_impl::SpanHeader&  GetHeader() const;
    protected:
        Span(const char* operationName, uint_fast64_t traceId, bool sampled);
        Span(const char* operationName, uint_fast64_t traceId, uint_fast64_t parentId, bool sampled);

    protected:
        std::shared_ptr<span_impl> m_span;
    };
}
#if defined(WIN32)
#pragma warning( pop )
#endif
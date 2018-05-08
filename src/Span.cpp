#include "Span.h"
#include "span_impl.h"
#include "TransportManager.h"

namespace cppkin
{


    Span::Span(const char* operationName, uint_fast64_t traceId, bool sampled)
    {
        m_span.reset(new span_impl(operationName, traceId, sampled));
    }

    Span::Span(const char* operationName, uint_fast64_t traceId, uint_fast64_t parentId, bool sampled)
    {
        uint_fast64_t id = span_impl::GenerateID();
        m_span.reset(new span_impl(operationName, traceId, parentId, id, sampled));
    }

    Span Span::CreateSpan(const char *operationName, const char* value)
    {
        const span_impl::SpanHeader& header = m_span->GetHeader();
        Span span( operationName, header.TraceID, header.ID, header.Sampled);
        span.AddAnnotation(value);
        return span;
    }

    void Span::AddAnnotation(const char* value)
    {
        if(m_span->GetHeader().Sampled == false)
            return;
        m_span->CreateSimpleAnnotation(value);
    }

    void Span::Submit(const char* value)
    {
        if(m_span->GetHeader().Sampled == false)
            return;
        m_span->SetEndTime();
        AddAnnotation(value);
        TransportManager::Instance().PushSpan(m_span);
    }
}

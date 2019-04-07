#include "Span.h"
#include "TransportManager.h"

namespace cppkin
{

    Span::Span() = default;

    Span::Span(const char* operationName, uint_fast64_t traceId, bool sampled)
    {
        m_span.reset(new span_impl(operationName, traceId, sampled));
    }

    Span::Span(const char* operationName, uint_fast64_t traceId, uint_fast64_t parentId, bool sampled)
    {
        uint_fast64_t id = span_impl::GenerateID();
        m_span.reset(new span_impl(operationName, traceId, parentId, id, sampled));
    }

    Span Span::CreateSpan(const char *operationName, const char* value) const
    {
        const span_impl::SpanHeader& header = m_span->GetHeader();
        Span span( operationName, header.TraceID, header.ID, header.Sampled);
        span.AddAnnotation(value, m_span->GetTimeStamp());
        return span;
    }

    void Span::Join(const char *operationName, uint_fast64_t traceId, uint_fast64_t parentId, uint_fast64_t id, bool sampled)
    {
        m_span.reset(new span_impl(operationName, traceId, parentId, id, sampled));
    }
    
    void Span::Join(const std::string& b3format)
    {
        m_span.reset(new span_impl(b3format));
    }

    void Span::AddAnnotation(const char* value)
    {
        if(m_span->GetHeader().Sampled == false)
            return;
        m_span->CreateSimpleAnnotation(value);
    }

    void Span::AddAnnotation(const char* value, int_fast64_t timeStamp)
    {
        if(m_span->GetHeader().Sampled == false)
            return;
        m_span->CreateSimpleAnnotation(value, timeStamp);
    }

    void Span::Submit(const char* value)
    {
        if(m_span->GetHeader().Sampled == false)
            return;
        m_span->SetEndTime();
        AddAnnotation(value, m_span->GetTimeStamp() + m_span->GetDuration());
        TransportManager::Instance().PushSpan(m_span);
    }

    bool Span::IsSampled() const
    {
        return m_span->GetHeader().Sampled;
    }
    
    std::string Span::GetHeaderB3Format() const
    {
        return m_span->GetHeaderB3Format();
    }

    const span_impl::SpanHeader& Span::GetHeader() const
    {
        return m_span->GetHeader();
    }
}

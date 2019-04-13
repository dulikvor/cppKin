#include "Span.h"
#include <functional>
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
    
    void Span::Join(const char* b3format)
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
    
    void Span::AddTag(const char* key, bool value)
    {
        if(m_span->GetHeader().Sampled == false)
            return;
        m_span->CreateBinaryAnnotation(key, value);
    }
    
    void Span::AddTag(const char* key, const char* value)
    {
        if(m_span->GetHeader().Sampled == false)
            return;
        m_span->CreateBinaryAnnotation(key, value);
    }

    void Span::Submit(const char* value)
    {
        if(m_span->GetHeader().Sampled == false)
            return;
        m_span->SetEndTime();
        if(strcmp(value, Annotation::Value::NOP) != 0)
            AddAnnotation(value, m_span->GetTimeStamp() + m_span->GetDuration());
        TransportManager::Instance().PushSpan(m_span);
    }

    bool Span::IsSampled() const
    {
        return m_span->GetHeader().Sampled;
    }
    
    void Span::GetHeaderB3Format(const char*& b3header) const
    {
        typedef std::unique_ptr<char, std::function<void(char*)>> char_ptr;
        std::string b3header_str = m_span->GetHeaderB3Format();
        char_ptr temp_header((char*)malloc(sizeof(char)*(b3header_str.size() + 1)), [](char* ptr){free(ptr);});
        std::memcpy(temp_header.get(), b3header_str.c_str(), b3header_str.size());
        temp_header.get()[b3header_str.size()] = '\0';
        b3header = temp_header.release();
    }

    const span_impl::SpanHeader& Span::GetHeader() const
    {
        return m_span->GetHeader();
    }
}

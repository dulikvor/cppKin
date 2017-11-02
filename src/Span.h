#pragma once

#include <string>
#include <cinttypes>
#include <vector>
#include <memory>
#include "Annotation.h"

namespace cppkin
{
    class Trace;
    class EncoderContextThrift;

    class Span
    {
    public:
        typedef std::vector<std::unique_ptr<Annotation>> Annotations;
        struct SpanHeader
        {
        public:
            SpanHeader(const std::string& name, int_fast64_t traceID, int_fast64_t parentID, int_fast64_t id);
            SpanHeader(){}
        public:
            std::string Name;
            int_fast64_t ID;
            int_fast64_t ParentID;
            int_fast64_t TraceID;
        };

    public:
        ~Span(){}
        const SpanHeader& GetHeader() const{
            return m_header;
        }
        const Annotations& GetAnnotations() const{
            return m_events;
        }
        void CreateSimpleAnnotation(const Annotation::EndPoint& endPoint, const std::string& event);
        template<typename T>
        void CreateBinaryAnnotation(const std::string& key, const T& value);
        int_fast64_t GetTimeStamp() const{
            return m_timeStamp;
        }
        int_fast64_t GetDuration() const{
            return m_duration;
        }
        void SetEndTime(){
            m_duration = GetCurrentTime() - m_timeStamp;
        }

    private:
        friend class Trace;
        friend class EncoderContextThrift;
        Span(const std::string& name, int_fast64_t traceID, int_fast64_t parentID, int_fast64_t id);
        int_fast64_t GetCurrentTime();

    private:
        SpanHeader m_header;
        Annotations m_events;
        int_fast64_t m_timeStamp;
        int_fast64_t m_duration;
    };
}

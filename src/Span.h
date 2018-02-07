#pragma once

#include <string>
#include <cinttypes>
#include <vector>
#include <memory>
#include "Core/src/Export.h"
#include "Annotation.h"

namespace cppkin
{
    class Trace;
    class EncoderContextThrift;

    class A_EXPORT Span
    {
    public:
        typedef std::vector<std::unique_ptr<Annotation>> Annotations;
        struct A_EXPORT SpanHeader
        {
        public:
            SpanHeader(const std::string& name, int_fast64_t traceID, int_fast64_t parentID, int_fast64_t id);
            SpanHeader(const std::string& name, int_fast64_t traceID, int_fast64_t id);
            SpanHeader(){}
        public:
            std::string Name;
            uint_fast64_t ID;
            uint_fast64_t ParentID;
            uint_fast64_t TraceID;
            bool ParentIDSet;
        };

    public:
        ~Span(){}
        Span(const Span&) = delete;
        Span& operator=(const Span&) = delete;
        const SpanHeader& GetHeader() const;
        const Annotations& GetAnnotations() const;
        void CreateSimpleAnnotation(const std::string& event);
        template<typename T>
		inline void CreateBinaryAnnotation(const std::string& key, const T& value){}
		int_fast64_t GetTimeStamp() const;
		int_fast64_t GetDuration() const;
		void SetEndTime();
        bool IsRootSpan() const { return !m_header.ParentIDSet; }

    private:
        friend class Trace;

        Span(const std::string& name, int_fast64_t traceID, int_fast64_t parentID, int_fast64_t id);
        Span(const std::string& name, int_fast64_t traceID);
        int_fast64_t GetCurrentTime();

    private:
        SpanHeader m_header;
        Annotations m_events;
        int_fast64_t m_timeStamp;
        int_fast64_t m_duration;
    };
}

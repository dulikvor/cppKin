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

    class span_impl
    {
    public:
        typedef std::vector<std::unique_ptr<Annotation>> Annotations;
        struct SpanHeader
        {
        public:
            SpanHeader(const std::string& name, uint_fast64_t traceID, uint_fast64_t parentID, uint_fast64_t id, bool sampled);
            SpanHeader(const std::string& name, uint_fast64_t traceID, uint_fast64_t id, bool sampled);
            SpanHeader(){}
        public:
            std::string Name;
            uint_fast64_t ID;
            uint_fast64_t ParentID;
            uint_fast64_t TraceID;
            bool Sampled;
            bool ParentIdSet;
        };

    public:
        ~span_impl(){}
        span_impl(const span_impl& obj);
        span_impl& operator=(const span_impl&) = delete;
        const SpanHeader& GetHeader() const;
        const Annotations& GetAnnotations() const;
        void CreateSimpleAnnotation(const std::string& event);
        void CreateSimpleAnnotation(const std::string& event, int_fast64_t timeStamp);
        template<typename T>
		inline void CreateBinaryAnnotation(const std::string& key, const T& value){}
		int_fast64_t GetTimeStamp() const;
		int_fast64_t GetDuration() const;
		void SetEndTime();

    private:
        friend class Trace;
        friend class Span;

        span_impl(const std::string& name, uint_fast64_t traceID, uint_fast64_t parentID, uint_fast64_t id, bool sampled);
        span_impl(const std::string& name, uint_fast64_t traceID, bool sampled);
        static uint_fast64_t GenerateID();
        int_fast64_t GetCurrentTime();

    private:
        SpanHeader m_header;
        Annotations m_events;
        int_fast64_t m_timeStamp;
        int_fast64_t m_duration;
    };
}

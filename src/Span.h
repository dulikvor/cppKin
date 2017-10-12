#pragma once

#include <string>
#include <cinttypes>
#include <vector>
#include <memory>
#include "Annotation.h"

namespace cppkin
{
    class Trace;

    class Span
    {
    public:
        struct SpanHeader
        {
        public:
            SpanHeader(const std::string& name, uint_fast64_t traceID, uint_fast64_t parentID, uint_fast64_t id);
        public:
            std::string Name;
            uint_fast64_t ID;
            uint_fast64_t ParentID;
            uint_fast64_t TraceID;
        };

    public:
        void CreateSimpleAnnotation(const std::string& event);
        template<typename T>
        void CreateBinaryAnnotation(const std::string& key, const T& value);

    private:
        friend class Trace;
        Span(const std::string& name, uint_fast64_t traceID, uint_fast64_t parentID, uint_fast64_t id);

    private:
        SpanHeader m_header;
        std::vector<std::unique_ptr<Annotation>> m_events;
    };
}

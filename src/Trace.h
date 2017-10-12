#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include "SpanContainer.h"


namespace cppkin
{
    class Span;

    class Trace
    {
    public:
        static Trace& Instance();
        std::shared_ptr<Span> CreateSpan(const char* operationName, uint_fast64_t traceID, uint_fast64_t parentID);
    private:
        Trace(){}
        static uint_fast64_t GenerateTraceID();
        static uint_fast64_t GenerateSpanID();

    private:
        std::unique_ptr<SpanContainer> m_spanContainer;
    };
}

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
        std::unique_ptr<Span> CreateSpan(const char* operationName, uint_fast64_t traceID, uint_fast64_t parentID);
        static uint_fast64_t GenerateTraceID();
        static uint_fast64_t GenerateSpanID();
    private:
        Trace(){}
    };
}

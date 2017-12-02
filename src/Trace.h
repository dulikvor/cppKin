#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include "Core\src\Export.h"
#include "SpanContainer.h"


namespace cppkin
{
    class Span;

	class A_EXPORT Trace
    {
    public:
        static Trace& Instance();
        std::unique_ptr<Span> CreateSpan(const char* operationName, uint_fast64_t traceID, uint_fast64_t parentID, uint_fast64_t id);
        std::unique_ptr<Span> CreateSpan(const char* operationName, uint_fast64_t traceID);
        static uint_fast64_t GenerateTraceID();
        static uint_fast64_t GenerateSpanID();
    private:
        Trace(){}
    };
}

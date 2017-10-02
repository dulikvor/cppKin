#pragma once

#include <cstdint>
#include <memory>
#include <string>

class Span;
class SpanContainer;

class Trace
{
public:
    static Trace& Instance();
    std::shared_ptr<Span> CreateSpan(const char* operationName, u_int64_t parentID);
private:
    Trace(){}
    u_int64_t GenerateTraceID(){};
    u_int64_t GenerateSpanID(){};

private:
    std::unique_ptr<SpanContainer> m_spanContainer;
};


#include "Span.h"
#include <chrono>
#include "SimpleAnnotation.h"

using namespace std;

namespace cppkin
{
    Span::SpanHeader::SpanHeader(const std::string& name, int_fast64_t traceID, int_fast64_t parentID, int_fast64_t id) :
            Name(name), ID(id), ParentID(parentID), TraceID(traceID){}

    Span::Span(const std::string &name, int_fast64_t traceID, int_fast64_t parentID, int_fast64_t id) :
            m_header(name, traceID, parentID, id), m_timeStamp(GetCurrentTime()) {}

    void Span::CreateSimpleAnnotation(const Annotation::EndPoint& endPoint, const std::string &event) {
        m_events.emplace_back(new SimpleAnnotation(endPoint, event, GetCurrentTime()));
    }

    int_fast64_t Span::GetCurrentTime() {
        auto currentTime = chrono::system_clock::now();
        auto currentTimeMs = chrono::time_point_cast<chrono::milliseconds>(currentTime);
        auto value = chrono::duration_cast<std::chrono::milliseconds>(currentTime.time_since_epoch());
        return value.count();
    }
}

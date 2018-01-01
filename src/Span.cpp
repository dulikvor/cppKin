#include "Span.h"
#include <chrono>
#include "Core/src/Assert.h"
#include "Core/src/Environment.h"
#include "SimpleAnnotation.h"
#include "ConfigTags.h"
#include "ConfigParams.h"

using namespace std;

namespace cppkin
{
    Span::SpanHeader::SpanHeader(const std::string& name, int_fast64_t traceID, int_fast64_t parentID, int_fast64_t id) :
            Name(name), ID(id), ParentID(parentID), TraceID(traceID), ParentIDSet(true){}

    Span::SpanHeader::SpanHeader(const std::string& name, int_fast64_t traceID, int_fast64_t id) :
            Name(name), ID(id), TraceID(traceID), ParentIDSet(false){}

    Span::Span(const std::string &name, int_fast64_t traceID, int_fast64_t parentID, int_fast64_t id) :
            m_header(name, traceID, parentID, id), m_timeStamp(GetCurrentTime()) {}

    Span::Span(const std::string &name, int_fast64_t traceID) :
            m_header(name, traceID, traceID), m_timeStamp(GetCurrentTime()) {}

    const Span::SpanHeader& Span::GetHeader() const{
        return m_header;
    }

    const Span::Annotations& Span::GetAnnotations() const{
        return m_events;
    }

    void Span::CreateSimpleAnnotation(const std::string &event) {
        VERIFY(core::Environment::Instance().GetIPV4Addresses().size() > 0, "Missing IPV4 address");
        static Annotation::EndPoint endPoint(ConfigParams::Instance().GetServiceName(),
                                      core::Environment::Instance().GetIPV4Addresses().back(),
                                      ConfigParams::Instance().GetPort());
        m_events.emplace_back(new SimpleAnnotation(endPoint, event, GetCurrentTime()));
    }

    int_fast64_t Span::GetTimeStamp() const{
        return m_timeStamp;
    }

    int_fast64_t Span::GetDuration() const{
        return m_duration;
    }

    void Span::SetEndTime(){
        m_duration = GetCurrentTime() - m_timeStamp;
    }

    int_fast64_t Span::GetCurrentTime() {
        auto currentTime = chrono::system_clock::now();
        auto value = chrono::duration_cast<std::chrono::microseconds>(currentTime.time_since_epoch());
        return value.count();
    }
}

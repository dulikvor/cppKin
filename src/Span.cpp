#include "Span.h"
#include <chrono>
#include "Core/src/Assert.h"
#include "Core/src/Enviorment.h"
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

    void Span::CreateSimpleAnnotation(const std::string &event) {
        VERIFY(core::Enviorment::Instance().GetIPV4Addresses().size() > 0, "Missing IPV4 address");
        static Annotation::EndPoint endPoint(ConfigParams::Instance().GetServiceName(),
                                      core::Enviorment::Instance().GetIPV4Addresses().back(),
                                      ConfigParams::Instance().GetPort());
        m_events.emplace_back(new SimpleAnnotation(endPoint, event, GetCurrentTime()));
    }

    int_fast64_t Span::GetCurrentTime() {
        auto currentTime = chrono::system_clock::now();
        auto value = chrono::duration_cast<std::chrono::microseconds>(currentTime.time_since_epoch());
        return value.count();
    }
}

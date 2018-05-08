#include "span_impl.h"
#include <chrono>
#include <random>
#include "Core/src/Assert.h"
#include "Core/src/Environment.h"
#include "SimpleAnnotation.h"
#include "ConfigTags.h"
#include "ConfigParams.h"

using namespace std;

namespace cppkin
{
    span_impl::SpanHeader::SpanHeader(const std::string& name, uint_fast64_t traceID, uint_fast64_t parentID, uint_fast64_t id, bool sampled) :
            Name(name), ID(id), ParentID(parentID), TraceID(traceID), Sampled(sampled), ParentIdSet(true){}

    span_impl::SpanHeader::SpanHeader(const std::string& name, uint_fast64_t traceID, uint_fast64_t id, bool sampled) :
            Name(name), ID(id), TraceID(traceID), Sampled(sampled), ParentIdSet(false){}

    span_impl::span_impl(const std::string &name, uint_fast64_t traceID, uint_fast64_t parentID, uint_fast64_t id, bool sampled) :
            m_header(name, traceID, parentID, id, sampled), m_timeStamp(GetCurrentTime()) {}

    span_impl::span_impl(const std::string &name, uint_fast64_t traceID, bool sampled) :
            m_header(name, traceID, traceID, sampled), m_timeStamp(GetCurrentTime()) {}

    const span_impl::SpanHeader& span_impl::GetHeader() const{
        return m_header;
    }

    const span_impl::Annotations& span_impl::GetAnnotations() const{
        return m_events;
    }

    void span_impl::CreateSimpleAnnotation(const std::string &event) {
        VERIFY(core::Environment::Instance().GetIPV4Addresses().size() > 0, "Missing IPV4 address");
        static Annotation::EndPoint endPoint(ConfigParams::Instance().GetServiceName(),
                                      core::Environment::Instance().GetIPV4Addresses().back(),
                                      ConfigParams::Instance().GetPort());
        m_events.emplace_back(new SimpleAnnotation(endPoint, event, GetCurrentTime()));
    }

    int_fast64_t span_impl::GetTimeStamp() const{
        return m_timeStamp;
    }

    int_fast64_t span_impl::GetDuration() const{
        return m_duration;
    }

    void span_impl::SetEndTime(){
        m_duration = GetCurrentTime() - m_timeStamp;
    }

    int_fast64_t span_impl::GetCurrentTime() {
        auto currentTime = chrono::system_clock::now();
        auto value = chrono::duration_cast<std::chrono::microseconds>(currentTime.time_since_epoch());
        return value.count();
    }

    uint_fast64_t span_impl::GenerateID(){
        static mt19937_64 twister;
        static random_device device;
        uint64_t seed = (static_cast<uint64_t>(device()) << 32) | device();
        twister.seed(seed);
        return twister();
    }
}

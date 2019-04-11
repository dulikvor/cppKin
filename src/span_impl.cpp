#include "span_impl.h"
#include <chrono>
#include <random>
#include <iomanip>
#include <sstream>
#include "SimpleAnnotation.h"

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
    
    span_impl::span_impl(const char* b3format)
        : m_timeStamp(GetCurrentTime())
    {
        std::istringstream is(b3format);
        std::string dummy_s;
        char dummy_c, debug_or_sampled;
        is >> std::hex >> dummy_s >> m_header.TraceID >>
            dummy_c >> m_header.ID>> dummy_c >> debug_or_sampled;
        if(is.peek() == '-')
        {
            is >> dummy_c >> m_header.ParentID;
            m_header.ParentIdSet = true;
        }
        if(debug_or_sampled == 'd' || debug_or_sampled == '1')
            m_header.Sampled = true;
        else
            m_header.Sampled = false;
    }

    span_impl::span_impl(const span_impl& obj)
    {
        for(const auto& event : obj.m_events)
        {
            if( event->GetType() == AnnotationType::Simple) {
                auto& simpleAnnotation = static_cast<SimpleAnnotation &>(*event);
                m_events.emplace_back(new SimpleAnnotation(simpleAnnotation));
            }
            else
            {
                auto &binaryAnnotation = static_cast<BinaryAnnotation &>(*event);
                m_events.emplace_back(new BinaryAnnotation(binaryAnnotation));
            }
            
        }
        m_timeStamp = obj.m_timeStamp;
        m_duration = obj.m_duration;
        m_header = obj.m_header;
    }

    const span_impl::SpanHeader& span_impl::GetHeader() const{
        return m_header;
    }

    const span_impl::Annotations& span_impl::GetAnnotations() const{
        return m_events;
    }

    void span_impl::CreateSimpleAnnotation(const std::string &event) {
        VERIFY(!core::Environment::Instance().GetIPV4Addresses().empty(), "Missing IPV4 address");
        static Annotation::EndPoint endPoint(ConfigParams::Instance().GetServiceName(),
                                      core::Environment::Instance().GetIPV4Addresses().back(),
                                      ConfigParams::Instance().GetPort());
        m_events.emplace_back(new SimpleAnnotation(endPoint, event, GetCurrentTime()));
    }

    void span_impl::CreateSimpleAnnotation(const std::string &event, int_fast64_t timeStamp) {
        VERIFY(!core::Environment::Instance().GetIPV4Addresses().empty(), "Missing IPV4 address");
        static Annotation::EndPoint endPoint(ConfigParams::Instance().GetServiceName(),
                                             core::Environment::Instance().GetIPV4Addresses().back(),
                                             ConfigParams::Instance().GetPort());
        m_events.emplace_back(new SimpleAnnotation(endPoint, event, timeStamp));
    }
    
    void span_impl::CreateBinaryAnnotation(const char *key, bool value)
    {
        VERIFY(!core::Environment::Instance().GetIPV4Addresses().empty(), "Missing IPV4 address");
        static Annotation::EndPoint endPoint(ConfigParams::Instance().GetServiceName(),
                                             core::Environment::Instance().GetIPV4Addresses().back(),
                                             ConfigParams::Instance().GetPort());
        m_events.emplace_back(new BinaryAnnotation(endPoint, key, value));
    }
    
    void span_impl::CreateBinaryAnnotation(const char *key, const char* value)
    {
        VERIFY(!core::Environment::Instance().GetIPV4Addresses().empty(), "Missing IPV4 address");
        static Annotation::EndPoint endPoint(ConfigParams::Instance().GetServiceName(),
                                             core::Environment::Instance().GetIPV4Addresses().back(),
                                             ConfigParams::Instance().GetPort());
        m_events.emplace_back(new BinaryAnnotation(endPoint, key, value));
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
        auto currentTime = std::chrono::system_clock::now();
        auto value = std::chrono::duration_cast<std::chrono::microseconds>(currentTime.time_since_epoch());
        return value.count();
    }

    uint_fast64_t span_impl::GenerateID(){
        static std::mt19937_64 twister;
        static std::random_device device;
        uint64_t seed = (static_cast<uint64_t>(device()) << 32) | device();
        twister.seed((unsigned long)seed);
        return twister();
    }
    
    std::string span_impl::GetHeaderB3Format() const
    {
        bool debug = ConfigParams::Instance().GetDebug();
        std::ostringstream os;
        {
            std::ostream temp_os(os.rdbuf());
            temp_os << std::hex << "b3: " << m_header.TraceID << '-' << m_header.ID << "-";
        }
        if(debug)
            os << 'd';
        else
            os << (m_header.Sampled ? 1 : 0);
        
        os << '-' << std::hex << m_header.ParentID;
        return os.str();
    }
}

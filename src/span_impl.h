#pragma once

#include <string>
#include <cinttypes>
#include <vector>
#include <memory>
#include "core/Assert.h"
#include "core/Environment.h"
#include "Annotation.h"
#include "BinaryAnnotation.h"
#include "ConfigParams.h"
#if defined(WIN32)
#pragma warning( push )
#pragma warning (disable : 4251)
#endif

namespace cppkin
{
    class Trace;
    class EncoderContextThrift;

    class CPPKIN_EXPORT span_impl
    {
    public:
        typedef std::vector<std::unique_ptr<Annotation>> Annotations;
        struct CPPKIN_EXPORT SpanHeader
        {
        public:
            SpanHeader(const std::string& name, uint_fast64_t traceID, uint_fast64_t parentID, uint_fast64_t id, bool sampled);
            SpanHeader(const std::string& name, uint_fast64_t traceID, uint_fast64_t id, bool sampled);
            SpanHeader() = default;
        public:
            std::string Name;
            uint_fast64_t ID;
            uint_fast64_t ParentID;
            uint_fast64_t TraceID;
            bool Sampled;
            bool ParentIdSet;
        };

    public:
        ~span_impl() = default;
        span_impl(const span_impl& obj);
        span_impl& operator=(const span_impl&) = delete;
        const SpanHeader& GetHeader() const;
        const Annotations& GetAnnotations() const;
        void CreateSimpleAnnotation(const std::string& event);
        void CreateSimpleAnnotation(const std::string& event, int_fast64_t timeStamp);
        template<typename T>
        typename std::enable_if<!std::is_array<typename std::remove_reference<T>::type>::value>::type
        CreateBinaryAnnotation(const char* key, T&& value)
        {
            VERIFY(!core::Environment::Instance().GetIPV4Addresses().empty(), "Missing IPV4 address");
            static Annotation::EndPoint endPoint(ConfigParams::Instance().GetServiceName(),
                                                 core::Environment::Instance().GetIPV4Addresses().back(),
                                                 ConfigParams::Instance().GetPort());
            m_events.emplace_back(new BinaryAnnotation(endPoint, key, std::forward<T>(value)));
        }
        template<std::size_t N>
        void CreateBinaryAnnotation(const char* key, char const(&value)[N])
        {
            VERIFY(!core::Environment::Instance().GetIPV4Addresses().empty(), "Missing IPV4 address");
            static Annotation::EndPoint endPoint(ConfigParams::Instance().GetServiceName(),
                                                 core::Environment::Instance().GetIPV4Addresses().back(),
                                                 ConfigParams::Instance().GetPort());
            auto chr_value = static_cast<const char*>(value);
            m_events.emplace_back(new BinaryAnnotation(endPoint, key, chr_value));
        }
		int_fast64_t GetTimeStamp() const;
		int_fast64_t GetDuration() const;
		void SetEndTime();

    private:
        friend class Trace;
        friend class Span;

        span_impl(const std::string& name, uint_fast64_t traceID, uint_fast64_t parentID, uint_fast64_t id, bool sampled);
        span_impl(const std::string& name, uint_fast64_t traceID, bool sampled);
        explicit span_impl(const std::string& b3format);
        static uint_fast64_t GenerateID();
        int_fast64_t GetCurrentTime();
        std::string GetHeaderB3Format() const;

    private:
        SpanHeader m_header;
        Annotations m_events;
        int_fast64_t m_timeStamp;
        int_fast64_t m_duration;
    };
}
#if defined(WIN32)
#pragma warning( pop )
#endif
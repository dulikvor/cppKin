#pragma once

#include <string>
#include <cinttypes>
#include "Annotation.h"

namespace cppkin
{
    class SimpleAnnotation : public Annotation
    {
    public:
        SimpleAnnotation(const EndPoint& endPoint, const std::string& event, int_fast64_t timeStamp);
        ~SimpleAnnotation() override = default;
        const std::string& GetEvent() const{
            return m_event;
        }
        int_fast64_t GetTimeStamp() const{
            return m_timeStamp;
        }
    private:
        std::string m_event;
        int_fast64_t m_timeStamp;
    };
}

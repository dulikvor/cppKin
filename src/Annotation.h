#pragma once

#include <string>
#include <stdint.h>
#include "AnnotationType.h"
#include "Export.h"

namespace cppkin
{
    class CPPKIN_EXPORT Annotation
    {
    public:
        struct CPPKIN_EXPORT EndPoint
        {
        public:
            EndPoint(const std::string& serviceName, std::string host, uint_fast16_t port);
        public:
            std::string ServiceName;
            std::string Host;
            int_fast16_t Port;
            int32_t BinaryHost;
        };

        struct CPPKIN_EXPORT Value
        {
        public:
            static const char* SERVER_SEND;
            static const char* SERVER_RECEIVE;
            static const char* CLIENT_SEND;
            static const char* CLIENT_RECEIVE;
            static const char* NOP;
        };

    public:
        Annotation(AnnotationType type, const EndPoint& endPoint): m_type(type),
            m_endPoint(endPoint){}
        virtual ~Annotation() = default;
        AnnotationType GetType() const{ return m_type; }
        const EndPoint& GetEndPoint() const {return m_endPoint;}

    protected:
        AnnotationType m_type;
        EndPoint m_endPoint;
    };
}


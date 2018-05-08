#pragma once

#include <string>
#include <stdint.h>
#include "AnnotationType.h"

namespace cppkin
{
    class Annotation
    {
    public:
        struct EndPoint
        {
        public:
            EndPoint(const std::string& serviceName, std::string host, uint_fast16_t port);
        public:
            int32_t Host;
            int_fast16_t Port;
            std::string ServiceName;
        };

        struct Value
        {
        public:
            static const char* SERVER_SEND;
            static const char* SERVER_RECEIVE;
            static const char* CLIENT_SEND;
            static const char* CLIENT_RECEIVE;
        };

    public:
        Annotation(AnnotationType type, const EndPoint& endPoint): m_type(type),
            m_endPoint(endPoint){}
        virtual ~Annotation(){}
        AnnotationType GetType() const{ return m_type; }
        const EndPoint& GetEndPoint() const {return m_endPoint;}

    private:
        AnnotationType m_type;
        EndPoint m_endPoint;
    };
}


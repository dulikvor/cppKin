#pragma once

#include <string>
#include "AnnotationType.h"

namespace cppkin
{
    class Annotation
    {
    public:
        struct EndPoint
        {
        public:
            EndPoint(const std::string& host, uint_fast16_t port): Host(host), Port(port){}
        public:
            std::string Host;
            uint_fast16_t Port;
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


#pragma once

#include "AnnotationType.h"

namespace cppkin
{
    class Annotation
    {
    public:
        Annotation(AnnotationType type): m_type(type){}
        virtual ~Annotation(){}
        AnnotationType GetType(){ return m_type; }

    private:
        AnnotationType m_type;
    };
}


#pragma once

#include <string>
#include "AnnotationType.h"
#include "Annotation.h"

namespace cppkin
{
    class SimpleAnnotation : public Annotation
    {
    public:
        SimpleAnnotation(const std::string& event);

    private:
        std::string m_event;
    };
}

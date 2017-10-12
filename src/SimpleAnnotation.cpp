#include "SimpleAnnotation.h"

using namespace std;

namespace cppkin
{
    SimpleAnnotation::SimpleAnnotation(const string &event):
            Annotation(AnnotationType::Simple), m_event(event) {}
}

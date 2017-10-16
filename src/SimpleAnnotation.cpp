#include "SimpleAnnotation.h"

using namespace std;

namespace cppkin
{
    SimpleAnnotation::SimpleAnnotation(const string &event, int_fast64_t timeStamp):
            Annotation(AnnotationType::Simple), m_event(event), m_timeStamp(timeStamp) {}
}

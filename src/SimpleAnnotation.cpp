#include "SimpleAnnotation.h"
#include "AnnotationType.h"

using namespace std;

namespace cppkin
{
    SimpleAnnotation::SimpleAnnotation(const EndPoint& endPoint, const string &event, int_fast64_t timeStamp):
            Annotation(AnnotationType::Simple, endPoint), m_event(event), m_timeStamp(timeStamp) {}
}

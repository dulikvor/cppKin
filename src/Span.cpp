#include "Span.h"
#include "SimpleAnnotation.h"

using namespace std;

namespace cppkin
{
    Span::SpanHeader::SpanHeader(const std::string& name, uint_fast64_t traceID, uint_fast64_t parentID, uint_fast64_t id) :
            Name(name), ID(id), ParentID(parentID), TraceID(traceID){}

    Span::Span(const std::string &name, uint_fast64_t traceID, uint_fast64_t parentID, uint_fast64_t id) :
            m_header(name, traceID, parentID, id) {}

    void Span::CreateSimpleAnnotation(const std::string &event) {
        m_events.emplace_back(new SimpleAnnotation(event));
    }
}

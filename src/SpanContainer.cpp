#include "SpanContainer.h"

using namespace std;

namespace cppkin
{

    thread_local std::unique_ptr<Span> SpanContainer::m_span;
    void SpanContainer::SetSpan(unique_ptr<Span> span) {
        swap(m_span, span);
    }
}

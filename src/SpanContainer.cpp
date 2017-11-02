#include "SpanContainer.h"
#include "Core/src/Assert.h"

using namespace std;
using namespace core;

namespace cppkin
{

    thread_local std::unique_ptr<Span> SpanContainer::m_span;
    void SpanContainer::SetSpan(unique_ptr<Span> span) {
        swap(m_span, span);
    }

    std::unique_ptr<Span> SpanContainer::ReleaseSpan(){
        unique_ptr<Span> returnSpan(nullptr);
        VERIFY(m_span.get() != nullptr, "Storage contains no span");
        swap(m_span, returnSpan);
        return returnSpan;
    }
}

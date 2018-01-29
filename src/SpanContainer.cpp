#include "SpanContainer.h"
#include "Core/src/Assert.h"

using namespace std;
using namespace core;

namespace cppkin
{
#if defined(WIN32)
    static __declspec(thread) Span* m_span = nullptr; //Not unique ptr due to limitation on static initialization for TLS object.
#else

#  include <features.h>
#  if __GNUC_PREREQ(4,8)
    static thread_local Span* m_span = nullptr;
    static thread_local std::unique_ptr<Span::SpanHeader> m_rootHeader = nullptr;
#else
    static __thread Span* m_span = nullptr;
    static __thread std::unique_ptr<Span::SpanHeader> m_rootHeader = nullptr;
#endif
#endif

    SpanContainer& SpanContainer::Instance(){
        static SpanContainer instance;
        return instance;
    }

    SpanContainer::~SpanContainer(){
        delete m_span;
    }

    Span* SpanContainer::GetSpan() const{
        return m_span;
    }

    SpanContainer::SpanContainer(){}

    void SpanContainer::SetSpan(Span* span) {
        swap(m_span, span);
        if(m_span->IsRootSpan())
            m_rootHeader.reset( new Span::SpanHeader(m_span->GetHeader()));
        delete span;
    }

    unique_ptr<Span> SpanContainer::ReleaseSpan(){
        Span* returnSpan = nullptr;
        VERIFY(m_span != nullptr, "Storage contains no span");
        swap(m_span, returnSpan);
        return unique_ptr<Span>(returnSpan);
    }
}

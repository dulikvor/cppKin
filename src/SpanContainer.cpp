#include <forward_list>
#include "SpanContainer.h"
#include "Core/src/Assert.h"

using namespace std;
using namespace core;

namespace cppkin
{
#if defined(WIN32)
    static __declspec(thread) std::forward_list<Span*>* m_spans = nullptr; //Not unique ptr due to limitation on static initialization for TLS object.
#else

#  include <features.h>
#  if __GNUC_PREREQ(4,8)
    static thread_local std::forward_list<Span*>* m_spans = nullptr;
    static thread_local std::unique_ptr<Span::SpanHeader> m_rootHeader = nullptr;
#else
    static __thread std::forward_list<Span*>* m_spans = nullptr;
    static __thread std::unique_ptr<Span::SpanHeader> m_rootHeader = nullptr;
#endif
#endif

    SpanContainer& SpanContainer::Instance(){
        static SpanContainer instance;
        return instance;
    }

    SpanContainer::~SpanContainer(){
        delete m_spans;
    }

    const Span* SpanContainer::TopSpan() const{
        if (m_spans->empty()){
            return nullptr;
        }
        return m_spans->front();
    }

    const Span::SpanHeader* SpanContainer::GetRootHeader() const{
        return m_rootHeader.get();
    }

    SpanContainer::SpanContainer(){
        m_spans = new std::forward_list<Span*>();
    }

    void SpanContainer::PushSpan(Span* span) {
        //swap(m_span, span);
        m_spans->push_front(span);
        if(span->IsRootSpan())
            m_rootHeader.reset( new Span::SpanHeader(span->GetHeader()));
        //delete span;
    }

   Span* SpanContainer::PopSpan(){
        Span* returnSpan = nullptr;
        if (!m_spans->empty()) {
            returnSpan = m_spans->front();
            m_spans->pop_front();
        }
       // VERIFY(m_span != nullptr, "Storage contains no span");
        //swap(m_span, returnSpan);
        return returnSpan;
    }
}

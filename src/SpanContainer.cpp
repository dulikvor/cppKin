#include <forward_list>
#include "Core/src/Assert.h"
#include "SpanContainer.h"

using namespace std;
using namespace core;

namespace cppkin
{
    using SpansList = std::forward_list<Span*>;

#if defined(WIN32)
    static __declspec(thread) SpansList* m_spans = nullptr; //Not unique ptr due to limitation on static initialization for TLS object.
#else

#  include <features.h>
#  if __GNUC_PREREQ(4,8)
    static thread_local SpansList* m_spans = nullptr;
    static thread_local std::unique_ptr<Span::SpanHeader> m_rootHeader = nullptr;
#else
    static __thread SpansList* m_spans = nullptr;
    static __thread std::unique_ptr<Span::SpanHeader> m_rootHeader = nullptr;
#endif
#endif

    static SpansList* GetSpans() {
        if (!m_spans){
            m_spans = new std::forward_list<Span*>();
        }
        return m_spans;
    }

    SpanContainer& SpanContainer::Instance(){
        static SpanContainer instance;
        return instance;
    }

    SpanContainer::SpanContainer(){}
    SpanContainer::~SpanContainer(){}

    const Span* SpanContainer::TopSpan() const{
        if (GetSpans()->empty()){
            return nullptr;
        }
        return GetSpans()->front();
    }

    const Span::SpanHeader* SpanContainer::GetRootHeader() const{
        return m_rootHeader.get();
    }

    void SpanContainer::PushSpan(Span* span) {
        GetSpans()->push_front(span);
        if(span->IsRootSpan())
            m_rootHeader.reset( new Span::SpanHeader(span->GetHeader()));
    }

   Span* SpanContainer::PopSpan(){
        Span* returnSpan = nullptr;
        if (!GetSpans()->empty()) {
            returnSpan = GetSpans()->front();
            GetSpans()->pop_front();
        }
        return returnSpan;
    }
}

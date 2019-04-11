#include "core/Exception.h"
#include "SpanContainer.h"

using namespace std;
using namespace core;

namespace cppkin
{


#if defined(WIN32)
    static __declspec(thread) SpanContainer* m_container = nullptr;
#else

#  include <features.h>
#  if __GNUC_PREREQ(4,8)
    static thread_local SpanContainer* m_container = nullptr;
#else
    static __thread SpanContainer* m_container = nullptr;
#endif
#endif

    SpanContainer& SpanContainer::Instance(){
        if(!m_container){
            m_container = new SpanContainer();
        }
        return *m_container;
    }

    SpanContainer::SpanContainer() {}
    SpanContainer::~SpanContainer(){}

    void SpanContainer::PushSpan(Span& span) {
        m_spans.push_front(&span);
    }
    
    Span& SpanContainer::TopSpan(){
        if (m_spans.empty())
            throw Exception(__CORE_SOURCE, "Span container is empty");
        
        return *m_spans.front();
    }

    void SpanContainer::PopSpan(){
        if (m_spans.empty())
            return;

        m_spans.pop_front();
    }
    
    bool SpanContainer::IsEmpty()
    {
        return m_spans.empty();
    }
}

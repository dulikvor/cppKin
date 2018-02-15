#include "Core/src/Assert.h"
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

    SpanContainer::SpanContainer() { Reset(); }
    SpanContainer::~SpanContainer(){}

    const Span* SpanContainer::TopSpan() const{

        if (!m_traceInfo.get()) {
            return nullptr;
        }

        if (m_traceInfo->m_spans.empty()) {
            return nullptr;
        }

        return m_traceInfo->m_spans.front().get();
    }

    const Span::SpanHeader* SpanContainer::GetRootHeader() const {
        if(m_traceInfo.get() && m_traceInfo->m_rootHeader.get())
            return m_traceInfo->m_rootHeader.get();
        return nullptr;
    }

    void SpanContainer::PushSpan(std::unique_ptr<Span>&& span) {
        if(span->IsRootSpan()) {
            Reset();
            m_traceInfo->m_rootHeader.reset(new Span::SpanHeader(span->GetHeader()));
        }

        m_traceInfo->m_spans.emplace_front(std::move(span));
    }

    std::unique_ptr<Span> SpanContainer::PopSpan(){
        if (!m_traceInfo.get() || ( m_traceInfo.get() && m_traceInfo->m_spans.empty()) ) {
            return std::unique_ptr<Span>();
        }

        std::unique_ptr<Span> returnSpan(std::move(m_traceInfo->m_spans.front()));
        m_traceInfo->m_spans.pop_front();
        return returnSpan;
    }
}

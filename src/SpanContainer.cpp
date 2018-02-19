#include "Core/src/Assert.h"
#include "SpanContainer.h"
#include "Sampler.h"

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

    const Span& SpanContainer::TopSpan() const{
        if(m_traceInfo.get() == nullptr || m_traceInfo->Spans.empty() == true)
            throw Exception(__CORE_SOURCE, "Container is empty");

        return *m_traceInfo->Spans.front();
    }

    Span& SpanContainer::TopSpan(){
        if(m_traceInfo.get() == nullptr || m_traceInfo->Spans.empty() == true)
            throw Exception(__CORE_SOURCE, "Container is empty");

        return *m_traceInfo->Spans.front();
    }

    const Span::SpanHeader& SpanContainer::GetRootHeader() const {
        if(m_traceInfo.get() == nullptr || m_traceInfo->RootHeader.get() == nullptr)
            throw Exception(__CORE_SOURCE, "Container is missing root's header");

        return *m_traceInfo->RootHeader;
    }

    bool SpanContainer::ShouldSample() const {
        return m_traceInfo->Sampled;
    }

    void SpanContainer::PushSpan(std::unique_ptr<Span>&& span) {
        if(span->IsRootSpan()) {
            Reset();
            m_traceInfo->RootHeader.reset(new Span::SpanHeader(span->GetHeader()));
            m_traceInfo->Sampled = Sampler::ShouldSample();
        }
        m_traceInfo->Spans.emplace_front(std::move(span));
    }

    std::unique_ptr<Span> SpanContainer::PopSpan(){
        if (m_traceInfo.get() == nullptr || m_traceInfo->Spans.empty() == true )
            throw Exception(__CORE_SOURCE, "Container is empty");

        std::unique_ptr<Span> returnSpan(std::move(m_traceInfo->Spans.front()));
        m_traceInfo->Spans.pop_front();
        return returnSpan;
    }
}

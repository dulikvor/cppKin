#include "SpanContainer.h"
#include "Core/src/Assert.h"

using namespace std;
using namespace core;

namespace cppkin
{
#if defined(WIN32)
	static __declspec(thread) Span* m_span = nullptr; //Not unique ptr due to limitation on static initialization for TLS object.
#else
	static thread_local Span* m_span = nullptr;
#endif

	SpanContainer& SpanContainer::Instance(){
		static SpanContainer instance;
		return instance;
	}

	SpanContainer::~SpanContainer(){
		delete m_span;
	}

	Span& SpanContainer::GetSpan() const{
		return *m_span;
	}

	SpanContainer::SpanContainer(){}

    void SpanContainer::SetSpan(Span* span) {
        swap(m_span, span);
		delete span;
    }

    unique_ptr<Span> SpanContainer::ReleaseSpan(){
        Span* returnSpan = nullptr;
        VERIFY(m_span != nullptr, "Storage contains no span");
        swap(m_span, returnSpan);
        return unique_ptr<Span>(returnSpan);
    }
}

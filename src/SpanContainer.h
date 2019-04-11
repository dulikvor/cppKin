#pragma once

#include <thread>
#include <memory>
#include <forward_list>
#include "Export.h"
#include "Span.h"
#include "span_impl.h"
#if defined(WIN32)
#pragma warning( push )
#pragma warning (disable : 4251)
#endif

namespace cppkin
{
    class SpanContainer
    {
    public:
        static SpanContainer& Instance();
        ~SpanContainer();
        void PushSpan(Span& span);
        Span& TopSpan();
        void PopSpan();
        bool IsEmpty();
     private:
        SpanContainer();

    private:
        std::forward_list<Span*> m_spans;
    };
}
#if defined(WIN32)
#pragma warning( pop )
#endif
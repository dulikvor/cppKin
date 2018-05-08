#pragma once

#include <thread>
#include <memory>
#include <forward_list>
#include "Export.h"
#include "Span.h"
#include "span_impl.h"

namespace cppkin
{
    class CPPKIN_EXPORT SpanContainer
    {
    public:
        static SpanContainer& Instance();
        ~SpanContainer();
        void PushSpan(Span& span);
        Span* const  PopSpan();
     private:
        SpanContainer();

    private:
        std::forward_list<Span*> m_spans;
    };
}


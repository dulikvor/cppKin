#pragma once

#include <thread>
#include <memory>
#include "Span.h"

namespace cppkin
{
    class SpanContainer
    {
    public:
        static SpanContainer& Instance(){
            static SpanContainer instance;
            return instance;
        }
        Span& GetSpan() const{
            return *m_span.get();
        }
        void SetSpan(std::unique_ptr<Span> span);
        std::unique_ptr<Span> ReleaseSpan();
    private:
        SpanContainer(){}

    private:
        static thread_local std::unique_ptr<Span> m_span;
    };
}


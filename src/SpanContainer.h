#pragma once

#include <thread>
#include <memory>
#include <forward_list>
#include "Core/src/Export.h"
#include "Span.h"

namespace cppkin
{
    class A_EXPORT SpanContainer
    {
    public:
        static SpanContainer& Instance();
        ~SpanContainer();
        const Span* TopSpan() const;
        void ResetRootHeader() { m_rootHeader.reset(); }
        const Span::SpanHeader* GetRootHeader() const;
        void PushSpan(std::unique_ptr<Span>&& span);
        std::unique_ptr<Span> PopSpan();
    private:
        SpanContainer();

    private:
        std::forward_list<std::unique_ptr<Span>> m_spans;
        std::unique_ptr<Span::SpanHeader>        m_rootHeader;
    };
}


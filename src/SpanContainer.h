#pragma once

#include <thread>
#include <memory>
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
        const Span::SpanHeader* GetRootHeader() const;
        void PushSpan(Span* span);
        Span* PopSpan();
    private:
        SpanContainer();
    };
}


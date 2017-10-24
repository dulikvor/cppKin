#pragma once

#include <vector>

namespace cppkin
{
    class Span;

    class Transport
    {
    public:
        virtual void Submit(const std::vector<Span*>& spans) = 0;
    };
}


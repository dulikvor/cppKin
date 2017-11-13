#pragma once

#include <vector>

namespace cppkin
{
    class Span;

    class Transport
    {
    public:
        virtual ~Transport(){}
        virtual void Submit(const std::vector<Span*>& spans) = 0;
    };
}


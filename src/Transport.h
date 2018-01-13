#pragma once

#include <memory>
#include <vector>

namespace cppkin
{
    class Span;

    class Transport
    {
    public:
        virtual ~Transport(){}
        virtual void Submit(std::vector<std::unique_ptr<Span>>& spans) = 0;
    };
}


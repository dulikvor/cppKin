#pragma once

#include <memory>
#include <vector>

namespace cppkin
{
    class span_impl;

    class Transport
    {
    public:
        virtual ~Transport(){}
        virtual void Submit(std::vector<std::unique_ptr<span_impl>>& spans) = 0;
    };
}


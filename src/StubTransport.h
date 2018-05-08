#pragma once

#include "Transport.h"

namespace cppkin
{
    class StubTransport : public Transport
    {
    public:
        StubTransport(){}
        virtual ~StubTransport(){}
        void Submit(std::vector<std::unique_ptr<span_impl>>& spans) override {}
    };
}

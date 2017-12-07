#pragma once

#include "Transport.h"

namespace cppkin
{
    class StubTransport : public Transport
    {
    public:
        StubTransport(){}
        virtual ~StubTransport(){}
        void Submit(const std::vector<Span *> &spans) override{}
    };
}

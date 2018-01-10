#pragma once

#include "Transport.h"

namespace cppkin
{
    class HttpTransport  : public Transport
    {
    public:
        HttpTransport() {}
        virtual ~HttpTransport() {}
        void Submit(std::vector<std::unique_ptr<Span>>& spans) override;
    };
}

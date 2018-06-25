#pragma once

#include <unordered_map>
#include <memory>
#include "core/Exception.h"
#include "TransportType.h"
#include "Transport.h"

namespace cppkin
{
    class TransportCreator
    {
    public:
        virtual ~TransportCreator(){}
        virtual std::unique_ptr<Transport> Create() = 0;
    };

    template<typename ConcreteTransport>
    class ConcreteTransportCreator : public TransportCreator
    {
    public:
        virtual ~ConcreteTransportCreator(){}
        std::unique_ptr<Transport> Create() override {
            return std::unique_ptr<Transport>(new ConcreteTransport());
        }
    };

    class TransportFactory
    {
    public:
        static const TransportFactory& Instance();
        ~TransportFactory();
        std::unique_ptr<Transport> Create(TransportType type) const;
    private:
        TransportFactory();
    private:
        std::unordered_map<TransportType, std::unique_ptr<TransportCreator>, TransportType::Hash> m_transports;
    };
}

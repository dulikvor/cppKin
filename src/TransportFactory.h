#pragma once

#include <unordered_map>
#include <memory>
#include "Core/src/Exception.h"
#include "TransportType.h"
#include "Transport.h"

namespace cppkin
{
    class TransportCreator
    {
    public:
        virtual ~TransportCreator(){}
        virtual std::unique_ptr<Transport> Create(){
            throw core::Exception(SOURCE, "Method is not supported");
        }
    };
    template<typename ConcreteTransport>
    class ConcreteTransportCreator : public TransportCreator
    {
    public:
        virtual ~ConcreteTransportCreator(){}
        std::unique_ptr<Transport> Create() override{
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
        std::unordered_map<TransportType, TransportCreator*, TransportType::Hash> m_transports;
    };
}

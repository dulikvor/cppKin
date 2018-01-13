#include "TransportFactory.h"
#include <utility>
#include "ScribeTransport.h"
#include "HttpTransport.h"
#include "StubTransport.h"

using namespace std;
using namespace core;

namespace cppkin
{
    TransportFactory::~TransportFactory() {
    }

    const TransportFactory& TransportFactory::Instance() {
        static TransportFactory instance;
        return instance;
    }

    TransportFactory::TransportFactory() {
        m_transports.emplace(TransportType::Stub, unique_ptr<ConcreteTransportCreator<StubTransport>>(new ConcreteTransportCreator<StubTransport>()));
        m_transports.emplace(TransportType::Scribe, unique_ptr<ConcreteTransportCreator<ScribeTransport>>(new ConcreteTransportCreator<ScribeTransport>()));
        m_transports.emplace(TransportType::Http, unique_ptr<ConcreteTransportCreator<HttpTransport>>(new ConcreteTransportCreator<HttpTransport>()));
    }

    unique_ptr<Transport> TransportFactory::Create(TransportType type) const{
        auto it = m_transports.find(type);
        if (it == m_transports.end())
            throw Exception(SOURCE, "Requested transport type - %s is not supported", type.ToString().c_str());
        return it->second->Create();
    }
}

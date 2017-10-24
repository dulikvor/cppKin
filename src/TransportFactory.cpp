#include "TransportFactory.h"
#include <utility>
#include "ScribeTransport.h"

using namespace std;
using namespace core;

namespace cppkin
{
    TransportFactory::~TransportFactory() {
        for(auto& creatorPair : m_transports)
            delete creatorPair.second;
        m_transports.clear();
    }

    const TransportFactory& TransportFactory::Instance() {
        static TransportFactory instance;
        return instance;
    }

    TransportFactory::TransportFactory() {
        m_transports.insert(make_pair(TransportType::Scribe, new ConcreteTransportCreator<ScribeTransport>()));
    }

    unique_ptr<Transport> TransportFactory::Create(TransportType type) const{
        using dictionary = std::unordered_map<TransportType, TransportCreator*, TransportType::Hash>;
        dictionary::const_iterator it = m_transports.find(type);
        if(it == m_transports.end())
            throw Exception(SOURCE, "Requested transport type - %s is not supported", type.ToString().c_str());
        return it->second->Create();
    }
}

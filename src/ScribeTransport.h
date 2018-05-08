#pragma once

#include <memory>
#include "boost/smart_ptr/shared_ptr.hpp"
#include "thrift/transport/TSocket.h"
#include "thrift/protocol/TBinaryProtocol.h"
#include "GeneratedFiles/scribe.h"
#include "Transport.h"

namespace cppkin
{
    class ScribeTransport  : public Transport
    {
    public:
        ScribeTransport();
        virtual ~ScribeTransport();
        void Submit(std::vector<std::unique_ptr<span_impl>>& spans) override;
    private:
        boost::shared_ptr<apache::thrift::protocol::TBinaryProtocolT<apache::thrift::transport::TSocket>> m_protocol;
        boost::shared_ptr<apache::thrift::transport::TSocket> m_socket;
        std::unique_ptr<scribe::thrift::scribeClient> m_client;
    };
}

#include "ScribeTransport.h"
#include "ConfigParams.h"
#include "Encoder.h"
#include "EncodingContext.h"
#include "EncoderBase64.h"

using namespace std;
using namespace apache::thrift;
using namespace scribe::thrift;

namespace cppkin
{
    ScribeTransport::ScribeTransport() {
        m_socket.reset(new transport::TSocket(
                ConfigParams::Instance().GetHostAddress(), ConfigParams::Instance().GetPort()));
        m_protocol.reset(new protocol::TBinaryProtocolT<transport::TSocket>(m_socket));
        m_client = make_unique<scribeClient>(m_protocol);
        m_socket->open();

    }

    ScribeTransport::~ScribeTransport() {
        m_socket->close();
    }

    void ScribeTransport::Submit(const std::vector<Span*>& spans) {
        EncoderImpl<EncodingTypes::Thrift> encoder;
        EncoderContext context(spans, encoder);
        using Entry = scribe::thrift::LogEntry;
        vector<Entry> entries;

        for (auto& span : context) {
            string buffer = base64EncodeText(span.ToString());

            Entry entry;
            entry.__set_category("zipkin");
            entry.__set_message(buffer);
            entries.push_back(entry);
        }

        try {
            m_client->Log(entries);
        }
        catch(apache::thrift::transport::TTransportException& exc){
            if(m_socket->isOpen() == false)
                m_socket->open();
        }
    }
}

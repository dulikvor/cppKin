#include "EncodingContext.h"

using namespace std;
using namespace apache::thrift;

namespace cppkin
{
    EncoderContextThrift::EncoderContextThrift(): m_buffer(new transport::TMemoryBuffer()) {
        m_protocol.reset(new protocol::TBinaryProtocol(m_buffer));
    }
    string EncoderContextThrift::ToString(){
       m_protocol->writeListBegin(protocol::T_STRUCT, m_spans.size());
        for(const auto& span : m_spans)
           span.write(m_protocol.get());
        m_protocol->writeListEnd();
        return m_buffer->getBufferAsString();
    }

    void EncoderContextThrift::AddSpan(const ::Span &thriftSpan){
        m_spans.emplace_back(thriftSpan);
    }

    string EncoderContextByteStream::ToString() {
        return m_buffer.str();
    }

    void EncoderContextByteStream::Write(const char* data, int size){
        m_buffer.write(data, size);
    }

    void EncoderContextByteStream::Read(char* data, int size){
        m_buffer.read(data, size);
    }

    void EncoderContextByteStream::Clean() {
        m_buffer.str(string());
    }
}

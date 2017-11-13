#include "EncodingContext.h"
#include "Core/src/Assert.h"
#include "GeneratedFiles/zipkinCore_types.h"

using namespace std;
using namespace apache::thrift;

namespace cppkin
{
    EncoderContext::~EncoderContext() {}

    EncoderContextThrift::~EncoderContextThrift() {}

    EncoderContextThrift::EncoderContextThrift(): m_buffer(new transport::TMemoryBuffer()) {
        m_protocol.reset(new protocol::TBinaryProtocol(m_buffer));
    }
    EncoderContextThrift::EncoderContextThrift(const char* buf, uint32_t sz): m_buffer(new transport::TMemoryBuffer(const_cast<uint8_t*>((uint8_t*)buf), sz)) {
        m_protocol.reset(new protocol::TBinaryProtocol(m_buffer));
    }

    string EncoderContextThrift::ToString(){
        m_spans.front().write(m_protocol.get());
        m_spans.pop_front();
        return m_buffer->getBufferAsString();
    }

    ::Span EncoderContextThrift::ToSpan() {
        ::Span thriftSpan;
        thriftSpan.read(m_protocol.get());
        return thriftSpan;
    }

    void EncoderContextThrift::AddSpan(const ::Span &thriftSpan){
        m_spans.emplace_back(thriftSpan);
    }
}

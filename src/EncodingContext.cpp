#include "EncodingContext.h"
#include "Core/src/Assert.h"
#include "GeneratedFiles/zipkinCore_types.h"

using namespace std;
using namespace apache::thrift;

namespace cppkin
{
    EncoderContext::~EncoderContext() {}


    EncoderContextThrift::ContextElement::ContextElement(reference span, protocol& _protocol, buffer& _buffer):
            m_span(new ::Span(span)), m_protocol(_protocol), m_buffer(_buffer){}

    EncoderContextThrift::ContextElement::~ContextElement() {}

    string EncoderContextThrift::ContextElement::ToString() const{
        m_buffer.resetBuffer();
        m_span->write(&m_protocol);
        return m_buffer.getBufferAsString();
    }

    EncoderContextThrift::ContextElement::reference EncoderContextThrift::ContextElement::operator*() const{
        return *m_span;
    }
    EncoderContextThrift::ContextElement::pointer EncoderContextThrift::ContextElement::operator->() const{
        return m_span.get();
    }


    EncoderContextThrift::EncoderContextThriftIterator::EncoderContextThriftIterator(const std::list<ContextElement>::iterator& it)
            :m_it(it){}

    EncoderContextThrift::EncoderContextThriftIterator::~EncoderContextThriftIterator(){}

    EncoderContextThrift::EncoderContextThriftIterator::reference EncoderContextThrift::EncoderContextThriftIterator::operator*() const{
        return *m_it;
    }

    EncoderContextThrift::EncoderContextThriftIterator::pointer EncoderContextThrift::EncoderContextThriftIterator::operator->() const{
        return m_it.operator->();
    }

    EncoderContextThrift::EncoderContextThriftIterator::self& EncoderContextThrift::EncoderContextThriftIterator::operator++(int){
        m_it++;
        return *this;
    }

    bool EncoderContextThrift::EncoderContextThriftIterator::operator==(const self& obj) const{
        return m_it == obj.m_it;
    }

    bool EncoderContextThrift::EncoderContextThriftIterator::operator!=(const self& obj) const{
        return m_it != obj.m_it;
    }


    EncoderContextThrift::EncoderContextThriftConstIterator::EncoderContextThriftConstIterator(const std::list<ContextElement>::const_iterator& it)
            :m_it(it){}

    EncoderContextThrift::EncoderContextThriftConstIterator::~EncoderContextThriftConstIterator(){}

    EncoderContextThrift::EncoderContextThriftConstIterator::reference EncoderContextThrift::EncoderContextThriftConstIterator::operator*() const{
        return *m_it;
    }

    EncoderContextThrift::EncoderContextThriftConstIterator::pointer EncoderContextThrift::EncoderContextThriftConstIterator::operator->() const{
        return m_it.operator->();
    }

    EncoderContextThrift::EncoderContextThriftConstIterator::self& EncoderContextThrift::EncoderContextThriftConstIterator::operator++(int){
        m_it++;
        return *this;
    }

    bool EncoderContextThrift::EncoderContextThriftConstIterator::operator==(const self& obj) const{
        return m_it == obj.m_it;
    }

    bool EncoderContextThrift::EncoderContextThriftConstIterator::operator!=(const self& obj) const{
        return m_it != obj.m_it;
    }

    EncoderContextThrift::~EncoderContextThrift() {}

    EncoderContextThrift::EncoderContextThrift(): m_buffer(new transport::TMemoryBuffer()) {
        m_protocol.reset(new protocol::TBinaryProtocol(m_buffer));
    }

    EncoderContextThrift::EncoderContextThrift(const char* buf, uint32_t sz): m_buffer(new transport::TMemoryBuffer(const_cast<uint8_t*>((uint8_t*)buf), sz)) {
        m_protocol.reset(new protocol::TBinaryProtocol(m_buffer));
    }

    EncoderContextThrift::iterator EncoderContextThrift::begin(){
        return iterator(m_spans.begin());
    }

    EncoderContextThrift::iterator EncoderContextThrift::end(){
        return iterator(m_spans.end());
    }

    EncoderContextThrift::const_iterator EncoderContextThrift::begin() const{
        return const_iterator(m_spans.begin());
    }

    EncoderContextThrift::const_iterator EncoderContextThrift::end() const{
        return const_iterator(m_spans.end());
    }

    string EncoderContextThrift::ToString(){
        m_buffer->resetBuffer();
        m_protocol->writeListBegin(protocol::T_STRUCT, m_spans.size());
        for (auto &span : m_spans)
        {
            span->write(m_protocol.get());
        }

        m_protocol->writeListEnd();
        return m_buffer->getBufferAsString();
    }

    ::Span EncoderContextThrift::ToSpan() {
        ::Span thriftSpan;
        thriftSpan.read(m_protocol.get());
        return thriftSpan;
    }

    void EncoderContextThrift::AddSpan(const ::Span &thriftSpan){
        m_spans.emplace_back(const_cast<::Span&>(thriftSpan), *m_protocol, *m_buffer);
    }
}

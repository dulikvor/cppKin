#pragma once

#include <string>
#include <memory>
#include <list>
#include <sstream>
#include "boost/shared_ptr.hpp"
#include "thrift/protocol/TBinaryProtocol.h"
#include "thrift/transport/TBufferTransports.h"
#include "EncodingTypes.h"
#include "Encoder.h"

class Span;

namespace cppkin
{
    class Span;
    class SimpleAnnotation;

    class EncoderContext
    {
    public:
        virtual ~EncoderContext();
        virtual std::string ToString() = 0;
    };

    class EncoderContextThrift : public EncoderContext
    {

    private:
        class ContextElement
        {
        public:
            typedef ::Span* pointer;
            typedef ::Span& reference;
            typedef apache::thrift::protocol::TBinaryProtocol protocol;
            typedef apache::thrift::transport::TMemoryBuffer buffer;

            ContextElement(reference span, protocol& _protocol, buffer& _buffer);
            ~ContextElement();
            ContextElement(const ContextElement& obj) = delete;
            ContextElement& operator=(const ContextElement& obj) = delete;
            std::string ToString() const;

            reference operator*() const;
            pointer operator->() const;
        private:
            std::unique_ptr<::Span> m_span;
            protocol& m_protocol;
            buffer& m_buffer;
        };

    public:
        class EncoderContextThriftIterator
        {
        public:
            typedef EncoderContextThriftIterator self;
            typedef ContextElement* pointer;
            typedef ContextElement& reference;

            EncoderContextThriftIterator(const std::list<ContextElement>::iterator& it);
            ~EncoderContextThriftIterator();
            reference operator*() const;
            pointer operator->() const;
            self& operator++(int);
            bool operator==(const self& obj) const;
            bool operator!=(const self& obj) const;


        private:
            std::list<ContextElement>::iterator m_it;
        };


        class EncoderContextThriftConstIterator
        {
        public:
            typedef EncoderContextThriftConstIterator self;
            typedef const ContextElement* pointer;
            typedef const ContextElement& reference;

            EncoderContextThriftConstIterator(const std::list<ContextElement>::const_iterator& it);
            ~EncoderContextThriftConstIterator();
            reference operator*() const;
            pointer operator->() const;
            self& operator++(int);
            bool operator==(const self& obj) const;
            bool operator!=(const self& obj) const;


        private:
            std::list<ContextElement>::const_iterator m_it;
        };

    public:
        typedef EncoderContextThriftIterator iterator;
        typedef EncoderContextThriftConstIterator const_iterator;

        EncoderContextThrift();
        EncoderContextThrift(const char* buf, uint32_t sz);
        virtual ~EncoderContextThrift();
        iterator begin();
        iterator end();
        const_iterator begin() const;
        const_iterator end() const;
        std::string ToString();
        ::Span ToSpan();

    private:
        friend class Encoder<EncodingTypes::Thrift>;
        void AddSpan(const ::Span& thriftSpan);

    private:
        std::list<ContextElement> m_spans;
        std::unique_ptr<apache::thrift::protocol::TBinaryProtocol> m_protocol;
        boost::shared_ptr<apache::thrift::transport::TMemoryBuffer> m_buffer;
    };
}

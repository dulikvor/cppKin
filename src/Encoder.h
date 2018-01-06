#pragma once

#include <string>

#include "boost/shared_ptr.hpp"
#include "thrift/protocol/TBinaryProtocol.h"
#include "thrift/transport/TBufferTransports.h"

#include "Core/src/Enumeration.h"
#include "EncodingTypes.h"
#include "EncodingContext.h"
#include "Span.h"

class Span;

namespace cppkin
{
    class Span;
    class SimpleAnnotation;

    class Encoder {
    public:
        virtual ~Encoder() {}
        virtual std::string ToString(const Span&) const = 0;
        virtual std::string ToString(const std::vector<EncoderContext::ContextElement>&) const = 0;
        virtual std::string ToString(const std::vector<Span*>&) const = 0;
    };

    template<EncodingTypes::Enumeration>
    class EncoderImpl: public Encoder
    {
    public:
        EncoderImpl();
        virtual std::string ToString(const Span&) const {
            throw core::Exception(SOURCE, "Unsupported method");
        }

        virtual std::string ToString(const std::vector<EncoderContext::ContextElement>&) const {
            throw core::Exception(SOURCE, "Unsupported method");
        }

        virtual std::string ToString(const std::vector<Span*>&) {
            throw core::Exception(SOURCE, "Unsupported method");
        }
    };

    template<>
    class EncoderImpl<EncodingTypes::Thrift>: public Encoder
    {
    public:
        EncoderImpl();
        virtual std::string ToString(const Span&) const;
        virtual std::string ToString(const std::vector<EncoderContext::ContextElement>&) const;
        virtual std::string ToString(const std::vector<Span*>&) const;

    private:
        template <class T>
        std::string SpansVectorToString(T& spans) const;
        static ::Span Serialize(const Span& span);
        static void Serialize(::Span& thriftSpan, const SimpleAnnotation &annotation);
    private:
        std::unique_ptr<apache::thrift::protocol::TBinaryProtocol> m_protocol;
        boost::shared_ptr<apache::thrift::transport::TMemoryBuffer> m_buffer;
    };

    template<>
    class EncoderImpl<EncodingTypes::Json>: public Encoder
    {
    public:
        virtual std::string ToString(const Span&) const;
        virtual std::string ToString(const std::vector<EncoderContext::ContextElement>&) const;
        virtual std::string ToString(const std::vector<Span*>&) const;
    };
}


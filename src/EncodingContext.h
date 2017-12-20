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
        virtual std::string ToString( bool ) = 0;
    };

    class EncoderContextThrift : public EncoderContext
    {
    public:
        EncoderContextThrift();
        EncoderContextThrift(const char* buf, uint32_t sz);
        virtual ~EncoderContextThrift();
        std::string ToString( bool asList = false );
        ::Span ToSpan();

    private:
        friend class Encoder<EncodingTypes::Thrift>;
        void AddSpan(const ::Span& thriftSpan);
    private:
        std::list<::Span> m_spans;
        std::unique_ptr<apache::thrift::protocol::TBinaryProtocol> m_protocol;
        boost::shared_ptr<apache::thrift::transport::TMemoryBuffer> m_buffer;
    };
}

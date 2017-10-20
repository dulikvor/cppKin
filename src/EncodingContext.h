#pragma once

#include <string>
#include <memory>
#include <vector>
#include <sstream>
#include "boost/shared_ptr.hpp"
#include "GeneratedFiles/zipkinCore_types.h"
#include "thrift/protocol/TBinaryProtocol.h"
#include "thrift/transport/TBufferTransports.h"
#include "EncodingTypes.h"
#include "Encoder.h"

namespace cppkin
{
    class Span;
    class SimpleAnnotation;

    class EncoderContext
    {
    public:
        virtual std::string ToString() = 0;
    };

    class EncoderContextThrift : public EncoderContext
    {
    public:
        EncoderContextThrift();
        virtual ~EncoderContextThrift(){}
        std::string ToString();

    private:
        friend class Encoder<EncodingTypes::Thrift>;
        void AddSpan(const ::Span& thriftSpan);
    private:
        std::vector<::Span> m_spans;
        std::unique_ptr<apache::thrift::protocol::TBinaryProtocol> m_protocol;
        boost::shared_ptr<apache::thrift::transport::TMemoryBuffer> m_buffer;
    };


    class EncoderContextByteStream : public EncoderContext
    {
    public:
        EncoderContextByteStream();
        virtual ~EncoderContextByteStream(){}
        std::string ToString();

    private:
        friend class Encoder<EncodingTypes::ByteStream>;
        void Write(const char* data, int size);
        void Read(char* data, int size);
        void Clean();

    private:
        std::stringstream m_buffer;
    };
}

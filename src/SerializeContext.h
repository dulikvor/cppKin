#pragma once

#include <string>
#include <memory>
#include <vector>
#include <sstream>
#include "boost/shared_ptr.hpp"
#include "GeneratedFiles/zipkinCore_types.h"
#include "thrift/protocol/TBinaryProtocol.h"
#include "thrift/transport/TBufferTransports.h"
#include "SerializeType.h"
#include "Serializor.h"

namespace cppkin
{
    class Span;
    class SimpleAnnotation;

    class SerializeContext
    {
    public:
        virtual std::string ToString() = 0;
    };

    class SerializeContextThrift : SerializeContext
    {
    public:
        SerializeContextThrift();
        virtual ~SerializeContextThrift(){}
        std::string ToString();

    private:
        friend class Serializor<SerializeType::Thrift>;
        void AddSpan(const ::Span& thriftSpan);
    private:
        std::vector<::Span> m_spans;
        std::unique_ptr<apache::thrift::protocol::TBinaryProtocol> m_protocol;
        boost::shared_ptr<apache::thrift::transport::TMemoryBuffer> m_buffer;
    };


    class SerializeContextByteStream : SerializeContext
    {
    public:
        SerializeContextByteStream();
        virtual ~SerializeContextByteStream(){}
        std::string ToString();

    private:
        friend class Serializor<SerializeType::ByteStream>;
        void Write(const char* data, int size);
        void Read(char* data, int size);
        void Clean();

    private:
        std::stringstream m_buffer;
    };
}

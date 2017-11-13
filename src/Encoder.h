#pragma once

#include "Core/src/Enumeration.h"
#include "EncodingTypes.h"
#include "Span.h"

class Span;

namespace cppkin
{
    class Span;
    class SimpleAnnotation;
    class EncoderContext;

    template<EncodingTypes::Enumeration >
    class Encoder
    {
    public:
        static void Serialize(EncoderContext& context, const Span::SpanHeader& spanHeader){
            throw core::Exception(SOURCE, "Unsupported method");
        }
        static void Serialize(EncoderContext& context, const Span& span){
            throw core::Exception(SOURCE, "Unsupported method");
        }
        static Span::SpanHeader DeSerializeSpanHeader(EncoderContext& context){
            throw core::Exception(SOURCE, "Unsupported method");
        }
    };

    template<>
    class Encoder<EncodingTypes::Thrift>
    {
    public:
        static void Serialize(EncoderContext& context, const Span::SpanHeader& spanHeader);
        static void Serialize(EncoderContext& context, const Span& span);
        static Span::SpanHeader DeSerializeSpanHeader(EncoderContext& context);

    private:
        static void Serialize(::Span& thriftSpan, const SimpleAnnotation& annotation);
    };
}


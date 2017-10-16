#pragma once

#include "GeneratedFiles/zipkinCore_types.h"
#include "Core/src/Enumeration.h"
#include "SerializeType.h"

namespace cppkin
{
    class Span;
    class SimpleAnnotation;
    class SerializeContext;

    template<SerializeType::Enumeration >
    class Serializor
    {
    public:
        static void Serialize(SerializeContext& context, const Span& span){
            throw core::Exception(SOURCE, "Unsupported method");
        }
    };

    template<>
    class Serializor<SerializeType::Thrift>
    {
    public:
        static void Serialize(SerializeContext& context, const Span& span);

    private:
        static void Serialize(::Span& thriftSpan, const SimpleAnnotation& annotation);
    };
}


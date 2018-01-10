#pragma once

#include <unordered_map>
#include <string>
#include "EncodingTypes.h"
#include "EncodingContext.h"

namespace cppkin
{
    class Span;
    class SimpleAnnotation;

    class Encoder {
    public:
        virtual ~Encoder() {}
        virtual std::string ToString(const Span&) const = 0;
        virtual std::string ToString(const std::vector<EncoderContext::ContextElement>&) const = 0;
    };

    template<EncodingType::Enumeration>
    class EncoderImpl: public Encoder
    {
    public:
        EncoderImpl();
        std::string ToString(const Span&) const override {
            throw core::Exception(SOURCE, "Unsupported method");
        }

        std::string ToString(const std::vector<EncoderContext::ContextElement>&) const override {
            throw core::Exception(SOURCE, "Unsupported method");
        }
    };

    class EncoderCreator
    {
    public:
        virtual ~EncoderCreator() {}
        virtual std::unique_ptr<Encoder> Create() = 0;
    };

    template<typename ConcreteEncoder>
    class ConcreteEncoderCreator : public EncoderCreator
    {
    public:
        virtual ~ConcreteEncoderCreator() {}
        std::unique_ptr<Encoder> Create() override {
            return std::unique_ptr<Encoder>(new ConcreteEncoder());
        }
    };

    class EncoderFactory
    {
    public:
        static const EncoderFactory& Instance();
        ~EncoderFactory() {}
        std::unique_ptr<Encoder> Create(EncodingType type) const;
    private:
        EncoderFactory();
    private:
        std::unordered_map<EncodingType, std::unique_ptr<EncoderCreator>, EncodingType::Hash> m_encoders;
    };

}


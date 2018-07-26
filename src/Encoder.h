#pragma once

#include <unordered_map>
#include <string>
#include <sstream>
#include "boost/archive/iterators/base64_from_binary.hpp"
#include "boost/archive/iterators/binary_from_base64.hpp"
#include "boost/archive/iterators/transform_width.hpp"
#include "EncodingTypes.h"
#include "EncodingContext.h"

namespace cppkin
{
    class span_impl;
    class SimpleAnnotation;

    class Encoder {
        static const char* base64_padding[];
    public:
        static std::string base64EncodeText(const std::string& data) {
            using namespace boost::archive::iterators;
            typedef base64_from_binary<transform_width<std::string::const_iterator, 6, 8> > base64_enc;
            std::stringstream ss;
            std::copy(base64_enc(data.begin()), base64_enc(data.end()), std::ostream_iterator<char>(ss));
            ss << base64_padding[data.size() % 3];
            return ss.str();
        }

        virtual ~Encoder() {}
        virtual std::string ToString(const span_impl&) const = 0;
        virtual std::string ToString(const std::vector<EncoderContext::ContextElement>&) const = 0;
    };

    template<EncodingType::Enumeration>
    class EncoderImpl: public Encoder
    {
    public:
        EncoderImpl();
        std::string ToString(const span_impl&) const override {
            throw core::Exception(__CORE_SOURCE, "Unsupported method");
        }

        std::string ToString(const std::vector<EncoderContext::ContextElement>&) const override {
            throw core::Exception(__CORE_SOURCE, "Unsupported method");
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


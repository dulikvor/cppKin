#pragma once

#include <string>
#include <memory>
#include <vector>
#include "EncodingTypes.h"

namespace cppkin
{
    class Span;
    class Encoder;

    class EncoderContext
    {

    public:

        class ContextElement {

        public:
            ContextElement(std::unique_ptr<Span>&& span, const Encoder& encoder);
            std::string ToString() const;
            const Span& operator*() const { return *m_span; }

        private:
            std::unique_ptr<Span> m_span;
            const Encoder&        m_encoder;
        };

        typedef std::vector<ContextElement> ContextElementVector;
        typedef ContextElementVector::iterator iterator;
        typedef ContextElementVector::const_iterator const_iterator;

        EncoderContext();
        EncoderContext(std::vector<std::unique_ptr<Span>>& spans);
        virtual ~EncoderContext();
        iterator begin();
        iterator end();
        const_iterator begin() const;
        const_iterator end() const;
        std::string ToString() const;

    private:
        std::vector<ContextElement>    m_spans;
        const std::unique_ptr<Encoder> m_encoder;
    };
}

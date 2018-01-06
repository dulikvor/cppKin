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

            typedef const Span& reference;
            ContextElement(Span* span, const Encoder& encoder);
            std::string ToString() const;
            reference operator*() const { return *m_span; }

        private:
            std::unique_ptr<Span> m_span;
            const Encoder&        m_encoder;
        };

        typedef std::vector<ContextElement> ContextElementVector;
        typedef ContextElementVector::iterator iterator;
        typedef ContextElementVector::const_iterator const_iterator;

        EncoderContext();
        EncoderContext(const std::vector<Span*>& spans, const Encoder& encoder);
        virtual ~EncoderContext();
        iterator begin();
        iterator end();
        const_iterator begin() const;
        const_iterator end() const;
        std::string ToString() const;
        void AddSpan(const Span* span);

    private:
        std::vector<ContextElement> m_spans;
        const Encoder&              m_encoder;
    };
}

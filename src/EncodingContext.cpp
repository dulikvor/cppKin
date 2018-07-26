#include "EncodingContext.h"
#include "span_impl.h"
#include "Encoder.h"
#include "ConfigParams.h"

using namespace std;

namespace cppkin
{
    EncoderContext::ContextElement::ContextElement(span_impl*const span, const Encoder& encoder):
        m_span(span), m_encoder(encoder) {
    }

    EncoderContext::ContextElement::ContextElement(const ContextElement& obj):
        m_span(obj.m_span), m_encoder(obj.m_encoder){
    }

    string EncoderContext::ContextElement::ToString() const {
        return m_encoder.ToString(*m_span);
    }

    EncoderContext::~EncoderContext() {}

    EncoderContext::EncoderContext(const vector<unique_ptr<span_impl>>& spans):
        m_encoder(EncoderFactory::Instance().Create(ConfigParams::Instance().GetEncodingType()))
    {
        for (auto& span : spans) {
            m_spans.emplace_back(span.get(), *m_encoder);
        }
    }

    EncoderContext::iterator EncoderContext::begin() {
        return m_spans.begin();
    }

    EncoderContext::iterator EncoderContext::end() {
        return m_spans.end();
    }

    EncoderContext::const_iterator EncoderContext::begin() const {
        return m_spans.begin();
    }

    EncoderContext::const_iterator EncoderContext::end() const {
        return m_spans.end();
    }

    string EncoderContext::ToString() const {
        return m_encoder->ToString(m_spans);
    }

}

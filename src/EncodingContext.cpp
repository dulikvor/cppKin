#include "Encoder.h"
#include "GeneratedFiles/zipkinCore_types.h"

using namespace std;
using namespace apache::thrift;

namespace cppkin
{
    EncoderContext::ContextElement::ContextElement(Span* span, const Encoder& encoder):
        m_span(span), m_encoder(encoder) {
    }

    string EncoderContext::ContextElement::ToString() const {
        return m_encoder.ToString(*m_span);
    }

    EncoderContext::~EncoderContext() {}

    EncoderContext::EncoderContext(const std::vector<Span*>& spans, const Encoder& encoder):
        m_encoder(encoder)
    {
        for (auto& span : spans) {
            m_spans.emplace_back(span, m_encoder);
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
        return m_encoder.ToString(m_spans);
    }

    void EncoderContext::AddSpan(const Span* span) {
        m_spans.emplace_back(const_cast<Span*>(span), m_encoder); //, *m_protocol, *m_buffer);
    }
}

#include <string>
#include <boost/make_shared.hpp>
#include "thrift/protocol/TBinaryProtocol.h"
#include "thrift/transport/TBufferTransports.h"
#include "GeneratedFiles/zipkinCore_types.h"
#include "Encoder.h"
#include "ConfigParams.h"
#include "span_impl.h"
#include "SimpleAnnotation.h"

using namespace apache::thrift;

namespace cppkin {
    template<>
    class EncoderImpl<EncodingType::Thrift>: public Encoder
    {
    public:
        std::string ToString(const span_impl&) const override;
        std::string ToString(const std::vector<EncoderContext::ContextElement>&) const override;

    private:
        friend ConcreteEncoderCreator<EncoderImpl<EncodingType::Thrift>>;
        EncoderImpl();
        static ::Span Serialize(const span_impl& span);
        static void Serialize(::Span& thriftSpan, const SimpleAnnotation &annotation);
    private:
        std::unique_ptr<apache::thrift::protocol::TBinaryProtocol> m_protocol;
        boost::shared_ptr<apache::thrift::transport::TMemoryBuffer> m_buffer;
    };

    inline EncoderImpl<EncodingType::Thrift>::EncoderImpl(): m_buffer(boost::make_shared<transport::TMemoryBuffer>()) {
        m_protocol.reset(new protocol::TBinaryProtocol(m_buffer));
    }

    inline ::Span EncoderImpl<EncodingType::Thrift>::Serialize(const span_impl& span) {
        ::Span thriftSpan;
        thriftSpan.__set_trace_id(span.GetHeader().TraceID);
        thriftSpan.__set_name(span.GetHeader().Name);
        thriftSpan.__set_id(span.GetHeader().ID);
        thriftSpan.__set_debug(ConfigParams::Instance().GetDebug());
        thriftSpan.__set_timestamp(span.GetTimeStamp());
        thriftSpan.__set_duration(span.GetDuration());
        if(span.GetHeader().ParentIdSet)
            thriftSpan.__set_parent_id(span.GetHeader().ParentID);
        for(auto& annotation : span.GetAnnotations())
            if(annotation->GetType() == AnnotationType::Simple)
                Serialize(thriftSpan, static_cast<const SimpleAnnotation&>(*annotation));

        return thriftSpan;
    }

    inline void EncoderImpl<EncodingType::Thrift>::Serialize(::Span& thriftSpan, const SimpleAnnotation &annotation) {
        ::Annotation thriftAnnotation;
        thriftAnnotation.__set_value(annotation.GetEvent());
        thriftAnnotation.__set_timestamp(annotation.GetTimeStamp());
        const Annotation::EndPoint& endPoint = annotation.GetEndPoint();
        ::Endpoint thriftEndPoint;
        thriftEndPoint.__set_service_name(endPoint.ServiceName);

        thriftEndPoint.__set_ipv4(endPoint.BinaryHost);
        thriftEndPoint.__set_port(endPoint.Port);

        thriftAnnotation.__set_host(thriftEndPoint);
        thriftSpan.annotations.emplace_back(thriftAnnotation);
    }

    inline string EncoderImpl<EncodingType::Thrift>::ToString(const span_impl& span) const {
        ::Span thriftSpan = EncoderImpl<EncodingType::Thrift>::Serialize(span);
        m_buffer->resetBuffer();
        thriftSpan.write(m_protocol.get());
        return m_buffer->getBufferAsString();
    }

    inline string EncoderImpl<EncodingType::Thrift>::ToString(const std::vector<EncoderContext::ContextElement>& spans) const {
        m_buffer->resetBuffer();
        m_protocol->writeListBegin(protocol::T_STRUCT, spans.size());
        for (auto &span : spans) {
            ::Span thriftSpan = EncoderImpl<EncodingType::Thrift>::Serialize(*span);
            thriftSpan.write(m_protocol.get());
        }
        m_protocol->writeListEnd();
        return m_buffer->getBufferAsString();
    }


}
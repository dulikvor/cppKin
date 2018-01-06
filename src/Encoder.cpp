#include <string>
#include <boost/make_shared.hpp>

#include "GeneratedFiles/zipkinCore_types.h"
#include "Encoder.h"
#include "Span.h"
#include "SimpleAnnotation.h"
#include "EncodingContext.h"
#include "ConfigParams.h"

using namespace apache::thrift;
using namespace core;
using namespace std;

namespace cppkin
{
    EncoderImpl<EncodingTypes::Thrift>::EncoderImpl(): m_buffer(boost::make_shared<transport::TMemoryBuffer>()) {
        m_protocol.reset(new protocol::TBinaryProtocol(m_buffer));
    }

    ::Span EncoderImpl<EncodingTypes::Thrift>::Serialize(const Span& span) {
        ::Span thriftSpan;
        thriftSpan.__set_trace_id(span.GetHeader().TraceID);
        thriftSpan.__set_name(span.GetHeader().Name);
        thriftSpan.__set_id(span.GetHeader().ID);
        thriftSpan.__set_debug(ConfigParams::Instance().GetDebug());
        thriftSpan.__set_timestamp(span.GetTimeStamp());
        thriftSpan.__set_duration(span.GetDuration());
        if(span.GetHeader().ParentIDSet)
            thriftSpan.__set_parent_id(span.GetHeader().ParentID);
        for(auto& annotation : span.GetAnnotations())
            if(annotation->GetType() == AnnotationType::Simple)
                Serialize(thriftSpan, static_cast<const SimpleAnnotation&>(*annotation));

        return thriftSpan;
    }

    void EncoderImpl<EncodingTypes::Thrift>::Serialize(::Span& thriftSpan, const SimpleAnnotation &annotation) {
        ::Annotation thriftAnnotation;
        thriftAnnotation.__set_value(annotation.GetEvent());
        thriftAnnotation.__set_timestamp(annotation.GetTimeStamp());
        const Annotation::EndPoint& endPoint = annotation.GetEndPoint();
        ::Endpoint thriftEndPoint;
        thriftEndPoint.__set_service_name(endPoint.ServiceName);

        thriftEndPoint.__set_ipv4(endPoint.Host);
        thriftEndPoint.__set_port(endPoint.Port);

        thriftAnnotation.__set_host(thriftEndPoint);
        thriftSpan.annotations.emplace_back(thriftAnnotation);
    }

    string EncoderImpl<EncodingTypes::Thrift>::ToString(const Span& span) const {
        ::Span thriftSpan = EncoderImpl<EncodingTypes::Thrift>::Serialize(span);
        m_buffer->resetBuffer();
        thriftSpan.write(m_protocol.get());
        return m_buffer->getBufferAsString();
    }

    template <class T>
    std::string EncoderImpl<EncodingTypes::Thrift>::SpansVectorToString(T& spans) const {
        m_buffer->resetBuffer();
        m_protocol->writeListBegin(protocol::T_STRUCT, spans.size());
        for (auto &span : spans) {
            ::Span thriftSpan = EncoderImpl<EncodingTypes::Thrift>::Serialize(*span);
            thriftSpan.write(m_protocol.get());
        }
        m_protocol->writeListEnd();
        return m_buffer->getBufferAsString();
    }

    string EncoderImpl<EncodingTypes::Thrift>::ToString(const std::vector<EncoderContext::ContextElement>& spans) const {
        return SpansVectorToString(spans);
    }

    string EncoderImpl<EncodingTypes::Thrift>::ToString(const std::vector<Span*>& spans) const {
        return SpansVectorToString(spans);
    }

    string EncoderImpl<EncodingTypes::Json>::ToString(const Span& span) const {
        return "not ready yet";
    }

    string EncoderImpl<EncodingTypes::Json>::ToString(const std::vector<EncoderContext::ContextElement>&) const {
        return "not ready yet";
    }

    string EncoderImpl<EncodingTypes::Json>::ToString(const std::vector<Span*>& spans) const {
        return "not ready yet";
    }

//

//
//    void Encoder<EncodingTypes::Thrift>::Serialize(EncoderContext& context, const Span::SpanHeader& spanHeader) {
////        EncoderContextThrift& thriftContext = static_cast<EncoderContextThrift&>(context);
////        ::Span thriftSpan;
////        thriftSpan.__set_trace_id(spanHeader.TraceID);
////        thriftSpan.__set_name(spanHeader.Name);
////        thriftSpan.__set_id(spanHeader.ID);
////        if(spanHeader.ParentIDSet)
////            thriftSpan.__set_parent_id(spanHeader.ParentID);
////        thriftContext.AddSpan(thriftSpan);
//    }
//
//    Span::SpanHeader Encoder<EncodingTypes::Thrift>::DeSerializeSpanHeader(EncoderContext &context) {
////        EncoderContextThrift& thriftContext = static_cast<EncoderContextThrift&>(context);
////        ::Span thriftSpan = thriftContext.ToSpan();
////        if(thriftSpan.__isset.parent_id == true) {
////            Span::SpanHeader spanHeader(thriftSpan.name, thriftSpan.trace_id, thriftSpan.parent_id, thriftSpan.id);
////            return spanHeader;
////        }
////        else{
////            Span::SpanHeader spanHeader(thriftSpan.name, thriftSpan.trace_id, thriftSpan.id);
////            return spanHeader;
////        }
//    }
//
//    void Encoder<EncodingTypes::Json>::Serialize(EncoderContext& context, const Span& span) {
//
//    }
//
//    void Encoder<EncodingTypes::Json>::Serialize(::Span& thriftSpan, const SimpleAnnotation &annotation) {
//
//    }
//
//    void Encoder<EncodingTypes::Json>::Serialize(EncoderContext& context, const Span::SpanHeader& spanHeader) {
//
//    }
//
//    Span::SpanHeader Encoder<EncodingTypes::Json>::DeSerializeSpanHeader(EncoderContext &context) {
//
//    }
}

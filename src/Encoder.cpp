#include "Encoder.h"
#include <string>
#include "GeneratedFiles/zipkinCore_types.h"
#include "Span.h"
#include "SimpleAnnotation.h"
#include "EncodingContext.h"
#include "ConfigParams.h"

using namespace core;
using namespace std;

namespace cppkin
{
    void Encoder<EncodingTypes::Thrift>::Serialize(EncoderContext& context, const Span& span){
        EncoderContextThrift& thriftContext = static_cast<EncoderContextThrift&>(context);
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
        thriftContext.AddSpan(thriftSpan);
    }

    void Encoder<EncodingTypes::Thrift>::Serialize(::Span& thriftSpan, const SimpleAnnotation &annotation) {
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

    void Encoder<EncodingTypes::Thrift>::Serialize(EncoderContext& context, const Span::SpanHeader& spanHeader){
        EncoderContextThrift& thriftContext = static_cast<EncoderContextThrift&>(context);
        ::Span thriftSpan;
        thriftSpan.__set_trace_id(spanHeader.TraceID);
        thriftSpan.__set_name(spanHeader.Name);
        thriftSpan.__set_id(spanHeader.ID);
        if(spanHeader.ParentIDSet)
            thriftSpan.__set_parent_id(spanHeader.ParentID);
        thriftContext.AddSpan(thriftSpan);
    }

    Span::SpanHeader Encoder<EncodingTypes::Thrift>::DeSerializeSpanHeader(EncoderContext &context) {
        EncoderContextThrift& thriftContext = static_cast<EncoderContextThrift&>(context);
        ::Span thriftSpan = thriftContext.ToSpan();
        if(thriftSpan.__isset.parent_id == true) {
            Span::SpanHeader spanHeader(thriftSpan.name, thriftSpan.trace_id, thriftSpan.parent_id, thriftSpan.id);
            return spanHeader;
        }
        else{
            Span::SpanHeader spanHeader(thriftSpan.name, thriftSpan.trace_id, thriftSpan.id);
            return spanHeader;
        }
    }
}

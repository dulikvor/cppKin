#include <string>
#include <sstream>
#include <boost/make_shared.hpp>
#include "Poco/JSON/Array.h"

#include "GeneratedFiles/zipkinCore_types.h"
#include "Encoder.h"
#include "SimpleAnnotation.h"
#include "EncodingContext.h"
#include "ConfigParams.h"

using namespace apache::thrift;
using namespace core;
using namespace std;
namespace json = Poco::JSON;

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

    string EncoderImpl<EncodingTypes::Thrift>::ToString(const std::vector<EncoderContext::ContextElement>& spans) const {
        m_buffer->resetBuffer();
        m_protocol->writeListBegin(protocol::T_STRUCT, spans.size());
        for (auto &span : spans) {
            ::Span thriftSpan = EncoderImpl<EncodingTypes::Thrift>::Serialize(*span);
            thriftSpan.write(m_protocol.get());
        }
        m_protocol->writeListEnd();
        return m_buffer->getBufferAsString();
    }

    json::Object EncoderImpl<EncodingTypes::Json>::Serialize(const Span& span) {
        json::Object jsonSpan;
        jsonSpan.set("traceId", span.GetHeader().TraceID);
        jsonSpan.set("name", span.GetHeader().Name);
        jsonSpan.set("id", span.GetHeader().ID);
        jsonSpan.set("debug", ConfigParams::Instance().GetDebug());
        jsonSpan.set("timestamp", span.GetTimeStamp());
        jsonSpan.set("duration", span.GetDuration());

        if(span.GetHeader().ParentIDSet)
            jsonSpan.set("parentId", span.GetHeader().ParentID);

        json::Array jsonAnnotations;
        for(auto& annotation : span.GetAnnotations())
            if(annotation->GetType() == AnnotationType::Simple)
                Serialize(jsonAnnotations, static_cast<const SimpleAnnotation&>(*annotation));
        jsonSpan.set("annotations", jsonAnnotations);

        return jsonSpan;
    }

    void EncoderImpl<EncodingTypes::Json>::Serialize(json::Array& jsonAnnotations, const SimpleAnnotation &annotation) {
        json::Object jsonAnnotation;
        jsonAnnotation.set("value", annotation.GetEvent());
        jsonAnnotation.set("timestamp", annotation.GetTimeStamp());

        const Annotation::EndPoint& endPoint = annotation.GetEndPoint();
        json::Object jsonEndPoint;
        jsonEndPoint.set("serviceName", endPoint.ServiceName);
        jsonEndPoint.set("ipv4", endPoint.Host);
        jsonEndPoint.set("port", endPoint.Port);
        jsonAnnotation.set( "endpoint", jsonEndPoint);

        jsonAnnotations.add(jsonAnnotation);
    }

    string EncoderImpl<EncodingTypes::Json>::ToString(const Span& span) const {
        json::Object jsonSpan = EncoderImpl<EncodingTypes::Json>::Serialize(span);
        ostringstream oss;
        jsonSpan.stringify(oss);
        return oss.str();
    }

    string EncoderImpl<EncodingTypes::Json>::  ToString(const std::vector<EncoderContext::ContextElement>& spans) const {

        json::Array jsonSpans;
        for (auto& span : spans) {
            json::Object jsonSpan = EncoderImpl<EncodingTypes::Json>::Serialize(*span);
            jsonSpans.add(jsonSpan);
        }

        ostringstream oss;
        jsonSpans.stringify(oss);
        return oss.str();
    }
}

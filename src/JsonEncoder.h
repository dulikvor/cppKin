#include <string>
#include "Poco/JSON/Object.h"
#include "Poco/JSON/Array.h"
#include "Encoder.h"
#include "span_impl.h"
#include "ConfigParams.h"
#include "SimpleAnnotation.h"
#include "BinaryAnnotation.h"

namespace json = Poco::JSON;

namespace cppkin {

    template<>
    class EncoderImpl<EncodingType::Json> : public Encoder {
    public:
        std::string ToString(const span_impl &) const override;
        std::string ToString(const std::vector<EncoderContext::ContextElement> &) const override;

    private:
        friend ConcreteEncoderCreator<EncoderImpl<EncodingType::Json>>;
        EncoderImpl() = default;
        static Poco::JSON::Object Serialize(const span_impl &span);
        static void Serialize(Poco::JSON::Array &jsonSpan, const SimpleAnnotation &annotation);
        static void Serialize(Poco::JSON::Array &jsonSpan, const BinaryAnnotation &annotation);
    };

    inline json::Object EncoderImpl<EncodingType::Json>::Serialize(const span_impl& span) {
        json::Object jsonSpan;
        jsonSpan.set("traceId", std::to_string(span.GetHeader().TraceID));
        jsonSpan.set("name", span.GetHeader().Name);
        jsonSpan.set("id", std::to_string(span.GetHeader().ID));
        jsonSpan.set("debug", ConfigParams::Instance().GetDebug());
        jsonSpan.set("timestamp", span.GetTimeStamp());
        jsonSpan.set("duration", span.GetDuration());

        if(span.GetHeader().ParentIdSet)
            jsonSpan.set("parentId", std::to_string(span.GetHeader().ParentID));

        json::Array jsonAnnotations;
        json::Array jsonBinaryAnnotations;
        for(auto& annotation : span.GetAnnotations())
            if(annotation->GetType() == AnnotationType::Simple)
                Serialize(jsonAnnotations, static_cast<const SimpleAnnotation&>(*annotation));
            else
                Serialize(jsonBinaryAnnotations, static_cast<const BinaryAnnotation&>(*annotation));
            
        jsonSpan.set("annotations", jsonAnnotations);
        jsonSpan.set("binaryAnnotations", jsonBinaryAnnotations);

        return jsonSpan;
    }

    inline void EncoderImpl<EncodingType::Json>::Serialize(json::Array& jsonAnnotations, const SimpleAnnotation &annotation) {
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
    
    inline void EncoderImpl<EncodingType::Json>::Serialize(json::Array& jsonAnnotations, const BinaryAnnotation &annotation) {
        json::Object jsonAnnotation;
        jsonAnnotation.set("key", annotation.GetKey());
        switch(annotation.GetValueType())
        {
            case BinaryValueTypes::Boolean:
                bool bool_value;
                annotation.GetValue(bool_value);
                jsonAnnotation.set("value", bool_value);
                break;
            case BinaryValueTypes::String:
                std::string str_value;
                annotation.GetValue(str_value);
                jsonAnnotation.set("value", str_value);
                break;
        }
        
        const Annotation::EndPoint& endPoint = annotation.GetEndPoint();
        json::Object jsonEndPoint;
        jsonEndPoint.set("serviceName", endPoint.ServiceName);
        jsonEndPoint.set("ipv4", endPoint.Host);
        jsonEndPoint.set("port", endPoint.Port);
        jsonAnnotation.set( "endpoint", jsonEndPoint);
        
        jsonAnnotations.add(jsonAnnotation);
    }

    inline std::string EncoderImpl<EncodingType::Json>::ToString(const span_impl& span) const {
        json::Object jsonSpan = EncoderImpl<EncodingType::Json>::Serialize(span);
        std::ostringstream oss;
        jsonSpan.stringify(oss);
        return oss.str();
    }

    inline std::string EncoderImpl<EncodingType::Json>::ToString(const std::vector<EncoderContext::ContextElement>& spans) const {

        json::Array jsonSpans;
        for (const auto& span : spans) {
            json::Object jsonSpan = EncoderImpl<EncodingType::Json>::Serialize(*span);
            jsonSpans.add(jsonSpan);
        }

        std::ostringstream oss;
        jsonSpans.stringify(oss);
        return oss.str();
    }
}

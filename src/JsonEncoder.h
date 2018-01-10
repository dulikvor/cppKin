#include <string>
#include "Poco/JSON/Object.h"
#include "Poco/JSON/Array.h"
#include "Encoder.h"
#include "Span.h"
#include "ConfigParams.h"
#include "SimpleAnnotation.h"

using namespace std;
namespace json = Poco::JSON;

namespace cppkin {

    template<>
    class EncoderImpl<EncodingType::Json> : public Encoder {
    public:
        virtual std::string ToString(const Span &) const override;
        virtual std::string ToString(const std::vector<EncoderContext::ContextElement> &) const override;

    private:
        static Poco::JSON::Object Serialize(const Span &span);

        static void Serialize(Poco::JSON::Array &jsonSpan, const SimpleAnnotation &annotation);
    };

    json::Object EncoderImpl<EncodingType::Json>::Serialize(const Span& span) {
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

    void EncoderImpl<EncodingType::Json>::Serialize(json::Array& jsonAnnotations, const SimpleAnnotation &annotation) {
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

    string EncoderImpl<EncodingType::Json>::ToString(const Span& span) const {
        json::Object jsonSpan = EncoderImpl<EncodingType::Json>::Serialize(span);
        ostringstream oss;
        jsonSpan.stringify(oss);
        return oss.str();
    }

    string EncoderImpl<EncodingType::Json>::  ToString(const std::vector<EncoderContext::ContextElement>& spans) const {

        json::Array jsonSpans;
        for (auto& span : spans) {
            json::Object jsonSpan = EncoderImpl<EncodingType::Json>::Serialize(*span);
            jsonSpans.add(jsonSpan);
        }

        ostringstream oss;
        jsonSpans.stringify(oss);
        return oss.str();
    }
}
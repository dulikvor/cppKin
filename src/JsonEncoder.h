#include <string>
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include "Encoder.h"
#include "span_impl.h"
#include "ConfigParams.h"
#include "SimpleAnnotation.h"
#include "BinaryAnnotation.h"

namespace cppkin {
    
    static std::string to_hex(std::int64_t value)
    {
        std::ostringstream os;
        os << std::hex << value;
        return os.str();
    }
    
    template<>
    class EncoderImpl<EncodingType::Json> : public Encoder {
    public:
        std::string ToString(const span_impl &) const override;
        std::string ToString(const std::vector<EncoderContext::ContextElement> &) const override;

    private:
        typedef rapidjson::StringBuffer jbuffer;
        typedef rapidjson::Writer<jbuffer> jwriter;
        friend ConcreteEncoderCreator<EncoderImpl<EncodingType::Json>>;
        EncoderImpl() = default;
        static void Serialize(jwriter &writer, const span_impl &span);
        static void Serialize(jwriter &writer, const SimpleAnnotation &annotation);
        static void Serialize(jwriter &writer, const BinaryAnnotation &annotation);
    };

    inline void EncoderImpl<EncodingType::Json>::Serialize(jwriter& writer, const span_impl& span) {
        writer.StartObject();
        writer.Key("traceId");
        writer.String(to_hex(span.GetHeader().TraceID).c_str());
        writer.Key("name");
        writer.String(span.GetHeader().Name.c_str());
        writer.Key("id");
        writer.String(to_hex(span.GetHeader().ID).c_str());
        writer.Key("debug");
        writer.Bool(ConfigParams::Instance().GetDebug());
        writer.Key("timestamp");
        writer.Int64(span.GetTimeStamp());
        writer.Key("duration");
        writer.Int64(span.GetDuration());

        if(span.GetHeader().ParentIdSet)
        {
            writer.Key("parentId");
            writer.String(to_hex(span.GetHeader().ParentID).c_str());
        }
    
        {
            writer.Key("annotations");
            writer.StartArray();
            for (auto &annotation : span.GetAnnotations())
                if (annotation->GetType() == AnnotationType::Simple)
                    Serialize(writer, static_cast<const SimpleAnnotation &>(*annotation));
        
            writer.EndArray();
        }
    
        {
            writer.Key("binaryAnnotations");
            writer.StartArray();
            for (auto &annotation : span.GetAnnotations())
                if (annotation->GetType() == AnnotationType::Binary)
                    Serialize(writer, static_cast<const BinaryAnnotation &>(*annotation));
        
            writer.EndArray();
        }
    
        writer.EndObject();
    }

    inline void EncoderImpl<EncodingType::Json>::Serialize(jwriter& writer, const SimpleAnnotation &annotation) {
        writer.StartObject();
        {
            writer.Key("value");
            writer.String(annotation.GetEvent().c_str());
            writer.Key("timestamp");
            writer.Int64(annotation.GetTimeStamp());
            {
                const Annotation::EndPoint &endPoint = annotation.GetEndPoint();
        
                writer.Key("endpoint");
                writer.StartObject();
                writer.Key("serviceName");
                writer.String(endPoint.ServiceName.c_str());
                writer.Key("ipv4");
                writer.String(endPoint.Host.c_str());
                writer.Key("port");
                writer.Int(endPoint.Port);
                writer.EndObject();
            }
        }
        writer.EndObject();
    }
    
    inline void EncoderImpl<EncodingType::Json>::Serialize(jwriter &writer, const BinaryAnnotation &annotation) {
        writer.StartObject();
        writer.Key("key");
        writer.String(annotation.GetKey().c_str());
        
        writer.Key("value");
        switch(annotation.GetValueType())
        {
            case BinaryValueTypes::Boolean:
                bool bool_value;
                annotation.GetValue(bool_value);
                writer.Bool(bool_value);
                break;
            case BinaryValueTypes::String:
                std::string str_value;
                annotation.GetValue(str_value);
                writer.String(str_value.c_str());
                break;
        }
    
        {
            const Annotation::EndPoint &endPoint = annotation.GetEndPoint();
        
            writer.Key("endpoint");
            writer.StartObject();
            writer.Key("serviceName");
            writer.String(endPoint.ServiceName.c_str());
            writer.Key("ipv4");
            writer.String(endPoint.Host.c_str());
            writer.Key("port");
            writer.Int(endPoint.Port);
            writer.EndObject();
        }
        writer.EndObject();
    }

    inline std::string EncoderImpl<EncodingType::Json>::ToString(const span_impl& span) const {
        jbuffer buffer;
        jwriter writer(buffer);
        EncoderImpl<EncodingType::Json>::Serialize(writer, span);
        return std::string(buffer.GetString());
    }

    inline std::string EncoderImpl<EncodingType::Json>::ToString(const std::vector<EncoderContext::ContextElement>& spans) const
    {
        jbuffer buffer;
        jwriter writer(buffer);
        
        writer.StartArray();
        for (const auto& span : spans)
            EncoderImpl<EncodingType::Json>::Serialize(writer, *span);
        
        writer.EndArray();
        return std::string(buffer.GetString());
    }
}

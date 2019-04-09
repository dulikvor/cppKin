#include "Encoder.h"

#ifdef THRIFT_FOUND
#include "ThriftEncoder.h"
#endif

#ifdef RAPID_JSON_FOUND
#include "JsonEncoder.h"
#endif

using namespace core;
using namespace std;

namespace cppkin
{
    const char* Encoder::base64_padding[] = {"", "==","="};

    const EncoderFactory& EncoderFactory::Instance() {
        static EncoderFactory instance;
        return instance;
    }

    EncoderFactory::EncoderFactory() {

#ifdef THRIFT_FOUND
        m_encoders.emplace(EncodingType::Thrift,
                           unique_ptr<ConcreteEncoderCreator<EncoderImpl<EncodingType::Thrift>>>(new ConcreteEncoderCreator<EncoderImpl<EncodingType::Thrift>>()));
#endif

#ifdef RAPID_JSON_FOUND
        m_encoders.emplace(EncodingType::Json,
                           unique_ptr<ConcreteEncoderCreator<EncoderImpl<EncodingType::Json>>>(new ConcreteEncoderCreator<EncoderImpl<EncodingType::Json>>()));
#endif
    }

    unique_ptr<Encoder> EncoderFactory::Create(EncodingType type) const{
        auto it = m_encoders.find(type);
        if (it == m_encoders.end())
            throw Exception(__CORE_SOURCE, "Requested encoder type - %s is not supported", type.ToString().c_str());
        return it->second->Create();
    }
}

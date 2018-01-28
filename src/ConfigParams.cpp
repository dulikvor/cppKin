#include "ConfigParams.h"
#include "Core/src/GeneralParams.h"
#include "ConfigTags.h"

using namespace std;
using namespace core;
namespace cppkin
{
    ConfigParams& ConfigParams::Instance()
    {
        static ConfigParams instance;
        return instance;
    }

    ConfigParams::ConfigParams():
            m_transportType(TransportType::Stub), m_encodingType(EncodingType::Thrift)
    {}

    void ConfigParams::Load(const GeneralParams& configParams)
    {
        m_transportType = TransportType::FromString(configParams.Get<string>(ConfigTags::TRANSPORT_TYPE));
        m_hostAddress = configParams.Get<string>(ConfigTags::HOST_ADDRESS);
        m_port = configParams.Get<int>(ConfigTags::PORT);
        m_serviceName = configParams.Get<string>(ConfigTags::SERVICE_NAME);
        m_debug = configParams.Get<bool>(ConfigTags::DEBUG);
        m_sampleCount = configParams.Get<int>(ConfigTags::SAMPLE_COUNT);
        m_encodingType = EncodingType::FromString(configParams.Get<string>(ConfigTags::ENCODING_TYPE));
    }
}

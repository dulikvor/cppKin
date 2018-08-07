#include "ConfigParams.h"
#include "core/GeneralParams.h"
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
            m_transportType(TransportType::Stub), m_debug(false), m_sampleCount(1000), m_encodingType(EncodingType::Json), m_batchSize(50)
    {}

    void ConfigParams::Load(const GeneralParams& configParams)
    {
        m_hostAddress = configParams.Get<string>(ConfigTags::HOST_ADDRESS);
        m_port = configParams.Get<int>(ConfigTags::PORT);
        m_serviceName = configParams.Get<string>(ConfigTags::SERVICE_NAME);

        if(configParams.Exists(ConfigTags::DEBUG))
            m_debug = configParams.Get<bool>(ConfigTags::DEBUG);
        if(configParams.Exists(ConfigTags::ENCODING_TYPE))
            m_encodingType = EncodingType::FromString(configParams.Get<string>(ConfigTags::ENCODING_TYPE));
        if(configParams.Exists(ConfigTags::TRANSPORT_TYPE))
            m_transportType = TransportType::FromString(configParams.Get<string>(ConfigTags::TRANSPORT_TYPE));
        if(configParams.Exists(ConfigTags::BATCH_SIZE))
            m_batchSize = configParams.Get<int>(ConfigTags::BATCH_SIZE);
        if(configParams.Exists(ConfigTags::SAMPLE_COUNT))
            m_sampleCount = configParams.Get<int>(ConfigTags::SAMPLE_COUNT);
    }
}

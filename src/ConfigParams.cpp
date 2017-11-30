#include "ConfigParams.h"
#include "GeneralParams.h"
#define EXTERN_IMPL
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

	void ConfigParams::Load(const GeneralParams& configParams)
	{
		m_hostAddress = StringConverter::Convert(configParams.GetValue(ConfigTags::HOST_ADDRESS));
		m_port = configParams.GetValue(ConfigTags::PORT);
		m_serviceName = StringConverter::Convert(configParams.GetValue(ConfigTags::SERVICE_NAME));
		m_debug = configParams.GetValue(ConfigTags::DEBUG);
		m_sampleCount = configParams.GetValue(ConfigTags::SAMPLE_COUNT);
	}
}

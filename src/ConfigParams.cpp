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

	ConfigParams::~ConfigParams(){}

	ConfigParams::ConfigParams():m_transportType(TransportType::Stub){}

	void ConfigParams::Load(const GeneralParams& configParams)
	{
		m_transportType = TransportType::FromString(configParams.GetValue(ConfigTags::TRANSPORT_TYPE));
		m_hostAddress = StringConverter::Convert(configParams.GetValue(ConfigTags::HOST_ADDRESS));
		m_port = configParams.GetValue(ConfigTags::PORT);
		m_serviceName = StringConverter::Convert(configParams.GetValue(ConfigTags::SERVICE_NAME));
		m_debug = configParams.GetValue(ConfigTags::DEBUG);
		m_sampleCount = configParams.GetValue(ConfigTags::SAMPLE_COUNT);
	}

	const string& ConfigParams::GetHostAddress() const{
		return m_hostAddress;
	}


	int ConfigParams::GetPort() const{
		return m_port;
	}
	TransportType ConfigParams::GetTransportType() const{
		return m_transportType;
	}
	const std::string& ConfigParams::GetServiceName() const{
		return m_serviceName;
	}
	bool ConfigParams::GetDebug() const{
		return m_debug;
	}
	int ConfigParams::GetSampleCount() const{
		return m_sampleCount;
	};
}

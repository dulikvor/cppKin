#include "ConfigParams.h"
#include "GeneralParams.h"
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
		m_hostAddress = StringConverter::Convert(configParams.GetValue(tags::HOST_ADDRESS));
		m_port = configParams.GetValue(tags::PORT);
        m_transportType = configParams.GetValue(tags::TRANSPORT_TYPE);
	}
}

# pragma once

#include <string>
#include "Core/src/Export.h"
#include "TransportType.h"
#if defined(WIN32)
#pragma warning (disable : 4251)
#endif

namespace cppkin
{
	class GeneralParams;

	class A_EXPORT ConfigParams
	{
	public:
		static ConfigParams& Instance();
		~ConfigParams();
		void Load(const GeneralParams& configParams);
		//Accessors
		const std::string& GetHostAddress() const;
		int GetPort() const;
		TransportType GetTransportType() const;
		const std::string& GetServiceName() const;
		bool GetDebug() const;
		int GetSampleCount() const;

	private:
		ConfigParams();

	private:
		std::string m_hostAddress;
		std::string m_serviceName;
		int m_port;
		TransportType m_transportType;
		bool m_debug;
		int m_sampleCount;
	};
}

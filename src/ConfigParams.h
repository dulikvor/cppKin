# pragma once

#include <string>
#include "Core/src/Export.h"
#include "TransportType.h"
#include "EncodingTypes.h"
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
        ~ConfigParams() {}
        void Load(const GeneralParams& configParams);
        //Accessors
        const std::string& GetHostAddress() const { return m_hostAddress; }
        int GetPort() const { return m_port; }
        TransportType GetTransportType() const { return m_transportType; }
        const std::string& GetServiceName() const { return m_serviceName;}
        bool GetDebug() const { return m_debug; }
        int GetSampleCount() const { return m_sampleCount; }
        EncodingType GetEncodingType() const { return m_encodingType; }

    private:
        ConfigParams();

    private:
        std::string m_hostAddress;
        std::string m_serviceName;
        int m_port;
        TransportType m_transportType;
        bool m_debug;
        int m_sampleCount;
        EncodingType m_encodingType;
    };
}

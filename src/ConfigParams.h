# pragma once

#include <string>

class GeneralParams;

class ConfigParams
{
public:
	static ConfigParams& Instance();
	~ConfigParams(){}
	void Load(const GeneralParams& configParams);
	//Accessors
	const std::string& GetHostAddress() const{
		return m_hostAddress;
	}
	int GetPort() const{
		return m_port;
	}

private:
	ConfigParams(){}

private:
	std::string m_hostAddress;
	int m_port;
};

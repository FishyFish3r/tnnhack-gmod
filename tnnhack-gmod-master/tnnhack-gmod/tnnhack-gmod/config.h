#pragma once

#include <deque>
#include <string>

class CConfigSystem
{
private:
	std::deque<std::string> m_Configs;
	std::string m_CurConfig;
public:
	CConfigSystem();
	
	void Init();

	bool CollectConfigs();
	bool LoadConfig(std::string a_Name);
	bool SaveConfig(std::string a_Name) const;
	bool DeleteConfig(std::string a_Name);

	bool SaveCurrentConfig();
	bool DeleteCurrentConfig();

	std::wstring GetConfigDir() const;
	std::wstring GetAppData() const;

	std::string GetCurrentConfig() const;
};

extern CConfigSystem* g_pConfigSystem;
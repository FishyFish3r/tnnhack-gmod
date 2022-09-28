#include <windows.h>
#include <filesystem>
#include <fstream>
#include "config.h"
#include "console.h"
#include "var.h"
#include "sperma_cast.h"

#define DEFAULT_CFG_FILE "default.tnncfg"

CConfigSystem* g_pConfigSystem = nullptr;

/*

<variable_name> <variable_value>

bool:
aim_enable 1

int:
name_steal_tick 20000

color:
chams_color_visible 255 255 255 255

*/

void LoadConfigCommandHandler(std::deque<std::string> args)
{
	if (args.size() < 2)
	{
		g_pConsole->Print(args.at(0) + " <config_name>");
		return;
	}

	g_pConfigSystem->LoadConfig(args.at(1));
}

void SaveConfigCommandHandler(std::deque<std::string> args)
{
	if (args.size() < 2)
	{
		g_pConfigSystem->SaveCurrentConfig();
	}
	else
	{
		g_pConfigSystem->SaveConfig(args.at(1));
	}
}

CConfigSystem::CConfigSystem()
{
	this->m_Configs = {};
	this->m_CurConfig = DEFAULT_CFG_FILE;
}

void CConfigSystem::Init()
{
	this->m_CurConfig = DEFAULT_CFG_FILE;

	if (!this->LoadConfig(this->m_CurConfig))
	{
		this->SaveCurrentConfig();
	}

	g_pConsole->RegisterCommand("load_config", &LoadConfigCommandHandler);
	g_pConsole->RegisterCommand("save_config", &SaveConfigCommandHandler);
}

bool CConfigSystem::CollectConfigs()
{
	this->m_Configs.clear();

	static std::wstring dirPath = this->GetConfigDir();
	static std::wstring appdata = this->GetAppData();

	if ((CreateDirectoryW(appdata.c_str(), NULL) || GetLastError() == ERROR_ALREADY_EXISTS) && (CreateDirectoryW(dirPath.c_str(), NULL) || GetLastError() == ERROR_ALREADY_EXISTS))
	{
		for (auto& entry : std::filesystem::directory_iterator(dirPath))
		{
			std::string filename = entry.path().filename().string();
			this->m_Configs.push_back(filename);
		}
	}
	else
	{
		g_pConsole->Print("Failed to collect configs: No appdata directory.");
		return false;
	}

	return true;
}

bool CConfigSystem::LoadConfig(std::string a_Name)
{
	if (a_Name.empty())
	{
		return false;
	}

	static std::wstring dirPath = this->GetConfigDir();
	static std::wstring appdata = this->GetAppData();

	if ((CreateDirectoryW(appdata.c_str(), NULL) || GetLastError() == ERROR_ALREADY_EXISTS) && (CreateDirectoryW(dirPath.c_str(), NULL) || GetLastError() == ERROR_ALREADY_EXISTS))
	{
		std::wstring configPath = dirPath + L"\\" + std::wstring(a_Name.begin(), a_Name.end());
		std::ifstream inputFile(configPath);
		if (inputFile && inputFile.is_open())
		{
			std::string varName;

			while (inputFile >> varName)
			{
				CHackVarBase* var = g_pSettings->GetVar(varName);
				if (!var)
				{
					g_pConsole->PrintF("LoadConfig: Varaible %s does not exist!", varName.c_str());
					continue;
				}

				switch (var->m_Type)
				{
					case(CHackVarType_BOOL):
					{
						CHackVarBool* varBool = sperma_cast<CHackVarBool*>(var);

						std::string varValue;
						inputFile >> varValue;

						bool realValue = false;
						bool found = false;

						if (varValue == "true" || varValue == "1" || varValue == "t")
						{
							realValue = true;
							found = true;
						}
						else if (varValue == "false" || varValue == "0" || varValue == "f")
						{
							realValue = false;
							found = true;
						}

						if (found)
						{
							varBool->m_Value = realValue;
						}
						else
						{
							g_pConsole->PrintF("Invalid argument of variable %s in config %s!", var->m_Name.c_str(), a_Name.c_str());
						}

						break;
					}
					case(CHackVarType_INT):
					{
						CHackVarInt* varInt = sperma_cast<CHackVarInt*>(var);

						std::string varValue;
						inputFile >> varValue;

						int realValue = 0;
						bool found = false;

						try
						{
							realValue = std::stoi(varValue);
							found = true;
						}
						catch (std::invalid_argument e)
						{
							found = false;
						}

						if (found)
						{
							varInt->m_Value = realValue;
						}
						else
						{
							g_pConsole->PrintF("Invalid argument of variable %s in config %s!", var->m_Name.c_str(), a_Name.c_str());
						}

						break;
					}
					case(CHackVarType_FLOAT):
					{
						CHackVarFloat* varFloat = sperma_cast<CHackVarFloat*>(var);

						std::string varValue;
						inputFile >> varValue;

						float realValue = 0.f;
						bool found = false;

						try
						{
							realValue = std::stof(varValue);
							found = true;
						}
						catch (std::invalid_argument e)
						{
							found = false;
						}

						if (found)
						{
							varFloat->m_Value = realValue;
						}
						else
						{
							g_pConsole->PrintF("Invalid argument of variable %s in config %s!", var->m_Name.c_str(), a_Name.c_str());
						}

						break;
					}
					case(CHackVarType_STRING):
					{
						CHackVarString* varString = sperma_cast<CHackVarString*>(var);

						std::string varValue;
						inputFile >> varValue;

						std::string realValue;

						if (varValue == "\"")
						{
							inputFile >> varValue;

							while (varValue != "\"")
							{
								realValue += varValue;
								inputFile >> varValue;
							}
						}

						varString->m_Value = realValue;

						break;
					}
					case(CHackVarType_COLOR):
					{
						CHackVarColor* varColor = sperma_cast<CHackVarColor*>(var);

						int varRed, varGreen, varBlue, varAlpha;
						inputFile >> varRed >> varGreen >> varBlue >> varAlpha;

						varColor->m_Red = varRed;
						varColor->m_Green = varGreen;
						varColor->m_Blue = varBlue;
						varColor->m_Alpha = varAlpha;

						break;
					}
				}
			}

			this->m_CurConfig = a_Name;
			inputFile.close();
		}
		else
		{
			g_pConsole->PrintF("Failed to load config %s: No file found.", a_Name.c_str());
			return false;
		}
	}
	else
	{
		g_pConsole->PrintF("Failed to load config %s: No appdata directory.", a_Name.c_str());
		return false;
	}

	return true;
}

bool CConfigSystem::SaveConfig(std::string a_Name) const
{
	if (a_Name.empty())
	{
		return false;
	}

	static std::wstring dirPath = this->GetConfigDir();
	static std::wstring appdata = this->GetAppData();

	if ((CreateDirectoryW(appdata.c_str(), NULL) || GetLastError() == ERROR_ALREADY_EXISTS) && (CreateDirectoryW(dirPath.c_str(), NULL) || GetLastError() == ERROR_ALREADY_EXISTS))
	{
		std::wstring configPath = dirPath + L"\\" + std::wstring(a_Name.begin(), a_Name.end());
		std::ofstream outputFile(configPath, std::ios::trunc);
		if (outputFile && outputFile.is_open())
		{
			for (CHackVarBase* var : g_pSettings->GetAllVars())
			{
				outputFile << var->m_Name << " ";

				switch (var->m_Type)
				{
					case(CHackVarType_BOOL):
					{
						CHackVarBool* varBool = sperma_cast<CHackVarBool*>(var);

						if (varBool->m_Value)
						{
							outputFile << "t\n";
						}
						else
						{
							outputFile << "f\n";
						}

						break;
					}
					case(CHackVarType_INT):
					{
						CHackVarInt* varInt = sperma_cast<CHackVarInt*>(var);

						outputFile << varInt->m_Value << "\n";

						break;
					}
					case(CHackVarType_FLOAT):
					{
						CHackVarFloat* varFloat = sperma_cast<CHackVarFloat*>(var);

						outputFile << varFloat->m_Value << "\n";

						break;
					}
					case(CHackVarType_STRING):
					{
						CHackVarString* varString = sperma_cast<CHackVarString*>(var);

						outputFile << "\" " << varString->m_Value << " \"\n";

						break;
					}
					case(CHackVarType_COLOR):
					{
						CHackVarColor* varColor = sperma_cast<CHackVarColor*>(var);

						outputFile << varColor->m_Red << " " << varColor->m_Green << " " << varColor->m_Blue << " " << varColor->m_Alpha << "\n";

						break;
					}
				}
			}

			outputFile.close();
		}
		else
		{
			g_pConsole->PrintF("Failed to save config %s: No file found.", a_Name.c_str());
			return false;
		}
	}
	else
	{
		g_pConsole->PrintF("Failed to save config %s: No appdata directory.", a_Name.c_str());
		return false;
	}

	return true;
}

bool CConfigSystem::DeleteConfig(std::string a_Name)
{
	if (a_Name.empty())
	{
		return false;
	}

	static std::wstring dirPath = this->GetConfigDir();
	static std::wstring appdata = this->GetAppData();

	std::wstring configPath = dirPath + L"\\" + std::wstring(a_Name.begin(), a_Name.end());
	if (!remove(std::string(configPath.begin(), configPath.end()).c_str()))
	{
		this->m_CurConfig = DEFAULT_CFG_FILE;
		return true;
	}

	return false;
}

bool CConfigSystem::SaveCurrentConfig()
{
	return this->SaveConfig(this->m_CurConfig);
}

bool CConfigSystem::DeleteCurrentConfig()
{
	return this->DeleteConfig(this->m_CurConfig);
}

std::wstring CConfigSystem::GetConfigDir() const
{
	static std::wstring appdata = this->GetAppData();
	static std::wstring configDirPath = appdata + L"\\configs";

	return configDirPath;
}

std::wstring CConfigSystem::GetAppData() const
{
	static std::wstring appdata = _wgetenv(L"appdata") + std::wstring(L"\\tnnhack-gmod");
	return appdata;
}

std::string CConfigSystem::GetCurrentConfig() const
{
	return this->m_CurConfig;
}
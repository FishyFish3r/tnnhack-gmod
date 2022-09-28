#include <stdexcept>
#include "var.h"
#include "sperma_cast.h"

#define HACK_VAR(varType, varName, initialValue) varType* varName = new varType(#varName, initialValue); g_pConsole->RegisterCommand(#varName, &SettingCommandHandler); this->m_Vars.push_back(varName)

CHackVars* g_pSettings = nullptr;

std::string HackVarTypeToString(CHackVarType type)
{
	switch (type)
	{
		case(CHackVarType_BOOL):
		{
			return "boolean";
		}
		case(CHackVarType_INT):
		{
			return "integer";
		}
		case(CHackVarType_FLOAT):
		{
			return "float";
		}
		case(CHackVarType_STRING):
		{
			return "string";
		}
		case(CHackVarType_COLOR):
		{
			return "color";
		}
	}

	return "unknown";
}

void SettingCommandHandler(std::deque<std::string> args)
{
	if (args.size() < 2)
	{
		g_pConsole->Print("Not enough arguments. <varname> <value>.");
		return;
	}

	std::string settingName = args.at(0);
	CHackVarBase* var = g_pSettings->GetVar(settingName);
	if (!var) // should never happen?
	{
		g_pConsole->PrintF("Variable %s not found.", settingName.c_str());
		return;
	}

	switch (var->m_Type)
	{
		case(CHackVarType_BOOL):
		{
			CHackVarBool* varBool = sperma_cast<CHackVarBool*>(var);

			std::string value = args.at(1);
			bool realValue = false;
			bool found = false;

			if (value == "true" || value == "1" || value == "t")
			{
				realValue = true;
				found = true;
			}
			else if (value == "false" || value == "0" || value == "f")
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
				g_pConsole->Print("Invalid argument. Try: (true, 1, t), or (false, 0, f).");
			}

			break;
		}
		case(CHackVarType_INT):
		{
			CHackVarInt* varInt = sperma_cast<CHackVarInt*>(var);

			std::string value = args.at(1);
			int realValue = 0;
			bool found = false;

			try
			{
				realValue = std::stoi(value);
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
				g_pConsole->Print("Invalid argument. Integer expected.");
			}

			break;
		}
		case(CHackVarType_FLOAT):
		{
			CHackVarFloat* varFloat = sperma_cast<CHackVarFloat*>(var);

			std::string value = args.at(1);
			float realValue = 0.f;
			bool found = false;

			try
			{
				realValue = std::stof(value);
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
				g_pConsole->Print("Invalid argument. Float expected.");
			}

			break;
		}
		case(CHackVarType_STRING):
		{
			CHackVarString* varString = sperma_cast<CHackVarString*>(var);

			std::string value = "";

			for (unsigned i = 1; i < args.size(); i++)
			{
				value += args.at(1) + (i == args.size() - 1 ? "" : " ");
			}

			varString->m_Value = value;

			break;
		}
		case(CHackVarType_COLOR):
		{
			CHackVarColor* varColor = sperma_cast<CHackVarColor*>(var);

			if (args.size() != 4 && args.size() != 5)
			{
				g_pConsole->Print("Invalid argument. Try: (INT:Red, INT:Green, INT:Blue), or (INT:Red, INT:Green, INT:Blue, INT:Alpha)");
				break;
			}

			if (args.size() == 4)
			{
				int r, g, b;
				bool found = false;

				try
				{
					r = std::stoi(args.at(1));
					g = std::stoi(args.at(2));
					b = std::stoi(args.at(3));

					found = true;
				}
				catch (std::invalid_argument e)
				{
					found = false;
				}

				if (found)
				{
					varColor->m_Red = r;
					varColor->m_Green = g;
					varColor->m_Blue = b;
					varColor->m_Alpha = 255;
				}
				else
				{
					g_pConsole->Print("Invalid argument. 3 integers (Red, Green, Blue) expected.");
				}
			}
			else
			{
				int r, g, b, a;
				bool found = false;

				try
				{
					r = std::stoi(args.at(1));
					g = std::stoi(args.at(2));
					b = std::stoi(args.at(3));
					a = std::stoi(args.at(4));

					found = true;
				}
				catch (std::invalid_argument e)
				{
					found = false;
				}

				if (found)
				{
					varColor->m_Red = r;
					varColor->m_Green = g;
					varColor->m_Blue = b;
					varColor->m_Alpha = a;
				}
				else
				{
					g_pConsole->Print("Invalid argument. 4 integers (Red, Green, Blue, Alpha) expected.");
				}
			}

			break;
		}
	}
}

void PrintAllVarsCommandHandler(std::deque<std::string> args)
{
	std::deque<CHackVarBase*> vars = g_pSettings->GetAllVars();
	for (CHackVarBase* c : vars)
	{
		g_pConsole->PrintF("%s: %s", c->m_Name.c_str(), HackVarTypeToString(c->m_Type).c_str());
	}
}

CHackVarBase::CHackVarBase()
{
	this->m_Name = "";
	this->m_Type = CHackVarType___none;
}

CHackVarBase::CHackVarBase(std::string a_Name, CHackVarType a_Type)
{
	this->m_Name = a_Name;
	this->m_Type = a_Type;
}

CHackVarBool::CHackVarBool(std::string a_Name, bool a_InitialValue)
{
	this->m_Name = a_Name;
	this->m_Type = CHackVarType_BOOL;
	this->m_Value = a_InitialValue;
}

CHackVarInt::CHackVarInt(std::string a_Name, int a_InitialValue)
{
	this->m_Name = a_Name;
	this->m_Type = CHackVarType_INT;
	this->m_Value = a_InitialValue;
}

CHackVarFloat::CHackVarFloat(std::string a_Name, float a_InitialValue)
{
	this->m_Name = a_Name;
	this->m_Type = CHackVarType_FLOAT;
	this->m_Value = a_InitialValue;
}

CHackVarString::CHackVarString(std::string a_Name, std::string a_InitialValue)
{
	this->m_Name = a_Name;
	this->m_Type = CHackVarType_STRING;
	this->m_Value = a_InitialValue;
}

CHackVarColor::CHackVarColor(std::string a_Name, int a_InitialRed, int a_InitialGreen, int a_InitialBlue)
{
	this->m_Name = a_Name;
	this->m_Type = CHackVarType_COLOR;

	this->m_Red = a_InitialRed;
	this->m_Green = a_InitialGreen;
	this->m_Blue = a_InitialBlue;
	this->m_Alpha = 255;
}

CHackVarColor::CHackVarColor(std::string a_Name, int a_InitialRed, int a_InitialGreen, int a_InitialBlue, int a_InitialAlpha)
{
	this->m_Name = a_Name;
	this->m_Type = CHackVarType_COLOR;

	this->m_Red = a_InitialRed;
	this->m_Green = a_InitialGreen;
	this->m_Blue = a_InitialBlue;
	this->m_Alpha = a_InitialAlpha;
}

CHackVars::CHackVars()
{
	this->m_Vars = {};

	g_pConsole->RegisterCommand("allvars", &PrintAllVarsCommandHandler);

	HACK_VAR(CHackVarBool, aim_enable, true);
	HACK_VAR(CHackVarBool, aim_silent, true);
	HACK_VAR(CHackVarBool, aim_fov, true);
	HACK_VAR(CHackVarFloat, aim_fov_radius, 5.f);
	
	HACK_VAR(CHackVarBool, aa_enable, false);
	HACK_VAR(CHackVarInt, aa_type, 1);
	HACK_VAR(CHackVarBool, aa_angle_circle, true);
	HACK_VAR(CHackVarFloat, aa_angle_circle_radius, 60.f);
	HACK_VAR(CHackVarBool, aa_angle_circle_caption, true);

	HACK_VAR(CHackVarBool, esp_enable, true);
	HACK_VAR(CHackVarBool, esp_box, true);
	HACK_VAR(CHackVarBool, esp_line, true);
	HACK_VAR(CHackVarBool, esp_heading, true);
	HACK_VAR(CHackVarBool, esp_info, true);
	HACK_VAR(CHackVarBool, esp_skeleton, true);

	CHackVarColor* esp_line_color = new CHackVarColor("esp_line_color", 255, 255, 255, 255);
	g_pConsole->RegisterCommand("esp_line_color", &SettingCommandHandler);
	this->m_Vars.push_back(esp_line_color);

	CHackVarColor* esp_skeleton_color = new CHackVarColor("esp_skeleton_color", 255, 255, 255, 255);
	g_pConsole->RegisterCommand("esp_skeleton_color", &SettingCommandHandler);
	this->m_Vars.push_back(esp_skeleton_color);

	CHackVarColor* esp_box_color = new CHackVarColor("esp_box_color", 255, 255, 255, 255);
	g_pConsole->RegisterCommand("esp_box_color", &SettingCommandHandler);
	this->m_Vars.push_back(esp_box_color);

	HACK_VAR(CHackVarBool, custom_hand_fov, true);
	HACK_VAR(CHackVarFloat, custom_hand_fov_value, 130.f);

	HACK_VAR(CHackVarBool, should_draw_fog, true);
	
	HACK_VAR(CHackVarBool, autostrafe_enable, true);
	HACK_VAR(CHackVarBool, bunnyhop_enable, true);

	HACK_VAR(CHackVarBool, chams_enable, true);
	HACK_VAR(CHackVarBool, chams_walls, true);
	HACK_VAR(CHackVarBool, chams_colored, true);

	CHackVarColor* chams_color_visible = new CHackVarColor("chams_color_visible", 0, 255, 0, 255);
	g_pConsole->RegisterCommand("chams_color_visible", &SettingCommandHandler);
	this->m_Vars.push_back(chams_color_visible);

	CHackVarColor* chams_color_invisible = new CHackVarColor("chams_color_invisible", 255, 0, 0, 255);
	g_pConsole->RegisterCommand("chams_color_invisible", &SettingCommandHandler);
	this->m_Vars.push_back(chams_color_invisible);

	HACK_VAR(CHackVarBool, force_thirdperson, false);
	HACK_VAR(CHackVarBool, bypass_svcheats, false);
	HACK_VAR(CHackVarInt, friend_list_id, 1337);
	HACK_VAR(CHackVarBool, menu_var_list, true);
	HACK_VAR(CHackVarBool, name_steal, false);
	HACK_VAR(CHackVarInt, name_steal_tick, 20000);
	HACK_VAR(CHackVarBool, disable_all, false);
}

CHackVars::~CHackVars()
{
	for (CHackVarBase* v : this->m_Vars)
	{
		delete v;
	}

	this->m_Vars.clear();
}

CHackVarBase* CHackVars::GetVar(std::string a_Name) const
{
	for (CHackVarBase* v : this->m_Vars)
	{
		if (v->m_Name == a_Name)
		{
			return v;
		}
	}

	return nullptr;
}

std::deque<CHackVarBase*> CHackVars::GetAllVars() const
{
	return this->m_Vars;
}
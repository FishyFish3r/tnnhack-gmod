#pragma once

#include "console.h"

void SettingCommandHandler(std::deque<std::string> args);
void PrintAllVarsCommandHandler(std::deque<std::string> args);

enum CHackVarType
{
	CHackVarType___none,
	CHackVarType_BOOL,
	CHackVarType_INT,
	CHackVarType_FLOAT,
	CHackVarType_STRING,
	CHackVarType_COLOR
};

std::string HackVarTypeToString(CHackVarType type);

class CHackVarBase
{
public:
	std::string		m_Name;
	CHackVarType	m_Type;
public:
	CHackVarBase();
	CHackVarBase(std::string a_Name, CHackVarType a_Type);
};

class CHackVarBool : public CHackVarBase
{
public:
	bool m_Value;
public:
	CHackVarBool(std::string a_Name, bool a_InitialValue);
};

class CHackVarInt : public CHackVarBase
{
public:
	int m_Value;
public:
	CHackVarInt(std::string a_Name, int a_InitialValue);
};

class CHackVarFloat : public CHackVarBase
{
public:
	float m_Value;
public:
	CHackVarFloat(std::string a_Name, float a_InitialValue);
};

class CHackVarString : public CHackVarBase
{
public:
	std::string m_Value;
public:
	CHackVarString(std::string a_Name, std::string a_InitialValue);
};

class CHackVarColor : public CHackVarBase
{
public:
	int m_Red, m_Green, m_Blue, m_Alpha;
public:
	CHackVarColor(std::string a_Name, int a_InitialRed, int a_InitialGreen, int a_InitialBlue);
	CHackVarColor(std::string a_Name, int a_InitialRed, int a_InitialGreen, int a_InitialBlue, int a_InitialAlpha);
};

class CHackVars
{
private:
	std::deque<CHackVarBase*> m_Vars;
public:
	CHackVars();
	~CHackVars();

	CHackVarBase*				GetVar(std::string a_Name) const;
	std::deque<CHackVarBase*>	GetAllVars() const;
};

extern CHackVars* g_pSettings;


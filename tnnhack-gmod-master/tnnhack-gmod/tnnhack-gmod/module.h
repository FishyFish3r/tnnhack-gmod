#pragma once

#include <windows.h>
#include <string>

class CModule
{
private:
	DWORD			m_Start, m_Size, m_End;
	std::string		m_Name;
public:
	CModule();
	CModule(std::string a_Name);

	DWORD			GetStart() const;
	DWORD			GetSize() const;
	DWORD			GetEnd() const;

	std::string		GetName() const;
};
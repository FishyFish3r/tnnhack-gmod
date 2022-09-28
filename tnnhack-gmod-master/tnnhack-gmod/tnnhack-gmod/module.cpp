#include "module.h"
#include "sperma_cast.h"
CModule::CModule()
{
	this->m_Start = 0;
	this->m_Size = 0;
	this->m_End = 0;
	this->m_Name = "UNDEF";
}

CModule::CModule(std::string a_Name)
{
	this->m_Start = sperma_cast<DWORD>(GetModuleHandleA(a_Name.c_str()));
	this->m_Size = PIMAGE_NT_HEADERS(this->m_Start + PIMAGE_DOS_HEADER(this->m_Start)->e_lfanew)->OptionalHeader.SizeOfImage;
	this->m_End = this->m_Start + this->m_Size - 1;
	this->m_Name = a_Name;
}

DWORD CModule::GetStart() const
{
	return this->m_Start;
}

DWORD CModule::GetSize() const
{
	return this->m_Size;
}

DWORD CModule::GetEnd() const
{
	return this->m_End;
}

std::string CModule::GetName() const
{
	return this->m_Name;
}
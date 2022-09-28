#include <stdexcept>
#include "VMTHooks.h"
#include "module.h"

CVMTHooks* g_pVmtHook = nullptr;

CVMTData::CVMTData()
{
	this->m_pFrame = nullptr;
	this->m_pVmtPtr = nullptr;
	this->m_index = 0;

	this->m_pOriginalFuncAddy = 0;
	this->m_Size = 0;
}

CVMTData::CVMTData(void* a_Frame, unsigned a_Index, unsigned a_Size)
{
	this->m_pFrame = a_Frame;
	this->m_index = a_Index;
	this->m_Size = a_Size;

	this->m_pVmtPtr = *zalypa_cast<void***>(this->m_pFrame);
	this->m_pOriginalFuncAddy = sperma_cast<DWORD>(this->m_pVmtPtr[this->m_index]);
}

CVMTHooks::CVMTHooks()
{
	this->m_AddyList = {};
}

CVMTHooks::~CVMTHooks()
{
	this->VMTUnhookAll();
}

void CVMTHooks::VMTHook(std::string a_HookName, CVMTData* a_Data, void* a_hFuncAddy)
{
	CVMTData* d = nullptr;

	try
	{
		d = this->m_AddyList.at(a_HookName);
	}
	catch (std::out_of_range e)
	{
		d = nullptr;
	}

	if (d)
	{
		return;
	}

	DWORD oldProtect = 0;
	VirtualProtect(&a_Data->m_pVmtPtr[a_Data->m_index], a_Data->m_Size, PAGE_EXECUTE_READWRITE, &oldProtect);

	a_Data->m_pVmtPtr[a_Data->m_index] = a_hFuncAddy;

	VirtualProtect(&a_Data->m_pVmtPtr[a_Data->m_index], a_Data->m_Size, oldProtect, &oldProtect);
	this->m_AddyList.insert(std::make_pair(a_HookName, a_Data));
}

bool CVMTHooks::VMTUnhook(std::string a_HookName)
{
	CVMTData* d = nullptr;

	try
	{
		d = this->m_AddyList.at(a_HookName);
	}
	catch (std::out_of_range e)
	{
		d = nullptr;
	}

	if (!d)
	{
		return false;
	}

	DWORD oldProtect = 0;
	VirtualProtect(&d->m_pVmtPtr[d->m_index], d->m_Size, PAGE_EXECUTE_READWRITE, &oldProtect);

	d->m_pVmtPtr[d->m_index] = sperma_cast<void*>(d->m_pOriginalFuncAddy);

	VirtualProtect(&d->m_pVmtPtr[d->m_index], d->m_Size, oldProtect, &oldProtect);
	return true;
}

void CVMTHooks::VMTUnhookAll()
{
	for (std::pair<std::string, CVMTData*> a : this->m_AddyList)
	{
		this->VMTUnhook(a.first);
		delete a.second;
	}

	this->m_AddyList.clear();
}

DWORD CVMTHooks::GetOriginalFunc(std::string a_Name) const
{
	CVMTData* d = nullptr;

	try
	{
		d = this->m_AddyList.at(a_Name);
	}
	catch (std::out_of_range e)
	{
		d = nullptr;
	}

	if (!d)
	{
		return false;
	}

	if (!d)
	{
		return 0;
	}

	return d->m_pOriginalFuncAddy;
}
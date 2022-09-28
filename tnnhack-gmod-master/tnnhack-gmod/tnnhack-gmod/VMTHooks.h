#pragma once

#include <map>
#include <Windows.h>
#include <string>
#include "sperma_cast.h"

class CVMTData
{
public:		
	DWORD				m_pOriginalFuncAddy;
	void*				m_pFrame;
	void**				m_pVmtPtr;
	unsigned			m_Size;
	unsigned			m_index;
public:
	CVMTData();
	CVMTData(void* a_Frame, unsigned a_Index, unsigned a_Size);
};

class CVMTHooks
{
private:
	std::map<std::string, CVMTData*> m_AddyList;
public:
	CVMTHooks();
	~CVMTHooks();

	void VMTHook(std::string a_Name, CVMTData* a_Data, void* a_hFuncAddy);
	bool VMTUnhook(std::string a_HookName);
	void VMTUnhookAll();

	DWORD GetOriginalFunc(std::string a_Name) const;
};

extern CVMTHooks* g_pVmtHook;
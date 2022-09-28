#pragma once

#include <windows.h>
#include <string>
#include <map>

class CDetourData
{
public:
	DWORD			m_HookAddress;
	DWORD			m_OriginalFuncAddress;
	DWORD			m_JumpAddress;
	unsigned		m_ByteCount;
	byte*			m_pOriginalBytes;
	std::string		m_Name;
public:
	CDetourData();
	CDetourData(DWORD a_HookAddress, DWORD a_OriginalFuncAddress, DWORD a_JumpAddress, unsigned a_ByteCount, std::string a_Name);
	~CDetourData();
};

class CDetours
{
private:
	std::map<std::string, CDetourData*> m_DetourList;
private:
	void	Patch(byte* dst, byte* src, unsigned size);
	bool	Detour(byte* src, byte* dst, unsigned len);
	byte*	TrampHook(byte* src, byte* dst, unsigned len);
	bool	Unhook(CDetourData* a_pData);
public:
	CDetours();
	~CDetours();

	DWORD	GetRelocateAddress(std::string a_Name) const; // return jmp relocation address (for original calls inside hooks)

	bool	Hook(std::string a_Name, DWORD a_Hook, DWORD a_Original, unsigned a_ByteCount);
	bool	Unhook(std::string a_Name);

	void	ClearAllHooks();
};

extern CDetours* g_pDetours;
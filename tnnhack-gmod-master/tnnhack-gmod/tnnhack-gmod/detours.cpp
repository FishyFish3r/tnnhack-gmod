#include "detours.h"
#include "sperma_cast.h"

CDetours* g_pDetours = nullptr;

CDetourData::CDetourData()
{
    this->m_ByteCount = 0;
    this->m_HookAddress = 0;
    this->m_JumpAddress = 0;
    this->m_Name = "UNDEF";
    this->m_pOriginalBytes = nullptr;
    this->m_OriginalFuncAddress = 0;
}

CDetourData::CDetourData(DWORD a_HookAddress, DWORD a_OriginalFuncAddress, DWORD a_JumpAddress, unsigned a_ByteCount, std::string a_Name)
{
    this->m_ByteCount = a_ByteCount;
    this->m_HookAddress = a_HookAddress;
    this->m_JumpAddress = a_JumpAddress;
    this->m_Name = a_Name;
    this->m_pOriginalBytes = new byte[a_ByteCount];
    this->m_OriginalFuncAddress = a_OriginalFuncAddress;

    memcpy(this->m_pOriginalBytes, &this->m_OriginalFuncAddress, this->m_ByteCount);
}

CDetourData::~CDetourData()
{
    delete[] this->m_pOriginalBytes;
}

CDetours::CDetours()
{
	this->m_DetourList = {};
}

CDetours::~CDetours()
{
	this->ClearAllHooks();
}

void CDetours::Patch(byte* dst, byte* src, unsigned size)
{
	DWORD oldProtect;

	VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldProtect);
	memcpy(dst, src, size);
	VirtualProtect(dst, size, oldProtect, &oldProtect);
}

bool CDetours::Detour(byte* src, byte* dst, unsigned len)
{
    if (len < 5)
    {
        return false;
    }

    DWORD oldProtect;
    VirtualProtect(src, len, PAGE_EXECUTE_READWRITE, &oldProtect);

    memset(src, 0x90, len);

    DWORD relAddy = zalypa_cast<DWORD>(dst - src - 5);
    *src = zalypa_cast<char>(0xE9);
    *sperma_cast<DWORD*>(src + 1) = zalypa_cast<DWORD>(relAddy);

    VirtualProtect(src, len, oldProtect, &oldProtect);

    return true;
}

byte* CDetours::TrampHook(byte* src, byte* dst, unsigned len)
{
    if (len < 5)
    {
        return nullptr;
    }

    byte* gateway = zalypa_cast<byte*>(VirtualAlloc(0, len + 5, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE));
    memcpy(gateway, src, len);

    DWORD jumpAddy = zalypa_cast<DWORD>(src - gateway - 5);
    *(gateway + len) = zalypa_cast<char>(0xE9);

    *sperma_cast<DWORD*>(gateway + len + 1) = jumpAddy;
    if (this->Detour(src, dst, len))
    {
        return gateway;
    }

    return nullptr;
}

bool CDetours::Unhook(CDetourData* a_pData)
{
    if (!a_pData)
    {
        return false;
    }

    this->Patch(sperma_cast<byte*>(a_pData->m_OriginalFuncAddress), a_pData->m_pOriginalBytes, a_pData->m_ByteCount);
    return true;
}

bool CDetours::Hook(std::string a_Name, DWORD a_Hook, DWORD a_Original, unsigned a_ByteCount)
{
    CDetourData* d = new CDetourData(a_Hook, a_Original, 0, a_ByteCount, a_Name);

    byte* originalJumpAddress = this->TrampHook(sperma_cast<byte*>(a_Original), sperma_cast<byte*>(a_Hook), a_ByteCount);
    d->m_JumpAddress = sperma_cast<DWORD>(originalJumpAddress);

    if (!originalJumpAddress)
    {
        delete d;
        return false;
    }

    this->m_DetourList.insert(std::make_pair(a_Name, d));
    return true;
}

bool CDetours::Unhook(std::string a_Name)
{
    CDetourData* d = this->m_DetourList.at(a_Name);
    if (!d)
    {
        return false;
    }
    else
    {
        this->Unhook(d);
        delete d;
        return true;
    }

    return false;
}

void CDetours::ClearAllHooks()
{
    for (std::pair<std::string, CDetourData*> p : this->m_DetourList)
    {
        this->Unhook(p.second);
        delete p.second;
    }

    this->m_DetourList.clear();
}

DWORD CDetours::GetRelocateAddress(std::string a_Name) const
{
    CDetourData* d = this->m_DetourList.at(a_Name);
    if (!d)
    {
        return NULL;
    }
    else
    {
        return d->m_JumpAddress;
    }
 
    return NULL;
}
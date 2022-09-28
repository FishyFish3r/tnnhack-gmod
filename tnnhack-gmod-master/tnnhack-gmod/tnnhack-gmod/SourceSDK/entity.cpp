#include <windows.h>
#include "entity.h"
#include "const.h"
#include "../interface.h"
#include "../sperma_cast.h"

int CBaseEntity::GetHealth()
{
	int* entityHp = sperma_cast<int*>(sperma_cast<DWORD>(this) + 0x90);
	return *entityHp;
}

int CBaseEntity::GetMaxHealth()
{
	int* entityHp = sperma_cast<int*>(sperma_cast<DWORD>(this) + 0x94);
	return *entityHp;
}

int CBaseEntity::GetFlags()
{
	int* entityFlags = sperma_cast<int*>(sperma_cast<DWORD>(this) + 0x350);
	return *entityFlags;
}

bool CBaseEntity::IsAlive()
{
	int curHp = this->GetHealth();
	int maxHp = this->GetMaxHealth();
	if (curHp <= 0 || curHp > maxHp)
	{
		return false;
	}

	return true;
}

Vector3f CBaseEntity::GetEyePos()
{
	Vector3f EyePos = this->GetAbsOrigin();
	if (this->GetFlags() & FL_DUCKING)
	{
		EyePos.z += 36.f;
	}
	else
	{
		EyePos.z += 72.f;
	}

	return EyePos;
}

// not working
Vector3f CBaseEntity::GetViewPunch()
{
	Vector3f* viewPunch = sperma_cast<Vector3f*>(sperma_cast<DWORD>(this) + 0x6C);
	return *viewPunch;
}

// not working
QAngle CBaseEntity::GetAngle()
{
	ILuaInterface* cl = Interfaces::Lua()->GetLuaInterface(ELuaInterfaceType_CLIENT);
	if (!cl)
		return { 0.f, 0.f, 0.f };

	cl->PushSpecial(ELuaSpecial_GLOBAL);

	// get entity by index
	cl->GetField(-1, "Entity");
	cl->PushNumber(this->GetIndex());
	cl->Call(1, 1);

	// get angle
	cl->GetField(-1, "GetAngles");
	cl->Push(-2);
	cl->Call(1, 1);

	QAngle ret = cl->GetAngle();

	cl->Pop(3);

	return ret;
}

int CBaseEntity::LookupBone(const char* name)
{
	ILuaInterface* cl = Interfaces::Lua()->GetLuaInterface(ELuaInterfaceType_CLIENT);
	if (!cl)
		return 0;

	cl->PushSpecial(ELuaSpecial_GLOBAL);

	// get entity by index
	cl->GetField(-1, "Entity");
	cl->PushNumber(this->GetIndex());
	cl->Call(1, 1);

	// get bone by name
	cl->GetField(-1, "LookupBone");
	cl->Push(-2);
	cl->PushString(name, strlen(name));
	cl->Call(2, 1);

	int ret = cl->GetNumber();

	cl->Pop(3);

	return ret;
}

CBaseEntityNetworkable* CBaseEntity::GetNetworkable()
{
	return sperma_cast<CBaseEntityNetworkable*>(sperma_cast<DWORD>(this) + 8);
}

CBaseEntityAnimating* CBaseEntity::GetAnimating()
{
	return sperma_cast<CBaseEntityAnimating*>(sperma_cast<DWORD>(this) + 4);
}

int CBaseEntity::GetIndex()
{
	CBaseEntityNetworkable* net = this->GetNetworkable();
	return net->GetIndex();
}

bool CBaseEntity::IsDormant()
{
	CBaseEntityNetworkable* net = this->GetNetworkable();
	return net->IsDormant();
}
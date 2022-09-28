#pragma once

#include "../entity.h"

class CBaseEntityUnknown;

class IClientEntityList
{
public:
	virtual CBaseEntityNetworkable* GetClientNetworkable(int entnum) = 0;
	virtual CBaseEntityNetworkable* GetClientNetworkableFromHandle(CBaseHandle handle) = 0;
	virtual CBaseEntityUnknown* GetClientUnknownFromHandle(CBaseHandle handle) = 0;
	virtual CBaseEntity* GetClientEntity(int entnum) = 0;
	virtual CBaseEntity* GetClientEntityFromHandle(CBaseHandle handle) = 0;
	virtual int NumberOfEntities(bool bIncludeNonNetworkable = false) = 0;
	virtual int GetHighestEntityIndex() = 0;
	virtual void SetMaxEntities(int maxents) = 0;
	virtual int GetMaxEntities() = 0;
};
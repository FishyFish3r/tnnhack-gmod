#include "misc.h"
#include "../interface.h"
#include "../module.h"
#include "../sperma_cast.h"
#include "../var.h"
#include "../utils.h"
#include <ctime>
#include <cstdlib>

#define SVCHEATS_STATIC_OFFSET 0x690C68

CMisc* g_pMisc = nullptr;

void CMisc::BypassSvCheats()
{
	static CHackVarBool* bypass_svcheats = sperma_cast<CHackVarBool*>(g_pSettings->GetVar("bypass_svcheats"));
	static bool bypassed = false;
	
	if (bypass_svcheats->m_Value)
	{
		if (Interfaces::Engine()->IsInGame())
		{
			if (!bypassed)
			{
				CModule* engine = g_pUtils->GetModule("engine.dll");

				if (engine)
				{
					int* svCheats = sperma_cast<int*>(engine->GetStart() + SVCHEATS_STATIC_OFFSET);
					*svCheats = 1;

					bypassed = true;
				}
			}
		}
		else
		{
			bypassed = false;
		}
	}
	else
	{
		if (bypassed)
		{
			CModule* engine = g_pUtils->GetModule("engine.dll");

			if (engine)
			{
				int* svCheats = sperma_cast<int*>(engine->GetStart() + SVCHEATS_STATIC_OFFSET);
				*svCheats = 0;

				bypassed = false;
			}
		}
	}
}

void CMisc::Thirdperson()
{
	static CHackVarBool* force_thirdperson = sperma_cast<CHackVarBool*>(g_pSettings->GetVar("force_thirdperson"));
	static bool oldValue = force_thirdperson->m_Value;

	if (force_thirdperson->m_Value != oldValue)
	{
		if (force_thirdperson->m_Value)
		{
			Interfaces::Engine()->ClientCmd("thirdperson");
		}
		else
		{
			Interfaces::Engine()->ClientCmd("firstperson");
		}

		oldValue = force_thirdperson->m_Value;
	}
}

void CMisc::NameSteal()
{
	static CHackVarInt* name_steal_tick = sperma_cast<CHackVarInt*>(g_pSettings->GetVar("name_steal_tick"));
	srand(time(0));

	int PlayerOnMap = 0;
	for (int i = 0; i < Interfaces::Engine()->GetMaxClients(); i++)
	{
		CBaseEntity* player = Interfaces::EntityList()->GetClientEntity(i);
		if (player != nullptr)
		{
			PlayerOnMap++;
		}
	}
	int id = rand() % PlayerOnMap;
	
	static const std::string command = "say /rpname ";
	std::string name;
	std::string result;
	static std::string oldName;
	CPlayerInfo info;
	static DWORD NextTick = 0;
	if (GetTickCount() >= NextTick)
	{
		if (id != Interfaces::Engine()->GetLocalPlayer())
		{
			Interfaces::Engine()->GetPlayerInfo(id, &info);
			if (info.name)
			{
				for (int i = 0; info.name[i] != 0; i++)
				{
					name += info.name[i];
					if (info.name[i + 1] == 0)
					{
						name += info.name[i];
					}
				}
				if (name != oldName && name.length() < 21 && !name.empty())
				{
					result = command + name;
					Interfaces::Engine()->ClientCmd(result.c_str());
					oldName = name;
					NextTick = GetTickCount() + name_steal_tick->m_Value;
				}
			}
		}
	}
}
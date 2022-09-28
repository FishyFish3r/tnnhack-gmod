#pragma once

#include <windows.h>
#include "SourceSDK/usercmd.h"
#include "SourceSDK/Interfaces/IPanel.h"
#include "SourceSDK/Interfaces/IMaterialSystem.h"
#include "SourceSDK/Interfaces/IGameEvent.h"

namespace Hooks
{
	void Run();
	void Shutdown();

	typedef void(__thiscall* tPaintTraverse)(void*, VPANEL, bool, bool);
	void __fastcall hPaintTraverse(void* __this, void* __reg, VPANEL panel, bool forceRepaint, bool allowForce);

	extern WNDPROC g_oWndProc;
	LRESULT CALLBACK hWndProc(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam);
	
	typedef bool(__thiscall* tCreateMove)(void*, float, CUserCMD*);
	bool __fastcall hCreateMove(void* __this, void* __reg, float sample, CUserCMD* cmd);

	typedef float(__thiscall* tGetViewmodelFOV)(void*);
	float __fastcall hGetViewmodelFOV(void* __this, void* __reg);

	typedef bool(__thiscall* tShouldDrawFog)(void*);
	bool __fastcall hShouldDrawFog(void* __this, void* __reg);

	typedef void(__thiscall* tDrawModelExecute)(void* __this, void* state, ModelRenderInfo_t& pInfo, void* pCustomBoneToWorld);
	void __fastcall hDrawModelexecute(void* __this, void* __reg, void* state, ModelRenderInfo_t& info, void* pCustomBoneToWorld);

	extern IGameEventListener2* g_pHurtEventListener;
	void HurtEventHandler(IGameEvent2* ev);

	extern IGameEventListener2* g_pDeathEventListener;
	void DeathEventHandler(IGameEvent2* ev);
}
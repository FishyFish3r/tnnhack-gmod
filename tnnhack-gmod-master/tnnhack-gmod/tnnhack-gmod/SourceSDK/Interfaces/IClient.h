#pragma once

#include "../iface.h"
#include "../netvar.h"
#include "../globals.h"

typedef int ButtonCode_t;

class IClient
{
public:
	virtual int Init(tCreateInterface appSystemFactory, tCreateInterface physicsFactory, CGlobalVarsBase* pGlobals) = 0;
	virtual void PostInit() = 0;
	virtual void Shutdown() = 0;
	virtual bool ReplayInit(tCreateInterface fnReplayFactory) = 0;
	virtual bool ReplayPostInit() = 0;
	virtual void LevelInitPreEntity(const char* pMapName) = 0;
	virtual void LevelInitPostEntity() = 0;
	virtual void LevelShutdown() = 0;
	virtual ClientClass* GetAllClasses() = 0;
	virtual int HudVidInit() = 0;
	virtual void HudProcessInput(bool bActive) = 0;
	virtual void HudUpdate(bool bActive) = 0;
	virtual void HudReset() = 0;
	virtual void HudText(const char* message) = 0;
	virtual void IN_ActivateMouse() = 0;
	virtual void IN_DeactivateMouse() = 0;
	virtual void IN_Accumulate() = 0;
	virtual void IN_ClearStates() = 0;
	virtual bool IN_IsKeyDown(const char* name, bool& isdown) = 0;
	virtual void IN_OnMouseWheeled(int nDelta) = 0;
	virtual int IN_KeyEvent(int eventcode, ButtonCode_t keynum, const char* pszCurrentBinding) = 0;
	virtual void CreateMove(int sequence_number, float input_sample_frametime, bool active) = 0;
};
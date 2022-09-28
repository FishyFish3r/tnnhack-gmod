#pragma once

#include "utils.h"
#include "SourceSDK/Interfaces/IClient.h"
#include "SourceSDK/Interfaces/IEngineClient.h"
#include "SourceSDK/Interfaces/IClientEntityList.h"
#include "SourceSDK/Interfaces/IInputSystem.h"
#include "SourceSDK/Interfaces/ISurface.h"
#include "SourceSDK/Interfaces/IPanel.h"
#include "SourceSDK/Interfaces/IMaterialSystem.h"
#include "SourceSDK/Interfaces/ILuaShared.h"
#include "SourceSDK/Interfaces/IGameEvent.h"
#include "SourceSDK/Interfaces/IEngineTraceClient.h"
#include "sperma_cast.h"

#define ITF(T, funcName, name, moduleName) inline T* funcName() { static T* r = sperma_cast<T*>(g_pUtils->GetInterface(##moduleName, ##name)); return r; }

namespace Interfaces
{
	
	ITF(IClient,				Client,			"VClient017",				"client.dll"); // client interface
	ITF(IEngineClient,			Engine,			"VEngineClient013",			"engine.dll"); // engine interface
	ITF(IClientEntityList,		EntityList,		"VClientEntityList003",		"client.dll"); // entitylist interface
	ITF(IInputSystem,			InputSystem,	"InputSystemVersion001",	"inputsystem.dll"); // inputsystem interface
	ITF(ISurface,				Surface,		"VGUI_Surface030",			"vguimatsurface.dll"); // surface interface for drawing
	ITF(IPanel,					Panel,			"VGUI_Panel009",			"vgui2.dll"); // panel interface
	ITF(IMaterialSystem,		MaterialSystem,	"VMaterialSystem080",       "materialsystem.dll"); // material system interface
	ITF(IModelInfoClient,		ModelInfo,		"VModelInfoClient006",		"engine.dll"); // model info interface
	ITF(IModelRender,			ModelRender,	"VEngineModel016",			"engine.dll"); // model render interface
	ITF(ILuaShared,				Lua,			"LUASHARED003",				"lua_shared.dll"); // lua interface
	ITF(IGameEventManager2,		EventManager,	"GAMEEVENTSMANAGER002",		"engine.dll"); // event manager interface
	ITF(IEngineTraceClient,		EngineTrace,	"EngineTraceClient003",		"engine.dll"); // trace interface
}
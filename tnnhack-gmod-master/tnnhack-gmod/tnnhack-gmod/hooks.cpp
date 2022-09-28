#include "hooks.h"
#include "utils.h"
#include "detours.h"
#include "VMTHooks.h"
#include "interface.h"
#include "console.h"
#include "var.h"
#include "drawing.h"
#include "Features/esp.h"
#include "Features/entityList.h"
#include "Features/chams.h"
#include "Features/misc.h"
#include "SourceSDK/Interfaces/IGameEvent.h"
#include "config.h"

extern HWND g_MainWindow;
extern bool g_Keys[256];

void Hooks::Run()
{
	Hooks::g_oWndProc = sperma_cast<WNDPROC>(SetWindowLongA(g_MainWindow, GWL_WNDPROC, sperma_cast<LONG>(Hooks::hWndProc))); // hook wndproc

	void* clientMode = **sperma_cast<void***>((*sperma_cast<DWORD**>(Interfaces::Client()))[10] + 0x5); // clientMode class has CreateMove

	CVMTData* dataCreateMove = new CVMTData(clientMode, 21, sizeof(DWORD)); // CreateMove index 21
	g_pVmtHook->VMTHook("CreateMove", dataCreateMove, &Hooks::hCreateMove);
	
	CVMTData* dataGetViewmodelFOV = new CVMTData(clientMode, 32, sizeof(DWORD)); // GetViewmodelFOV index 32
	g_pVmtHook->VMTHook("GetViewmodelFOV", dataGetViewmodelFOV, &Hooks::hGetViewmodelFOV);

	CVMTData* dataShouldDrawFog = new CVMTData(clientMode, 15, sizeof(DWORD)); // ShouldDrawFog index 15
	g_pVmtHook->VMTHook("ShouldDrawFog", dataShouldDrawFog, &Hooks::hShouldDrawFog);

	CVMTData* dataPaintTraverse = new CVMTData(Interfaces::Panel(), 41, sizeof(DWORD)); // PaintTraverse index 41
	g_pVmtHook->VMTHook("PaintTraverse", dataPaintTraverse, &Hooks::hPaintTraverse);

	CVMTData* dataDrawModelExecute = new CVMTData(Interfaces::ModelRender(), 20, sizeof(DWORD));
	g_pVmtHook->VMTHook("DrawModelExecute", dataDrawModelExecute, &Hooks::hDrawModelexecute); // DrawModelExecute index 20

	Hooks::g_pHurtEventListener = new IGameEventListener2(Hooks::HurtEventHandler);
	Interfaces::EventManager()->AddListener(Hooks::g_pHurtEventListener, "player_hurt");

	Hooks::g_pDeathEventListener = new IGameEventListener2(Hooks::DeathEventHandler);
	Interfaces::EventManager()->AddListener(Hooks::g_pDeathEventListener, "player_death");
}

void Hooks::Shutdown()
{
	SetWindowLongA(g_MainWindow, GWL_WNDPROC, sperma_cast<LONG>(g_oWndProc)); // unhook wndproc

	Interfaces::EventManager()->RemoveListener(Hooks::g_pHurtEventListener);
	Interfaces::EventManager()->RemoveListener(Hooks::g_pDeathEventListener);
	delete g_pHurtEventListener;
	delete g_pDeathEventListener;

	g_pDetours->ClearAllHooks();
	g_pVmtHook->VMTUnhookAll();
}

void __fastcall Hooks::hPaintTraverse(void* __this, void* __reg, VPANEL panel, bool forceRepaint, bool allowForce)
{
	static tPaintTraverse oPaintTraverse = sperma_cast<tPaintTraverse>(g_pVmtHook->GetOriginalFunc("PaintTraverse"));
	(oPaintTraverse)(__this, panel, forceRepaint, allowForce);

	static VPANEL overlayPopupPanelId = 0;
	if (!overlayPopupPanelId)
	{
		const char* panelName = Interfaces::Panel()->GetName(panel);
		if (!lstrcmp(panelName, "OverlayPopupPanel"))
		{
			g_pDrawing->CreateFont("ESP", "Verdana", ESP_FONT_HEIGHT, 300, FONTFLAG_OUTLINE);
			g_pDrawing->CreateFont("Menu", "Arial", MENU_FONT_HEIGHT, 600, FONTFLAG_OUTLINE);
			g_pDrawing->CreateFont("Console", "Consolas", CONSOLE_FONT_HEIGHT, 400, FONTFLAG_DROPSHADOW);

			overlayPopupPanelId = panel;
		}
	}

	if (panel == overlayPopupPanelId)
	{
		g_pMisc->BypassSvCheats();
		g_pMisc->Thirdperson();

		Interfaces::Surface()->PushMakeCurrent(panel, false);

		if (Interfaces::Engine()->IsInGame() && Interfaces::Engine()->IsConnected())
		{
			g_pDrawing->PushFont("ESP");
			g_pEntityList->Redraw();
		}

		if (g_pConsole->GetEnabled())
		{
			g_pDrawing->PushFont("Console");
			g_pConsole->Redraw();
		}

		static CHackVarBool* menu_var_list = sperma_cast<CHackVarBool*>(g_pSettings->GetVar("menu_var_list"));

		g_pDrawing->PushFont("Menu");
		g_pDrawing->PushColor(255, 255, 0);
		g_pDrawing->DrawString(150.f, 60.f, "Hello, FishyFish3r :)");
		g_pDrawing->DrawString(150.f, 78.f, "Hello, eternalfrenzy :)");
		g_pDrawing->PushColor(255, 0, 255);
		g_pDrawing->DrawString(30.f, 56.f, "tnnhack");

		if (menu_var_list->m_Value)
		{
			static int maxWindowColumnSetts = 30;
			int yItem = 0;
			float xOffset = 0.f;
			float bestStringWidth = 0.f;

			static float rectX1 = 30.f;
			static float rectY1 = 100.f;
			float rectX2 = rectX1;
			float rectY2 = rectY1 + (zalypa_cast<float>(maxWindowColumnSetts) * MENU_FONT_HEIGHT) + 10.f;

			class __CMenuDrawSettingString
			{
			public:
				std::string m_Text;
				float m_X, m_Y;
			};

			class __CMenuDrawSettingRect
			{
			public:
				CHackVarColor* m_Color;
				float m_X, m_Y;
			};

			std::deque<__CMenuDrawSettingString> strings = {};
			std::deque<__CMenuDrawSettingRect> rects = {};

			for (CHackVarBase* var : g_pSettings->GetAllVars())
			{
				if (yItem >= maxWindowColumnSetts)
				{
					rectX2 += bestStringWidth + 10.f;
					xOffset += bestStringWidth + 5.f;
					bestStringWidth = 0.f;
					yItem = 0;
				}

				std::string drawString = var->m_Name + ": ";

				switch (var->m_Type)
				{
					case(CHackVarType_BOOL):
					{
						CHackVarBool* varBool = sperma_cast<CHackVarBool*>(var);
						drawString += varBool->m_Value ? "T" : "F";
						break;
					}
					case(CHackVarType_INT):
					{
						CHackVarInt* varInt = sperma_cast<CHackVarInt*>(var);

						char buf[256];
						sprintf(buf, "%d", varInt->m_Value);
						drawString += std::string(buf);

						break;
					}
					case(CHackVarType_FLOAT):
					{
						CHackVarFloat* varFloat = sperma_cast<CHackVarFloat*>(var);

						char buf[256];
						sprintf(buf, "%.1f", varFloat->m_Value);
						drawString += std::string(buf);

						break;
					}
					case(CHackVarType_STRING):
					{
						CHackVarString* varString = sperma_cast<CHackVarString*>(var);

						if (varString->m_Value.size() > 3)
						{
							for (unsigned i = 0; i < 3; i++)
							{
								drawString += varString->m_Value.at(i);
							}

							drawString += "...";
						}
						else
						{
							drawString += varString->m_Value;
						}

						break;
					}
					case(CHackVarType_COLOR):
					{
						CHackVarColor* varColor = sperma_cast<CHackVarColor*>(var);

						Vector2i unformattedTextSize = g_pDrawing->GetTextSize(drawString.c_str());
						rects.push_back({ varColor, rectX1 + 10.f + xOffset + unformattedTextSize.x - 2.f, rectY1 + 15.f + (yItem * MENU_FONT_HEIGHT) });

						break;
					}
				}

				Vector2i textSize = g_pDrawing->GetTextSize(drawString.c_str());

				if (var->m_Type == CHackVarType_COLOR)
				{
					textSize.x += 15.f;
				}

				if (textSize.x > bestStringWidth)
				{
					bestStringWidth = textSize.x;
				}

				strings.push_back({ drawString, rectX1 + 10.f + xOffset, rectY1 + 10.f + (yItem * MENU_FONT_HEIGHT) });

				yItem++;
			}

			rectX2 += bestStringWidth + 10.f;
			rectY2 += 10.f;

			g_pDrawing->PushColor(0, 255, 0);
			g_pDrawing->DrawBorderedRect(rectX1 - 1.f, rectY1 - 1.f, rectX2 + 1.f, rectY2 + 1.f);
			g_pDrawing->PushColor(3, 3, 3, 150);
			g_pDrawing->DrawFilledRect(rectX1, rectY1, rectX2, rectY2);

			g_pDrawing->PushColor(255, 255, 255);
			for (__CMenuDrawSettingString s : strings)
			{
				g_pDrawing->DrawString(s.m_X, s.m_Y, s.m_Text.c_str());
			}

			for (__CMenuDrawSettingRect r : rects)
			{
				g_pDrawing->PushColor(0, 0, 0);
				g_pDrawing->DrawFilledRect(r.m_X - 1.f, r.m_Y - 1.f, r.m_X + 14.f, r.m_Y + 7.f);
				g_pDrawing->PushColor(r.m_Color->m_Red, r.m_Color->m_Green, r.m_Color->m_Blue, r.m_Color->m_Alpha);
				g_pDrawing->DrawFilledRect(r.m_X, r.m_Y, r.m_X + 13.f, r.m_Y + 6.f);
			}

			g_pDrawing->PushColor(255, 255, 255);
			g_pDrawing->DrawString(rectX1, rectY2 + 2.f, g_pConfigSystem->GetCurrentConfig().c_str());
			g_pDrawing->DrawString(rectX1, rectY2 + MENU_FONT_HEIGHT + 2.f, "Total vars: %i", g_pSettings->GetAllVars().size());
		}
		else
		{
			g_pDrawing->PushColor(255, 255, 255);
			g_pDrawing->DrawString(30.f, 40.f, g_pConfigSystem->GetCurrentConfig().c_str());
		}

#ifdef _DEBUG

		// draw debug info
		g_pDrawing->PushColor(255, 255, 255);
		g_pDrawing->DrawString(40.f, 25.f, "DEBUGBUILD: Press END to unload");

		if (Interfaces::Engine()->IsInGame() && Interfaces::Engine()->IsConnected())
		{
			int localPlayer = Interfaces::Engine()->GetLocalPlayer();
			CBasePlayer* pLocal = sperma_cast<CBasePlayer*>(Interfaces::EntityList()->GetClientEntity(localPlayer));
			if (pLocal)
			{
				g_pDrawing->DrawString(10.f, 600.f, "LocalPlayer: 0x%x", sperma_cast<DWORD>(pLocal));
			}
		}

#endif

		Interfaces::Surface()->PopMakeCurrent(panel);
	}
}

WNDPROC Hooks::g_oWndProc = NULL;
LRESULT CALLBACK Hooks::hWndProc(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam) // wndproc, hooked for key updates
{
	switch (msg)
	{
		case(WM_CHAR): // displayable key event
		{
			switch (wparam)
			{
				case(VK_BACK): // backspace
				{
					if (g_pConsole->GetEnabled())
					{
						g_pConsole->OnKey(wparam);
					}
					break;
				}
				case(0x0A): // unknown key but need to be processed
				{
					break;
				}
				case(VK_ESCAPE): // esc
				{
					if (g_pConsole->GetEnabled())
					{
						g_pConsole->SetEnabled(false);
						Interfaces::InputSystem()->EnableInput(true);
					}
					break;
				}
				case(VK_TAB): // tab
				{
					if (g_pConsole->GetEnabled())
					{
						g_pConsole->OnKey(wparam);
					}
					break;
				}
				case(VK_RETURN): // enter
				{
					if (g_pConsole->GetEnabled())
					{
						g_pConsole->OnKey(wparam);
					}
					break;
				}
				default: // any renderable keys
				{
					if (g_pConsole->GetEnabled())
					{
						g_pConsole->OnKey(wparam);
					}
					break;
				}
			}

			break;
		}
		case(WM_KEYDOWN): // key press event
		{
			g_Keys[wparam] = true; // set global key pressed

			switch (wparam)
			{
				case(VK_INSERT):
				{
					g_pConsole->Toggle();

					if (g_pConsole->GetEnabled())
					{
						Interfaces::InputSystem()->EnableInput(false);
					}
					else
					{
						Interfaces::InputSystem()->EnableInput(true);
					}

					break;
				}
				case(VK_LEFT): // left arrow
				{
					if (g_pConsole->GetEnabled())
					{
						g_pConsole->OnKey(wparam);
					}
					break;
				}
				case(VK_RIGHT): // right arrow
				{
					if (g_pConsole->GetEnabled())
					{
						g_pConsole->OnKey(wparam);
					}
					break;
				}
				case(VK_UP): // up arrow
				{
					if (g_pConsole->GetEnabled())
					{
						g_pConsole->OnKey(wparam);
					}
					break;
				}
			}

			break;
		}
		case(WM_KEYUP): // key release event
		{
			g_Keys[wparam] = false; // set global key not pressed
			break;
		}
	}

	return CallWindowProcA(Hooks::g_oWndProc, wnd, msg, wparam, lparam);
}

bool __fastcall Hooks::hCreateMove(void* __this, void* __reg, float sample, CUserCMD* cmd)
{
	static tCreateMove oCreateMove = sperma_cast<tCreateMove>(g_pVmtHook->GetOriginalFunc("CreateMove"));

	if (!cmd || !cmd->command_number)
	{
		return (oCreateMove)(__this, sample, cmd);
	}

	if (Interfaces::Engine()->IsInGame() && Interfaces::Engine()->IsConnected())
	{
		g_pEntityList->CreateMove(sample, cmd);
	}

	// clamp angles and movement
	cmd->viewangles.Normalize();
	cmd->forwardmove = std::clamp(cmd->forwardmove, -450.f, 450.f);
	cmd->sidemove = std::clamp(cmd->sidemove, -450.f, 450.f);
	cmd->upmove = std::clamp(cmd->upmove, -320.f, 320.f);

	return false;
}

float __fastcall Hooks::hGetViewmodelFOV(void* __this, void* __reg)
{
	static CHackVarBool* custom_hand_fov = sperma_cast<CHackVarBool*>(g_pSettings->GetVar("custom_hand_fov"));
	static CHackVarFloat* custom_hand_fov_value = sperma_cast<CHackVarFloat*>(g_pSettings->GetVar("custom_hand_fov_value"));
	static CHackVarBool* disable_all = sperma_cast<CHackVarBool*>(g_pSettings->GetVar("disable_all"));

	if (custom_hand_fov->m_Value && !disable_all->m_Value)
	{
		return custom_hand_fov_value->m_Value;
	}

	static tGetViewmodelFOV oGetViewmodelFov = sperma_cast<tGetViewmodelFOV>(g_pVmtHook->GetOriginalFunc("GetViewmodelFOV"));
	return (oGetViewmodelFov)(__this);
}

bool __fastcall Hooks::hShouldDrawFog(void* __this, void* __reg)
{
	static CHackVarBool* should_draw_fog = sperma_cast<CHackVarBool*>(g_pSettings->GetVar("should_draw_fog"));
	static CHackVarBool* disable_all = sperma_cast<CHackVarBool*>(g_pSettings->GetVar("disable_all"));

	if (!disable_all->m_Value)
	{
		return should_draw_fog->m_Value;
	}

	static tShouldDrawFog oShouldDrawFog = sperma_cast<tShouldDrawFog>(g_pVmtHook->GetOriginalFunc("ShouldDrawFog"));
	return (oShouldDrawFog)(__this);
}

void __fastcall Hooks::hDrawModelexecute(void* __this, void* __reg, void* state, ModelRenderInfo_t& info, void* pCustomBoneToWorld)
{
	static Hooks::tDrawModelExecute oDrawModelExecute = sperma_cast<Hooks::tDrawModelExecute>(g_pVmtHook->GetOriginalFunc("DrawModelExecute"));
	if (info.pModel)
	{
		CBasePlayer* entity = zalypa_cast<CBasePlayer*>(Interfaces::EntityList()->GetClientEntity(info.entity_index));
		if (!entity || entity->IsDormant())
		{
			oDrawModelExecute(__this, state, info, pCustomBoneToWorld);
			return;
		}

		if (entity->IsAlive())
		{
			g_pChams->DrawModel(state, info, pCustomBoneToWorld);
			Interfaces::ModelRender()->ForcedMaterialOverride(nullptr);
			return;
		}
	}

	oDrawModelExecute(__this, state, info, pCustomBoneToWorld);
}

IGameEventListener2* Hooks::g_pHurtEventListener = nullptr;
void Hooks::HurtEventHandler(IGameEvent2* ev)
{
	int user = ev->GetInt("userid");
	int attacker = ev->GetInt("attacker");
	int health = ev->GetInt("health");
	int userID = Interfaces::Engine()->GetPlayerForUserID(user);
	int attackerID = Interfaces::Engine()->GetPlayerForUserID(attacker);
	if (attackerID == Interfaces::Engine()->GetLocalPlayer())
	{
		CPlayerInfo info;
		Interfaces::Engine()->GetPlayerInfo(userID, &info);
		if (info.name)
		{
			static const std::string log = "You caused damage to ";
			std::string name;
			std::string damage = std::to_string(100 - health);
			for (int i = 0; info.name[i] != 0; i++)
			{
				name += info.name[i];
			}
			if (!name.empty())
			{
				std::string result = log + name + " (" + damage + ")";
				g_pConsole->Print(result);
			}
		}
	}
}

IGameEventListener2* Hooks::g_pDeathEventListener = nullptr;
void Hooks::DeathEventHandler(IGameEvent2* ev)
{
	int user = ev->GetInt("userid");
	int attacker = ev->GetInt("attacker");
	int userID = Interfaces::Engine()->GetPlayerForUserID(user);
	int attackerID = Interfaces::Engine()->GetPlayerForUserID(attacker);
	if (attackerID == Interfaces::Engine()->GetLocalPlayer())
	{
		CPlayerInfo info;
		Interfaces::Engine()->GetPlayerInfo(userID, &info);
		if (info.name)
		{
			std::string userName;
			for (int i = 0; info.name[i] != 0; i++)
			{
				userName += info.name[i];
			}
			if (!userName.empty())
			{
				std::string result = "You killed " + userName;
				g_pConsole->Print(result);
			}
		}
	}
}
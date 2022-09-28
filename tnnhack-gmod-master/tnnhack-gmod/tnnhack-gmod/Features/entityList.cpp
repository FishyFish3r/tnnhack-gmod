#define _USE_MATH_DEFINES
#include <math.h>
#include "entityList.h"
#include "esp.h"
#include "movement.h"
#include "aim.h"
#include "../var.h"
#include "../SourceSDK/in_buttons.h"
#include "../drawing.h"
#include "../sperma_cast.h"
#include "antiAim.h"
#include "../utils.h"
#include "misc.h"
#include "bones.h"

CEntityList* g_pEntityList = nullptr;

void CEntityList::Redraw() const
{
	static CHackVarBool* esp_enable = sperma_cast<CHackVarBool*>(g_pSettings->GetVar("esp_enable"));
	static CHackVarBool* esp_box = sperma_cast<CHackVarBool*>(g_pSettings->GetVar("esp_box"));
	static CHackVarBool* esp_line = sperma_cast<CHackVarBool*>(g_pSettings->GetVar("esp_line"));
	static CHackVarBool* esp_heading = sperma_cast<CHackVarBool*>(g_pSettings->GetVar("esp_heading"));
	static CHackVarBool* esp_info = sperma_cast<CHackVarBool*>(g_pSettings->GetVar("esp_info"));
	static CHackVarBool* esp_skeleton = sperma_cast<CHackVarBool*>(g_pSettings->GetVar("esp_skeleton"));
	static CHackVarBool* disable_all = sperma_cast<CHackVarBool*>(g_pSettings->GetVar("disable_all"));
	static CHackVarBool* aa_angle_circle = sperma_cast<CHackVarBool*>(g_pSettings->GetVar("aa_angle_circle"));
	static CHackVarBool* aim_fov = sperma_cast<CHackVarBool*>(g_pSettings->GetVar("aim_fov"));
	static CHackVarBool* name_steal = sperma_cast<CHackVarBool*>(g_pSettings->GetVar("name_steal"));
	
	if (name_steal->m_Value)
	{
		g_pMisc->NameSteal();
	}

	if (esp_enable->m_Value && !disable_all->m_Value)
	{
		int localPlayer = Interfaces::Engine()->GetLocalPlayer();
		for (int i = 0; i < Interfaces::Engine()->GetMaxClients(); i++)
		{
			if (i == localPlayer)
			{
				continue;
			}

			CBasePlayer* entity = sperma_cast<CBasePlayer*>(Interfaces::EntityList()->GetClientEntity(i));
			if (!entity)
			{
				continue;
			}

			if (entity->IsDormant())
			{
				continue;
			}

			if (!entity->IsAlive())
			{
				continue;
			}

			// esp...
			if (esp_box->m_Value)
			{
				g_pEsp->Box2d(entity);
			}

			if (esp_info->m_Value)
			{
				g_pEsp->PlayerInfo(entity, i);
			}

			if (esp_line->m_Value)
			{
				g_pEsp->Snaplines(entity);
			}

			if (esp_heading->m_Value)
			{
				g_pEsp->Heading(entity);
			}

			if (esp_skeleton->m_Value)
			{
				g_pEsp->Skeleton(entity);
			}
		}
	}
	
	if (aa_angle_circle->m_Value && !disable_all->m_Value)
	{
		g_pAntiAim->DrawAngle();
	}

	/*if (aim_fov->m_Value == true)
	{
		g_pAimbot->DrawCircleFov();
	}
	*/
}

void CEntityList::CreateMove(float sample, CUserCMD* cmd) const
{
	static CHackVarBool* aim_enable = sperma_cast<CHackVarBool*>(g_pSettings->GetVar("aim_enable"));
	static CHackVarBool* aim_fov = sperma_cast<CHackVarBool*>(g_pSettings->GetVar("aim_fov"));
	static CHackVarBool* aa_enable = sperma_cast<CHackVarBool*>(g_pSettings->GetVar("aa_enable"));
	static CHackVarBool* autostrafe_enable = sperma_cast<CHackVarBool*>(g_pSettings->GetVar("autostrafe_enable"));
	static CHackVarBool* bunnyhop_enable = sperma_cast<CHackVarBool*>(g_pSettings->GetVar("bunnyhop_enable"));
	static CHackVarBool* disable_all = sperma_cast<CHackVarBool*>(g_pSettings->GetVar("disable_all"));

	g_pAntiAim->m_OldCmdAngle = cmd->viewangles;

	// movement...
	if (bunnyhop_enable && !disable_all->m_Value)
	{
		g_pMovement->Bunnyhop(cmd);
	}

	if (autostrafe_enable->m_Value && !disable_all->m_Value)
	{
		g_pMovement->Autostrafe(cmd);
	}

	// backup for movefix
	QAngle oldAngle = cmd->viewangles;
	float oldForward = cmd->forwardmove;
	float oldSide = cmd->sidemove;

	// antiAim
	if (aa_enable->m_Value && !disable_all->m_Value)
	{
		bool shouldAA = !(cmd->buttons & IN_ATTACK);
		if (shouldAA)
		{
			g_pAntiAim->AntiAim(cmd);
		}
	}

	bool isAttacking = cmd->buttons & IN_ATTACK;

	// aimbot
	if (aim_enable->m_Value && !disable_all->m_Value)
	{
		g_pAimbot->Begin();

		int localPlayer = Interfaces::Engine()->GetLocalPlayer();
		for (int i = 0; i < Interfaces::Engine()->GetMaxClients(); i++)
		{
			if (i == localPlayer)
			{
				continue;
			}

			CBasePlayer* entity = sperma_cast<CBasePlayer*>(Interfaces::EntityList()->GetClientEntity(i));
			if (entity == nullptr)
			{
				continue;
			}

			if (entity->IsDormant())
			{
				continue;
			}

			if (!entity->IsAlive())
			{
				continue;
			}

			g_pAimbot->TestEntity(entity, aim_fov->m_Value);
		}

		if (isAttacking)				// фикс бестентити в есп
		{
			g_pAimbot->Rage(cmd);
		}
	}

	if (isAttacking)
	{
		g_pAimbot->RCS(cmd);
	}

	// movefix
	float delta;
	float f1;
	float f2;

	if (oldAngle.yaw < 0.f)
		f1 = 360.f + oldAngle.yaw;
	else
		f1 = oldAngle.yaw;

	if (cmd->viewangles.yaw < 0.0f)
		f2 = 360.0f + cmd->viewangles.yaw;
	else
		f2 = cmd->viewangles.yaw;

	if (f2 < f1)
		delta = abs(f2 - f1);
	else
		delta = 360.0f - abs(f1 - f2);

	delta = 360.0f - delta;
	cmd->forwardmove = cos(delta * M_PI / 180.f) * oldForward + cos((delta + 90.f) * M_PI / 180.f) * oldSide;
	cmd->sidemove = sin(delta * M_PI / 180.f) * oldForward + sin((delta + 90.f) * M_PI / 180.f) * oldSide;

	g_pAntiAim->m_AntiAimAngle = cmd->viewangles;
}
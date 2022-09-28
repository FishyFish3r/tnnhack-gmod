#include <string>
#include "esp.h"
#include "../interface.h"
#include "../drawing.h"
#include "../sperma_cast.h"
#include "../utils.h"
#include "aim.h"
#include "antiAim.h"
#include "bones.h"

CEsp* g_pEsp = nullptr;

void CEsp::Snaplines(CBasePlayer* entity) const
{
	static CHackVarColor* esp_line_color = sperma_cast<CHackVarColor*>(g_pSettings->GetVar("esp_line_color"));

	Vector3f pos = entity->GetAbsOrigin();
	Vector2i screenPos;

	if (g_pUtils->World2Screen(pos, screenPos))
	{
		Vector2i screenSize;
		Interfaces::Engine()->GetScreenSize(screenSize.x, screenSize.y); // get screen size from engine

		if (g_pAimbot->m_BestEntity == entity)
		{
			g_pDrawing->PushColor(255, 0, 0, esp_line_color->m_Alpha);
		}
		else if (entity == g_pAimbot->FriendAddy())
		{
			g_pDrawing->PushColor(0, 255, 0, esp_line_color->m_Alpha);
		}
		else
		{
			g_pDrawing->PushColor(esp_line_color->m_Red, esp_line_color->m_Green, esp_line_color->m_Blue, esp_line_color->m_Alpha);
		}

		g_pDrawing->DrawLine(screenSize.x / 2, screenSize.y, screenPos.x, screenPos.y); // from bottom center on screen to entity on screen
	}
}

void CEsp::Box2d(CBasePlayer* entity) const
{
	static CHackVarColor* esp_box_color = sperma_cast<CHackVarColor*>(g_pSettings->GetVar("esp_box_color"));

	Vector3f posLegs = entity->GetAbsOrigin(); // origin position is the bottom center of legs
	Vector3f posHead = posLegs;
	posHead.z += 72.f; // bottom center of legs + 72 up 

	Vector2i screenPosL; // legs screen pos
	Vector2i screenPosH; // head screen pos
	if (g_pUtils->World2Screen(posLegs, screenPosL) &&
		g_pUtils->World2Screen(posHead, screenPosH))
	{
		float High = screenPosL.y - screenPosH.y; // legs screen pos y - head screen pos y
		float Width = High * 0.27f; // calc width magic

		if (g_pAimbot->m_BestEntity == entity)
		{
			g_pDrawing->PushColor(255, 0, 0, esp_box_color->m_Alpha);
		}
		else if (entity == g_pAimbot->FriendAddy())
		{
			g_pDrawing->PushColor(0, 255, 0, esp_box_color->m_Alpha);
		}
		else
		{
			g_pDrawing->PushColor(esp_box_color->m_Red, esp_box_color->m_Green, esp_box_color->m_Blue, esp_box_color->m_Alpha);
		}

		g_pDrawing->DrawBorderedRectOutline(screenPosH.x - Width, screenPosH.y, screenPosL.x + Width, screenPosL.y);
	}
}

void CEsp::PlayerInfo(CBasePlayer* entity, int idx) const
{
	Vector3f posLegs = entity->GetAbsOrigin(); // origin position is the bottom center of legs
	Vector3f posHead = posLegs;
	posHead.z += 72.f; // bottom center of legs + 72 up 

	Vector2i screenPosL; // legs screen pos
	Vector2i screenPosH; // head screen pos

	if (g_pUtils->World2Screen(posLegs, screenPosL) &&
		g_pUtils->World2Screen(posHead, screenPosH))
	{
		int entityHp = entity->GetHealth();

		CBasePlayer* lPlayer = zalypa_cast<CBasePlayer*>(Interfaces::EntityList()->GetClientEntity(Interfaces::Engine()->GetLocalPlayer()));
		Vector3f lPEye = lPlayer->GetAbsOrigin();
		Vector3f entityPos = entity->GetAbsOrigin();
		float distU = lPEye.Distance3D(entityPos);
		float distM = (distU * 1.906f) / 100.f;

		float y = screenPosL.y;

		g_pDrawing->PushColor(0, 222, 0);
		g_pDrawing->DrawString(screenPosL.x, y, "%i hp", entityHp); // hp

		y += ESP_FONT_HEIGHT + 2.f;

		g_pDrawing->PushColor(255, 255, 255);
		g_pDrawing->DrawString(screenPosL.x, y, "%.1f m", distM); // dist

		y += ESP_FONT_HEIGHT + 2.f;
		g_pDrawing->PushColor(255, 255, 255);
		g_pDrawing->DrawString(screenPosL.x, y, "%i ID", idx); // dist

		y += ESP_FONT_HEIGHT + 2.f;

		if (g_pAimbot->m_BestEntity == entity)
		{
			g_pDrawing->PushColor(255, 0, 0);
			g_pDrawing->DrawString(screenPosL.x, y, "AIMBOT TARGET");
		}
		else if (entity == g_pAimbot->FriendAddy())
		{
			g_pDrawing->PushColor(0, 255, 0);
			g_pDrawing->DrawString(screenPosL.x, y, "FRIEND");
		}

		CPlayerInfo engineInfo;
		if (Interfaces::Engine()->GetPlayerInfo(idx, &engineInfo))
		{
			if (engineInfo.name)
			{
				wchar_t wname[MAX_PLAYER_NAME_LENGTH];
				if (MultiByteToWideChar(CP_UTF8, 0, engineInfo.name, -1, wname, MAX_PLAYER_NAME_LENGTH))
				{
					g_pDrawing->PushColor(255, 255, 255);
					g_pDrawing->DrawStringW(screenPosH.x, screenPosH.y - 18.f, wname);
				}
			}
		}
	}
}

void CEsp::Heading(CBasePlayer* entity) const
{
	Vector3f eyePos = entity->GetEyePos();
	QAngle ang = entity->GetAngle();

	Vector3f forward = {};
	g_pUtils->AngleVectors(ang, &forward, nullptr, nullptr);

	Vector3f endPos = eyePos + (forward * 45.f);

	Vector2i eyePosScreen, endPosScreen;
	if (g_pUtils->World2Screen(eyePos, eyePosScreen) &&
		g_pUtils->World2Screen(endPos, endPosScreen))
	{
		if (g_pAimbot->m_BestEntity == entity)
		{
			g_pDrawing->PushColor(255, 0, 0);
		}
		else if (entity == g_pAimbot->FriendAddy())
		{
			g_pDrawing->PushColor(0, 255, 0);
		}
		else
		{
			g_pDrawing->PushColor(255, 255, 255);
		}

		g_pDrawing->DrawLine(eyePosScreen.x, eyePosScreen.y, endPosScreen.x, endPosScreen.y);
	}
}

static const char* g_SkeletonBones[14][2] = {
	{ "ValveBiped.Bip01_Head1", "ValveBiped.Bip01_Neck1" },
	{ "ValveBiped.Bip01_Neck1", "ValveBiped.Bip01_Pelvis" },
	{ "ValveBiped.Bip01_Pelvis", "ValveBiped.Bip01_L_Thigh" },
	{ "ValveBiped.Bip01_Pelvis", "ValveBiped.Bip01_R_Thigh" },
	{ "ValveBiped.Bip01_R_Thigh", "ValveBiped.Bip01_R_Calf" },
	{ "ValveBiped.Bip01_R_Calf", "ValveBiped.Bip01_R_Foot" },
	{ "ValveBiped.Bip01_L_Thigh", "ValveBiped.Bip01_L_Calf" },
	{ "ValveBiped.Bip01_L_Calf", "ValveBiped.Bip01_L_Foot" },
	{ "ValveBiped.Bip01_Neck1", "ValveBiped.Bip01_R_UpperArm" },
	{ "ValveBiped.Bip01_R_UpperArm", "ValveBiped.Bip01_R_Forearm" },
	{ "ValveBiped.Bip01_R_Forearm", "ValveBiped.Bip01_R_Hand" },
	{ "ValveBiped.Bip01_Neck1", "ValveBiped.Bip01_L_UpperArm" },
	{ "ValveBiped.Bip01_L_UpperArm", "ValveBiped.Bip01_L_Forearm" },
	{ "ValveBiped.Bip01_L_Forearm", "ValveBiped.Bip01_L_Hand" }
};

void CEsp::Skeleton(CBasePlayer* entity) const
{
	static CHackVarColor* esp_skeleton_color = sperma_cast<CHackVarColor*>(g_pSettings->GetVar("esp_skeleton_color"));

	Vector3f startBone;
	Vector3f endBone;

	int start;
	int end;

	Vector2i startScreen;
	Vector2i endScreen;

	for (unsigned i = 0; i < 14; i++)
	{
		start = entity->LookupBone(g_SkeletonBones[i][0]);
		end = entity->LookupBone(g_SkeletonBones[i][1]);
		
		g_pBones->GetBonePosition(entity, start, &startBone);
		g_pBones->GetBonePosition(entity, end, &endBone);

		if (g_pUtils->World2Screen(startBone, startScreen) && g_pUtils->World2Screen(endBone, endScreen))
		{
			g_pDrawing->PushColor(esp_skeleton_color->m_Red, esp_skeleton_color->m_Green, esp_skeleton_color->m_Blue, esp_skeleton_color->m_Alpha);
			g_pDrawing->DrawLine(startScreen.x, startScreen.y, endScreen.x, endScreen.y);
		}
	}
}
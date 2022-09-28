#define _USE_MATH_DEFINES
#include <math.h>
#include "antiAim.h"
#include "../drawing.h"
#include "../sperma_cast.h"
#include "../interface.h"
#include "../SourceSDK/const.h"

CAntiAim* g_pAntiAim = nullptr;
extern bool g_Keys[256];

void CAntiAim::AntiAim(CUserCMD* cmd)
{
	static CHackVarInt* aa_type = sperma_cast<CHackVarInt*>(g_pSettings->GetVar("aa_type"));

	int localPlayer = Interfaces::Engine()->GetLocalPlayer();
	CBasePlayer* pLocal = sperma_cast<CBasePlayer*>(Interfaces::EntityList()->GetClientEntity(localPlayer));
	if (!pLocal || !pLocal->IsAlive())
	{
		return;
	}
	if (pLocal->GetFlags() & FL_ONGROUND)		// не нарушаю работу автострейфа
	{
		switch (aa_type->m_Value)
		{
			case (EAntiAimType_STATIC_AA):
			{
				this->m_AntiAimAngle = cmd->viewangles;
				if (g_Keys[VK_RIGHT])
				{
					this->m_AntiAimAngle.yaw += 90.f;
				}
				else if (g_Keys[VK_LEFT])
				{
					this->m_AntiAimAngle.yaw -= 90.f;
				}
				else
				{
					this->m_AntiAimAngle.yaw += 180.f;
				}
				this->m_AntiAimAngle.pitch = 89.f;
				cmd->viewangles = this->m_AntiAimAngle;
				
				break;
			}
			case (EAntiAimType_JITTER_AA):
			{
				static bool left = true;
				this->m_AntiAimAngle = cmd->viewangles;
				this->m_AntiAimAngle.pitch = 89.f;
				left == true ? this->m_AntiAimAngle.yaw += 90.f : this->m_AntiAimAngle.yaw -= 90.f;
				left = !left;
				cmd->viewangles = this->m_AntiAimAngle;
				break;
			}
			case (EAntiAimType_SPIN_AA):
			{
				static float spinyaw = 0.0f;
				if (spinyaw >= 180.f)
				{
					spinyaw = 0.f;
				}
				else
				{
					spinyaw += 11.f;
				}
				this->m_AntiAimAngle = cmd->viewangles;
				this->m_AntiAimAngle.pitch = 89.f;
				this->m_AntiAimAngle.yaw = spinyaw;
				cmd->viewangles = this->m_AntiAimAngle;
				break;
			}
		}
	}
	
}

// first - yaw, second - pitch
std::pair<std::string, std::string> AngleToString(float yaw, float pitch)
{
	std::string yawStr = "forward";
	std::string pitchStr = "forward";

	float realPitch = std::clamp(pitch, -89.f, 89.f);
	float realYaw = std::clamp(std::remainder(yaw, 360.f), -180.f, 180.f);

	if (realYaw <= 45.f && realYaw >= -45.f)
	{
		yawStr = "forward";
	}
	else if (realYaw < 45.f && realYaw >= -135.f)
	{
		yawStr = "left";
	}
	else if (realYaw <= 135.f && realYaw > -45.f)
	{
		yawStr = "right";
	}
	else
	{
		yawStr = "back";
	}

	if (realPitch <= 45.f && realPitch >= -45.f)
	{
		pitchStr = "forward";
	}
	else if (realPitch < -45.f)
	{
		pitchStr = "up";
	}
	else
	{
		pitchStr = "down";
	}

	return std::make_pair(yawStr, pitchStr);
}

void CAntiAim::DrawAngle()
{
	static CHackVarFloat* aa_angle_circle_radius = sperma_cast<CHackVarFloat*>(g_pSettings->GetVar("aa_angle_circle_radius"));
	static CHackVarBool* aa_angle_circle_caption = sperma_cast<CHackVarBool*>(g_pSettings->GetVar("aa_angle_circle_caption"));

	// get local player
	int localPlayer = Interfaces::Engine()->GetLocalPlayer();
	CBasePlayer* pLocal = sperma_cast<CBasePlayer*>(Interfaces::EntityList()->GetClientEntity(localPlayer));
	if (!pLocal || !pLocal->IsAlive())
	{
		return;
	}

	Vector2i screenSize;
	Interfaces::Engine()->GetScreenSize(screenSize.x, screenSize.y); // get screen size from engine

	float circleX = screenSize.x - 400.f;
	static float circleY = 200.f;

	float yawRad = this->m_AntiAimAngle.yaw * M_PI / 180.f;
	float pointX = circleX + (aa_angle_circle_radius->m_Value * cos(yawRad));
	float pointY = circleY + (aa_angle_circle_radius->m_Value * sin(yawRad));

	float oldYawRad = this->m_OldCmdAngle.yaw * M_PI / 180.f;
	float oldPointX = circleX + (aa_angle_circle_radius->m_Value * cos(oldYawRad));
	float oldPointY = circleY + (aa_angle_circle_radius->m_Value * sin(oldYawRad));

	float pitchRad = this->m_AntiAimAngle.pitch * M_PI / 180.f;
	float pitchPointY = circleY + (aa_angle_circle_radius->m_Value * sin(pitchRad));

	float oldPitchRad = this->m_OldCmdAngle.pitch * M_PI / 180.f;
	float oldPitchPointY = circleY + (aa_angle_circle_radius->m_Value * sin(oldPitchRad));

	g_pDrawing->PushColor(255, 255, 255);
	g_pDrawing->DrawCircle(circleX, circleY, aa_angle_circle_radius->m_Value, 240);

	g_pDrawing->PushColor(255, 255, 0, 90);
	g_pDrawing->DrawLine(circleX, circleY, oldPointX, oldPointY);

	g_pDrawing->PushColor(0, 255, 0, 90);
	g_pDrawing->DrawLine(circleX, circleY, circleX, oldPitchPointY);

	g_pDrawing->PushColor(255, 0, 0);
	g_pDrawing->DrawLine(circleX, circleY, pointX, pointY);

	g_pDrawing->PushColor(0, 255, 0);
	g_pDrawing->DrawLine(circleX, circleY, circleX, pitchPointY);

	if (aa_angle_circle_caption->m_Value)
	{
		g_pDrawing->PushFont("Menu");

		std::pair<std::string, std::string> str = AngleToString(this->m_AntiAimAngle.yaw - this->m_OldCmdAngle.yaw, this->m_AntiAimAngle.pitch - this->m_OldCmdAngle.pitch);

		g_pDrawing->PushColor(255, 0, 0);
		g_pDrawing->DrawString(circleX + aa_angle_circle_radius->m_Value + 20.f, circleY, "yaw: %s", str.first.c_str());

		g_pDrawing->PushColor(0, 255, 0);
		g_pDrawing->DrawString(circleX + aa_angle_circle_radius->m_Value + 20.f, circleY + MENU_FONT_HEIGHT + 2.f, "pitch: %s", str.second.c_str());
	}
}
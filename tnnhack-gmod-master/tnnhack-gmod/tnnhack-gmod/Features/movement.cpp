#include "movement.h"
#include "../interface.h"
#include "../SourceSDK/const.h"
#include "../SourceSDK/in_buttons.h"

CMovement* g_pMovement = nullptr;

void CMovement::Bunnyhop(CUserCMD* cmd) const
{
	// get local player
	int localPlayer = Interfaces::Engine()->GetLocalPlayer();
	CBasePlayer* pLocal = sperma_cast<CBasePlayer*>(Interfaces::EntityList()->GetClientEntity(localPlayer));
	if (!pLocal || !pLocal->IsAlive())
	{
		return;
	}

	// if pressing jump
	if (cmd->buttons & IN_JUMP)
	{
		if (pLocal->GetFlags() & FL_ONGROUND) // if on ground
		{
			cmd->buttons |= IN_JUMP; // keep jump key
		}
		else // not on ground
		{
			cmd->buttons &= ~IN_JUMP; // release jump key
		}
	}
}

void CMovement::Autostrafe(CUserCMD* cmd) const
{
	// get local player
	int localPlayer = Interfaces::Engine()->GetLocalPlayer();
	CBasePlayer* pLocal = sperma_cast<CBasePlayer*>(Interfaces::EntityList()->GetClientEntity(localPlayer));
	if (!pLocal || !pLocal->IsAlive() || pLocal->GetFlags() & FL_ONGROUND) // dont strafe if on ground
	{
		return;
	}

	static bool left = false; // left strafe if left == true, right strafe if left == false

	if (left) // left strafe
	{
		cmd->viewangles.yaw += 12.f;
		cmd->sidemove = 450.f;
	}
	else // right strafe
	{
		cmd->viewangles.yaw -= 12.f;
		cmd->sidemove = -450.f;
	}

	left = !left; // change strafe direction every tick
}
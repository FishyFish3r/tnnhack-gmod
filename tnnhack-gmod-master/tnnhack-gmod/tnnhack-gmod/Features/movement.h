#pragma once

#include "../SourceSDK/usercmd.h"

class CMovement
{
public:
	void Bunnyhop(CUserCMD* cmd) const;
	void Autostrafe(CUserCMD* cmd) const;
};

extern CMovement* g_pMovement;
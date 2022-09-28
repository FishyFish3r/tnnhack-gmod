#pragma once

#include "../SourceSDK/Math/qangle.h"
#include "../SourceSDK/Math/vector.h"
#include "../SourceSDK/usercmd.h"
#include "../SourceSDK/entity.h"

class CAimbot
{
public:
	QAngle			m_AimAngle;
	CBaseEntity*	m_BestEntity;
	float			m_BestSearchValue;
public:
	CAimbot();

	void Rage(CUserCMD* cmd);
	void TestEntity(CBaseEntity* a_Entity, bool fov);
	void Begin();

	CBaseEntity* FriendAddy();

	void DrawCircleFov();

	void RCS(CUserCMD* cmd);
};

extern CAimbot* g_pAimbot;
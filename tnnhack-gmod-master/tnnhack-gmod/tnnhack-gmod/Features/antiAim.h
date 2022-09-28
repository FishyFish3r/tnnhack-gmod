#pragma once

#include "../SourceSDK/Math/qangle.h"
#include "../SourceSDK/Math/vector.h"
#include "../SourceSDK/usercmd.h"
#include "../SourceSDK/entity.h"
#include "../var.h"

enum EAntiAimType
{
	EAntiAimType_STATIC_AA = 1,
	EAntiAimType_JITTER_AA = 2,
	EAntiAimType_SPIN_AA = 3,
};

class CAntiAim
{
public:
	QAngle m_AntiAimAngle;
	QAngle m_OldCmdAngle;
public:
	void AntiAim(CUserCMD* cmd);
	void DrawAngle();
};

extern CAntiAim* g_pAntiAim;
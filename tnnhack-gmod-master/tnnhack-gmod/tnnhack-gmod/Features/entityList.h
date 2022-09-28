#pragma once

#include "../SourceSDK/entity.h"
#include "../SourceSDK/math/qangle.h"
#include "../SourceSDK/Math/vector.h"
#include "../SourceSDK/usercmd.h"
#include "../interface.h"

class CEntityList
{
public:
	void Redraw() const;
	void CreateMove(float sample, CUserCMD* cmd) const;
};

extern CEntityList* g_pEntityList;
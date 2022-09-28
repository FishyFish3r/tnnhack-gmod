#pragma once
#include "../SourceSDK/entity.h"
#include "../SourceSDK/Math/Vector.h"

class CBones
{
public:
	void GetBonePosition(CBasePlayer* player, int idx, Vector3f* vector);
};

extern CBones* g_pBones;
#pragma once

#include "../SourceSDK/Math/vector.h"
#include "../SourceSDK/Math/matrix.h"
#include "../SourceSDK/entity.h"

class CEsp
{
public:
	void Snaplines(CBasePlayer* entity) const;
	void Box2d(CBasePlayer* entity) const;
	void PlayerInfo(CBasePlayer* entity, int idx) const;
	void Heading(CBasePlayer* entity) const;
	void Skeleton(CBasePlayer* entity) const;
};

extern CEsp* g_pEsp;
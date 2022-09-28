#include "aim.h"
#include "../SourceSDK/const.h"
#include "../utils.h"
#include "../interface.h"
#include "../sperma_cast.h"
#include "../var.h"
#include "../drawing.h"
#include "bones.h"

CAimbot* g_pAimbot = nullptr;

CAimbot::CAimbot()
{
	this->m_AimAngle = QAngle(0, 0, 0);
	this->m_BestEntity = nullptr;
	this->m_BestSearchValue = FLT_MAX;
}

void CAimbot::Rage(CUserCMD* cmd)
{
	static CHackVarBool* aim_silent = sperma_cast<CHackVarBool*>(g_pSettings->GetVar("aim_silent"));

	// get local player
	int localPlayer = Interfaces::Engine()->GetLocalPlayer();
	CBasePlayer* pLocal = sperma_cast<CBasePlayer*>(Interfaces::EntityList()->GetClientEntity(localPlayer));
	if (!pLocal || !pLocal->IsAlive())
	{
		return;
	}

	if (!this->m_BestEntity || !this->m_BestEntity->IsAlive())
	{
		return;
	}

	Vector3f myEye = pLocal->GetEyePos();
	Vector3f aimPos;
	int boneId;

	boneId = this->m_BestEntity->LookupBone("ValveBiped.Bip01_Head1");
	g_pBones->GetBonePosition(sperma_cast<CBasePlayer*>(this->m_BestEntity), boneId, &aimPos);
	aimPos.z += 10.f;

	this->m_AimAngle = g_pUtils->VectorAngles(aimPos - myEye); // calc aim angle

	if (aim_silent->m_Value)
	{
		cmd->viewangles = this->m_AimAngle;
	}
	else
	{
		cmd->viewangles = this->m_AimAngle;
		Interfaces::Engine()->SetViewAngles(this->m_AimAngle);
	}
}

void CAimbot::DrawCircleFov()
{
	static CHackVarFloat* aim_fov_radius = sperma_cast<CHackVarFloat*>(g_pSettings->GetVar("aim_fov_radius"));

	Vector2i screenSize;
	Interfaces::Engine()->GetScreenSize(screenSize.x, screenSize.y); // get screen size from engine

	g_pDrawing->DrawCircle(screenSize.x / 2, screenSize.y / 2, aim_fov_radius->m_Value + 70.f, 360);
}

void CAimbot::RCS(CUserCMD* cmd)
{

}

void CAimbot::TestEntity(CBaseEntity* a_Entity, bool fov)
{
	static CHackVarFloat* aim_fov_radius = sperma_cast<CHackVarFloat*>(g_pSettings->GetVar("aim_fov_radius"));

	// get local player
	int localPlayer = Interfaces::Engine()->GetLocalPlayer();
	CBasePlayer* pLocal = sperma_cast<CBasePlayer*>(Interfaces::EntityList()->GetClientEntity(localPlayer));
	if (a_Entity == FriendAddy())
	{
		return;
	}
	if (!pLocal || !pLocal->IsAlive())
	{
		return;
	}

	Vector3f myPos = pLocal->GetAbsOrigin();
	Vector3f targetPos = a_Entity->GetAbsOrigin();
	Vector3f delta = targetPos - myPos;

	// visibility check
	Ray_t ray;
	ray.mStart.x = myPos.x;
	ray.mStart.y = myPos.y;
	ray.mStart.z = myPos.z;
	ray.mStart.w = 0;

	ray.mDelta.x = delta.x;
	ray.mDelta.y = delta.y;
	ray.mDelta.z = delta.z;
	ray.mDelta.w = 0;

	ray.mExtents.x = 0;
	ray.mExtents.y = 0;
	ray.mExtents.z = 0;
	ray.mExtents.w = 0;

	ray.mStartOffset.x = 0;
	ray.mStartOffset.y = 0;
	ray.mStartOffset.z = 0;
	ray.mStartOffset.w = 0;

	ray.mIsRay = true;
	ray.mIsSwept = (delta.x * delta.x + delta.y * delta.y + delta.z * delta.z) != 0;

	CTraceFilterSingleEntity filter = CTraceFilterSingleEntity(pLocal);
	unsigned int mask = MASK_SOLID;
	trace_t result = {};

	Interfaces::EngineTrace()->TraceLine(ray, mask | 0x40000000, &filter, &result);
	if (result.mFraction < 0.94)
	{
		return;
	}

	if (!fov)
	{
		// sort by distance
		float dist = myPos.Distance3D(targetPos); // get distance between positions
		if (dist < this->m_BestSearchValue) // if distance is less than best distance
		{
			this->m_BestSearchValue = dist; // new best distance is target player distance
			this->m_BestEntity = a_Entity; // best entity is target player
		}
	}
	else
	{
		// sort by fov
		QAngle myAngle;
		Interfaces::Engine()->GetViewAngles(myAngle);

		float fov = g_pUtils->CalcFOV(myAngle, myPos, targetPos);
		if (fov < this->m_BestSearchValue && fov <= aim_fov_radius->m_Value)
		{
			this->m_BestSearchValue = fov;
			this->m_BestEntity = a_Entity;
		}
	}
}

CBaseEntity* CAimbot::FriendAddy()
{
	static CHackVarInt* friend_list_id = sperma_cast<CHackVarInt*>(g_pSettings->GetVar("friend_list_id"));
	return Interfaces::EntityList()->GetClientEntity(friend_list_id->m_Value);
}

void CAimbot::Begin()
{
	this->m_BestEntity = nullptr;
	this->m_BestSearchValue = FLT_MAX;
}
#pragma once

#include "../Math/vector.h"
#include "../Math/qangle.h"
#include "../Math/matrix.h"
#include "../entity.h"
#include "IMaterialSystem.h"
typedef int ButtonCode_t;

class model_t;
class SurfInfo;
class client_textmessage_t;
class CSentence;
class CAudioSource;

class IEngineClient
{
public:
	virtual int GetIntersectingSurfaces(const model_t* model, const Vector3f& vCenter, const float radius, const bool bOnlyVisibleSurfaces, SurfInfo* pInfos, const int nMaxInfos) = 0;
	virtual Vector3f GetLightForPoint(const Vector3f& pos, bool bClamp) = 0;
	virtual IMaterial* TraceLineAndMaterial(const Vector3f& start, const Vector3f& end, Vector3f& diffuseLightColor, Vector3f& baseColor) = 0;
	virtual const char* ParseFile(const char* data, char* token, int maxlen) = 0;
	virtual bool CopyLocalFile(const char* source, const char* destination) = 0;
	virtual void GetScreenSize(int& width, int& height) = 0;
	virtual void ServerCmd(const char* szCmdString, bool bReliable = true) = 0;
	virtual void ClientCmd(const char* szCmdString) = 0;
	virtual bool GetPlayerInfo(int ent_num, CPlayerInfo* pinfo) = 0;
	virtual int GetPlayerForUserID(int userID) = 0;
	virtual client_textmessage_t* TextMessageGet(const char* pName) = 0;
	virtual bool Con_IsVisible() = 0;
	virtual int GetLocalPlayer() = 0;
	virtual const model_t* LoadModel(const char* pName, bool bProp = false) = 0;
	virtual float Time() = 0;
	virtual float GetLastTimeStamp() = 0;
	virtual CSentence* GetSentence(CAudioSource* pAudioSource) = 0;
	virtual float GetSentenceLength(CAudioSource* pAudioSource) = 0;
	virtual bool IsStreaming(CAudioSource* pAudioSource) = 0;
	virtual void GetViewAngles(QAngle& va) = 0;
	virtual void SetViewAngles(QAngle& va) = 0;
	virtual int GetMaxClients() = 0;
	virtual	const char* Key_LookupBinding(const char* pBinding) = 0;
	virtual const char* Key_BindingForKey(ButtonCode_t code) = 0;
	virtual void StartKeyTrapMode() = 0;
	virtual bool CheckDoneKeyTrapping(ButtonCode_t& code) = 0;
	virtual bool IsInGame() = 0;
	virtual bool IsConnected() = 0;
	virtual bool IsDrawingLoadingImage() = 0;
	virtual void Con_NPrintf(int pos, const char* fmt, ...) = 0;
	virtual void Con_NXPrintf(const struct con_nprint_s* info, const char* fmt, ...) = 0;
	virtual int IsBoxVisible(const Vector3f& mins, const Vector3f& maxs) = 0;
	virtual int IsBoxInViewCluster(const Vector3f& mins, const Vector3f& maxs) = 0;
	virtual bool CullBox(const Vector3f& mins, const Vector3f& maxs) = 0;
	virtual void Sound_ExtraUpdate() = 0;
	virtual const char* GetGameDirectory() = 0;
	virtual const Matrix4x4f& WorldToScreenMatrix() = 0;
	virtual const Matrix4x4f& WorldToViewMatrix() = 0;
};
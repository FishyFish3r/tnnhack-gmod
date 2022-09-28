#pragma once
#include "Math/vector.h"
#include "Math/qangle.h"
#include "netvar.h"
#include "Math/matrix.h"

#define MAX_PLAYER_NAME_LENGTH 128
#define SIGNED_GUID_LEN 33
#define MAX_CUSTOM_FILES 4

typedef unsigned int CRC32_t;
typedef void* CBaseHandle;
typedef unsigned char byte;

class CPlayerInfo
{
public:
	char name[MAX_PLAYER_NAME_LENGTH];
private:
	char pad_00[0x10];
public:
	int	 userID;
	char guid[SIGNED_GUID_LEN];
	unsigned long friendsID;
	char friendsName[128];
	bool fakeplayer;
	bool ishltv;
	unsigned long customFiles[MAX_CUSTOM_FILES];
	unsigned char filesDownloaded;
private:
	byte pad_01[200];
};

class CBaseEntityNetworkable
{
private:
	virtual void*		__PAD00() = 0;
	virtual void*		__PAD01() = 0;
	virtual void*		__PAD02() = 0;
	virtual void*		__PAD03() = 0;
	virtual void*		__PAD04() = 0;
	virtual void*		__PAD05() = 0;
	virtual void*		__PAD06() = 0;
	virtual void*		__PAD07() = 0;
public:
	virtual bool		IsDormant() = 0;
	virtual int			GetIndex() = 0;
};

class CBaseEntityAnimating
{
private:
	virtual void* PAD_000() = 0;
	virtual void* PAD_001() = 0;
	virtual void* PAD_002() = 0;
	virtual void* PAD_003() = 0;
	virtual void* PAD_004() = 0;
	virtual void* PAD_005() = 0;
	virtual void* PAD_006() = 0;
	virtual void* PAD_007() = 0;
	virtual void* PAD_008() = 0;
	virtual void* PAD_009() = 0;
	virtual void* PAD_010() = 0;
	virtual void* PAD_011() = 0;
	virtual void* PAD_012() = 0;
	virtual void* PAD_013() = 0;
	virtual void* PAD_014() = 0;
	virtual void* PAD_015() = 0;
public:
	virtual bool SetupBones(Matrix3x4f* pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime) = 0;
private:
	virtual void* PAD_017() = 0;
	virtual void* PAD_018() = 0;
	virtual void* PAD_019() = 0;
	virtual void* PAD_020() = 0;
	virtual void* PAD_021() = 0;
	virtual void* PAD_022() = 0;
	virtual void* PAD_023() = 0;
	virtual void* PAD_024() = 0;
	virtual void* PAD_025() = 0;
	virtual void* PAD_026() = 0;
	virtual void* PAD_027() = 0;
	virtual void* PAD_028() = 0;
	virtual void* PAD_029() = 0;
	virtual void* PAD_030() = 0;
	virtual void* PAD_031() = 0;
	virtual void* PAD_032() = 0;
	virtual void* PAD_033() = 0;
	virtual void* PAD_034() = 0;
	virtual void* PAD_035() = 0;
	virtual void* PAD_036() = 0;
	virtual void* PAD_037() = 0;
	virtual void* PAD_038() = 0;
	virtual void* PAD_039() = 0;
	virtual void* PAD_040() = 0;
	virtual void* PAD_041() = 0;
	virtual void* PAD_042() = 0;
	virtual void* PAD_043() = 0;
	virtual void* PAD_044() = 0;
	virtual void* PAD_045() = 0;
	virtual void* PAD_046() = 0;
	virtual void* PAD_047() = 0;
	virtual void* PAD_048() = 0;
	virtual void* PAD_049() = 0;
	virtual void* PAD_050() = 0;
	virtual void* PAD_051() = 0;
	virtual void* PAD_052() = 0;
	virtual void* PAD_053() = 0;
	virtual void* PAD_054() = 0;
	virtual void* PAD_055() = 0;
	virtual void* PAD_056() = 0;
	virtual void* PAD_057() = 0;
	virtual void* PAD_058() = 0;
	virtual void* PAD_059() = 0;
	virtual void* PAD_060() = 0;
	virtual void* PAD_061() = 0;
	virtual void* PAD_062() = 0;
	virtual void* PAD_063() = 0;
	virtual void* PAD_064() = 0;
	virtual void* PAD_065() = 0;
	virtual void* PAD_066() = 0;
	virtual void* PAD_067() = 0;
	virtual void* PAD_068() = 0;
	virtual void* PAD_069() = 0;
	virtual void* PAD_070() = 0;
	virtual void* PAD_071() = 0;
	virtual void* PAD_072() = 0;
	virtual void* PAD_073() = 0;
	virtual void* PAD_074() = 0;
	virtual void* PAD_075() = 0;
	virtual void* PAD_076() = 0;
	virtual void* PAD_077() = 0;
	virtual void* PAD_078() = 0;
	virtual void* PAD_079() = 0;
	virtual void* PAD_080() = 0;
	virtual void* PAD_081() = 0;
	virtual void* PAD_082() = 0;
	virtual void* PAD_083() = 0;
	virtual void* PAD_084() = 0;
	virtual void* PAD_085() = 0;
	virtual void* PAD_086() = 0;
	virtual void* PAD_087() = 0;
	virtual void* PAD_088() = 0;
	virtual void* PAD_089() = 0;
	virtual void* PAD_090() = 0;
	virtual void* PAD_091() = 0;
	virtual void* PAD_092() = 0;
	virtual void* PAD_093() = 0;
	virtual void* PAD_094() = 0;
	virtual void* PAD_095() = 0;
};

typedef bool(__thiscall* tSetupBones)(Matrix3x4f* pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime);

class CBaseEntity		
{
public:
	int							GetHealth();
	int							GetMaxHealth();
	int							GetFlags();
	bool						IsAlive();
	Vector3f					GetEyePos();
	Vector3f					GetViewPunch();
	QAngle						GetAngle();
	int							GetIndex();
	bool						IsDormant();
	int							LookupBone(const char* name);

	CBaseEntityNetworkable*		GetNetworkable();
	CBaseEntityAnimating*		GetAnimating();
public:
	virtual void* SetRefEHandle(CBaseHandle const&) = 0;		// miss
	virtual void* GetRefHandle(void) = 0;
	virtual void* GetCollideable(void) = 0;					// ICollideable*
	virtual void* GetClientNetworkable(int entnum) = 0;		// IClientNetworkable*
	virtual void* GetClientRenderable() = 0;					// IClientRenderable*
	virtual void* GetIClientEntity() = 0;						// IClientEntity*
	virtual CBaseEntity* GetBaseEntity() = 0;
	virtual void* GetClientThinkable() = 0;					// IClientThinkable*
	virtual void* Release() = 0;
	virtual	const Vector3f& GetAbsOrigin() = 0;
	virtual	const QAngle& __GetAbsAngle() = 0;
	virtual void* GetMounth() = 0;
	virtual void* GetSoundSpatialization() = 0;
	virtual void* GetDataDescMap() = 0;
	virtual void* YouForgotToImplementOrDeclareClientClass() = 0;
	virtual ClientClass* GetClientClass() = 0;
	virtual void* GetPredDescMap() = 0;
	virtual void* FireBullets() = 0;
	virtual void* ModifyFireBulletsDamage() = 0;
	virtual void* ShouldDrawUnderwaterBulletBubbles() = 0;
	virtual void* ShouldDrawWaterImpacts() = 0;
	virtual void* HandleShotImpactingWater() = 0;
	virtual void* GetBeamTraceFilter() = 0;
	virtual void* DispatchTraceAttack() = 0;
	virtual void* TraceAttack() = 0;
	virtual void* DoImpactEffect() = 0;
	virtual void* MakeTracer() = 0;
	virtual void* GetTracerAttachment() = 0;
	virtual void* BloodColor() = 0;
	virtual void* GetTracerType() = 0;
	virtual void* Spawn() = 0;
	virtual void* SpawnClientEntity() = 0;
	virtual void* Precache() = 0;
	virtual void* Activate() = 0;
	virtual void* ParseMapData() = 0;
	virtual void* KeyValue1() = 0;
	virtual void* KeyValue2() = 0;
	virtual void* KeyValue3() = 0;
	virtual void* GetKeyValue() = 0;
	virtual void* Init() = 0;
	virtual void* GetIClientUnknown() = 0;
	virtual void* GetBaseAnimating() = 0;
	virtual void* SetClassname() = 0;
	virtual void* RecordToolMessage() = 0;
	virtual void* GetRenderOrigin() = 0;
	virtual void* GetRenderAngles() = 0;
	virtual void* GetObserverCamOrigin() = 0;
	virtual void* RenderableToWorldTransform() = 0;
	virtual void* IsTransparent() = 0;
	virtual void* UsesPowerOfTwoFrameBufferTexture() = 0;
	virtual void* UsesFullFrameBufferTexture() = 0;
	virtual void* IgnoresZBuffer() = 0;
	virtual void* GetModel() = 0;
	virtual void* DrawModel() = 0;
	virtual void* ComputeFxBlend() = 0;
	virtual void* GetFxBlend() = 0;
	virtual void* LODTest() = 0;
	virtual void* GetRenderBounds() = 0;
	virtual void* GetPVSNotifyInterface() = 0;
	virtual void* GetRenderBoundsWorldspace() = 0;
	virtual void* GetShadowRenderBounds() = 0;
	virtual void* GetColorModulation() = 0;
	virtual void* OnThreadedDrawSetup() = 0;
	virtual void* TestCollision() = 0;
	virtual void* TestHitboxes() = 0;
	virtual void* GetAttackDamageScale() = 0;
	virtual void* NotifyShouldTransmit() = 0;
	virtual void* PreDataUpdate() = 0;
	virtual void* PostDataUpdate() = 0;
	virtual void* OnDataUnchangedInPVS() = 0;
	virtual void* ValidateModelIndex() = 0;
	virtual void* SetDormant() = 0;
	virtual void* __IsDormant() = 0;
	virtual void* SetDestroyedOnRecreateEntities() = 0;
	virtual void* GetEFlags() = 0;
	virtual void* SetEFlags() = 0;
	virtual int entindex() = 0;
	virtual void* ReceiveMessage() = 0;
	virtual void* GetDataTableBasePtr() = 0;
	virtual void* ClientThink() = 0;
	virtual void* GetThinkHandle() = 0;
	virtual void* SetThinkHandle() = 0;
	virtual void* ShouldSavePhysics() = 0;
	virtual void* OnSave() = 0;
	virtual void* OnRestore() = 0;
	virtual void* ObjectCaps() = 0;
	virtual void* Save() = 0;
	virtual void* PAD_086() = 0;
	virtual void* PAD_087() = 0;
	virtual void* PAD_088() = 0;
	virtual void* PAD_089() = 0;
	virtual void* PAD_090() = 0;
	virtual void* PAD_091() = 0;
	virtual void* PAD_092() = 0;
	virtual void* PAD_093() = 0;
	virtual void* PAD_094() = 0;
	virtual void* PAD_095() = 0;
	virtual bool SetupBones(Matrix3x4f* pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime) = 0;
};

class CBasePlayer : public CBaseEntity
{
public:
};
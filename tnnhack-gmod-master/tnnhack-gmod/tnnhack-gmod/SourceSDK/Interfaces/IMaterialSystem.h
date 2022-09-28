#pragma once
#include "../Math/vector.h"
#include "../Math/qangle.h"

enum MaterialVarFlags_t
{
	MATERIAL_VAR_DEBUG = (1 << 0),
	MATERIAL_VAR_NO_DEBUG_OVERRIDE = (1 << 1),
	MATERIAL_VAR_NO_DRAW = (1 << 2),
	MATERIAL_VAR_USE_IN_FILLRATE_MODE = (1 << 3),

	MATERIAL_VAR_VERTEXCOLOR = (1 << 4),
	MATERIAL_VAR_VERTEXALPHA = (1 << 5),
	MATERIAL_VAR_SELFILLUM = (1 << 6),
	MATERIAL_VAR_ADDITIVE = (1 << 7),
	MATERIAL_VAR_ALPHATEST = (1 << 8),
	MATERIAL_VAR_MULTIPASS = (1 << 9),
	MATERIAL_VAR_ZNEARER = (1 << 10),
	MATERIAL_VAR_MODEL = (1 << 11),
	MATERIAL_VAR_FLAT = (1 << 12),
	MATERIAL_VAR_NOCULL = (1 << 13),
	MATERIAL_VAR_NOFOG = (1 << 14),
	MATERIAL_VAR_IGNOREZ = (1 << 15),
	MATERIAL_VAR_DECAL = (1 << 16),
	MATERIAL_VAR_ENVMAPSPHERE = (1 << 17),
	MATERIAL_VAR_NOALPHAMOD = (1 << 18),
	MATERIAL_VAR_ENVMAPCAMERASPACE = (1 << 19),
	MATERIAL_VAR_BASEALPHAENVMAPMASK = (1 << 20),
	MATERIAL_VAR_TRANSLUCENT = (1 << 21),
	MATERIAL_VAR_NORMALMAPALPHAENVMAPMASK = (1 << 22),
	MATERIAL_VAR_NEEDS_SOFTWARE_SKINNING = (1 << 23),
	MATERIAL_VAR_OPAQUETEXTURE = (1 << 24),
	MATERIAL_VAR_ENVMAPMODE = (1 << 25),
	MATERIAL_VAR_SUPPRESS_DECALS = (1 << 26),
	MATERIAL_VAR_HALFLAMBERT = (1 << 27),
	MATERIAL_VAR_WIREFRAME = (1 << 28),
	MATERIAL_VAR_ALLOWALPHATOCOVERAGE = (1 << 29),
	MATERIAL_VAR_IGNORE_ALPHA_MODULATION = (1 << 30),

	// NOTE: Only add flags here that either should be read from
	// .vmts or can be set directly from client code. Other, internal
	// flags should to into the flag enum in imaterialinternal.h
};

enum OverrideType_t
{
	OVERRIDE_NORMAL = 0,
	OVERRIDE_BUILD_SHADOWS,
	OVERRIDE_DEPTH_WRITE,
};

struct ModelRenderInfo_t
{
	Vector3f origin;
	QAngle angles;
	void* pRenderable;
	void* pModel;
	const void* pModelToWorld;
	const void* pLightingOffset;
	const void* pLightingOrigin;
	int flags;
	int entity_index;
	int skin;
	int body;
	int hitboxset;
	void* instance;
};

class IMaterialVar
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
public:
	virtual void SetVecValue(float x, float y) = 0;
	virtual void SetVecValue(float x, float y, float z) = 0;
	virtual void SetVecValue(float x, float y, float z, float w) = 0;
	virtual void SetVecComponentValue(float val, int comp) = 0;
};

class IMaterial
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
public:
	virtual IMaterialVar* FindVar(const char* name, bool* found, bool complain = true) = 0;
	virtual void IncrementReferenceCount() = 0;
	virtual void DecrementReferenceCount() = 0;
public:
	virtual void* PAD_014() = 0;
	virtual void* PAD_015() = 0;
	virtual void* PAD_016() = 0;
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
public:
	virtual void AlphaModulate(float a) = 0;
	virtual void ColorModulate(float r, float g, float b) = 0;
	virtual void SetMaterialVarFlag(int flag, bool on) = 0;
};

class IModelRender
{
private:
	virtual void* PAD_000() = 0;
public:
	virtual void ForcedMaterialOverride(IMaterial* newMaterial, int nOverrideType = OVERRIDE_NORMAL) = 0;
private:
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
	virtual void* PAD_016() = 0;
	virtual void* PAD_017() = 0;
	virtual void* PAD_018() = 0;
	virtual void* PAD_019() = 0;
public:
	virtual void DrawModelExecute(const void* state, const ModelRenderInfo_t& pInfo, void* pCustomBoneToWorld) = 0;
};

class IModelInfoClient
{
private:
	virtual void* PAD_000() = 0;
public:
	virtual void* GetModel(int id) = 0;
	virtual const char* GetModelName(const void* model) const = 0;
};

// These are given to FindMaterial to reference the texture groups that show up on the 
#define TEXTURE_GROUP_LIGHTMAP                        "Lightmaps"
#define TEXTURE_GROUP_WORLD                            "World textures"
#define TEXTURE_GROUP_MODEL                            "Model textures"
#define TEXTURE_GROUP_VGUI                            "VGUI textures"
#define TEXTURE_GROUP_PARTICLE                        "Particle textures"
#define TEXTURE_GROUP_DECAL                            "Decal textures"
#define TEXTURE_GROUP_SKYBOX                        "SkyBox textures"
#define TEXTURE_GROUP_CLIENT_EFFECTS                "ClientEffect textures"
#define TEXTURE_GROUP_OTHER                            "Other textures"
#define TEXTURE_GROUP_PRECACHED                        "Precached"                // TODO: assign texture groups to the precached materials
#define TEXTURE_GROUP_CUBE_MAP                        "CubeMap textures"
#define TEXTURE_GROUP_RENDER_TARGET                    "RenderTargets"
#define TEXTURE_GROUP_UNACCOUNTED                    "Unaccounted textures"    // Textures that weren't assigned a texture group.
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER            "Static Vertex"
#define TEXTURE_GROUP_STATIC_INDEX_BUFFER            "Static Indices"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_DISP        "Displacement Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_COLOR    "Lighting Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_WORLD    "World Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_MODELS    "Model Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_OTHER    "Other Verts"
#define TEXTURE_GROUP_DYNAMIC_INDEX_BUFFER            "Dynamic Indices"
#define TEXTURE_GROUP_DYNAMIC_VERTEX_BUFFER            "Dynamic Verts"
#define TEXTURE_GROUP_DEPTH_BUFFER                    "DepthBuffer"
#define TEXTURE_GROUP_VIEW_MODEL                    "ViewModel"

class IMaterialSystem
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
	virtual void* PAD_016() = 0;
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
public:
	virtual IMaterial* FindMaterial(const char* materialName, const char*, bool* pFound = nullptr, bool bComplain = true) = 0;
};
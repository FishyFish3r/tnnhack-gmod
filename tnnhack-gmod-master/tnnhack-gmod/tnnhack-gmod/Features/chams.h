#pragma once
#include "../SourceSDK/entity.h"
#include "../SourceSDK/Interfaces/IMaterialSystem.h"

class CChamsMaterial
{
private:
	IMaterial* m_InternalPtr;
	std::string m_ColorVariableName;
	std::string m_Name;
public:
	CChamsMaterial();
	~CChamsMaterial();
	CChamsMaterial(std::string a_Name, std::string a_ColorVariableName);

	void SetColor(float a_Red, float a_Green, float a_Blue, float a_Alpha = 1.f);
	void ApplyMaterial();
	void DisableZ();
	void EnableZ();
};

class CChams
{
private:
	CChamsMaterial* m_WireframeMaterial;
public:
	CChams();
	~CChams();
	void DrawModel(void* state, ModelRenderInfo_t& info, void* bones);
};

extern CChams* g_pChams;
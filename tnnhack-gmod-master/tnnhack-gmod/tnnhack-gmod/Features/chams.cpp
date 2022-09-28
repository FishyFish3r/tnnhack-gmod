#include "chams.h"
#include "../hooks.h"
#include "../VMTHooks.h"
#include "../interface.h"
#include "../var.h"

CChams* g_pChams = nullptr;

CChamsMaterial::CChamsMaterial()
{
	this->m_ColorVariableName = "";
	this->m_InternalPtr = nullptr;
	this->m_Name = "";
}

CChamsMaterial::~CChamsMaterial()
{
	if (this->m_InternalPtr)
	{
		this->m_InternalPtr->DecrementReferenceCount();
	}
}

CChamsMaterial::CChamsMaterial(std::string a_Name, std::string a_ColorVariableName)
{
	this->m_Name = a_Name;
	this->m_ColorVariableName = a_ColorVariableName;

	this->m_InternalPtr = Interfaces::MaterialSystem()->FindMaterial(this->m_Name.c_str(), TEXTURE_GROUP_OTHER);
	if (this->m_InternalPtr)
	{
		this->m_InternalPtr->IncrementReferenceCount();
	}
}

void CChamsMaterial::SetColor(float a_Red, float a_Green, float a_Blue, float a_Alpha)
{
	if (this->m_InternalPtr)
	{
		this->m_InternalPtr->FindVar(this->m_ColorVariableName.c_str(), nullptr)->SetVecValue(a_Red, a_Green, a_Blue, a_Alpha);
	}
}

void CChamsMaterial::ApplyMaterial()
{
	Interfaces::ModelRender()->ForcedMaterialOverride(this->m_InternalPtr);
}

void CChamsMaterial::DisableZ()
{
	if (this->m_InternalPtr)
	{
		this->m_InternalPtr->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);
	}
}

void CChamsMaterial::EnableZ()
{
	if (this->m_InternalPtr)
	{
		this->m_InternalPtr->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
	}
}

CChams::CChams()
{
	this->m_WireframeMaterial = new CChamsMaterial("debug/debugspritewireframe", "$color2");
}

CChams::~CChams()
{
	delete this->m_WireframeMaterial;
}

void CChams::DrawModel(void* state, ModelRenderInfo_t& info, void* bones)
{
	static Hooks::tDrawModelExecute oDrawModelExecute = sperma_cast<Hooks::tDrawModelExecute>(g_pVmtHook->GetOriginalFunc("DrawModelExecute"));

	static CHackVarBool* chams_enable = sperma_cast<CHackVarBool*>(g_pSettings->GetVar("chams_enable"));
	static CHackVarBool* chams_walls = sperma_cast<CHackVarBool*>(g_pSettings->GetVar("chams_walls"));
	static CHackVarColor* chams_color_visible = sperma_cast<CHackVarColor*>(g_pSettings->GetVar("chams_color_visible"));
	static CHackVarColor* chams_color_invisible = sperma_cast<CHackVarColor*>(g_pSettings->GetVar("chams_color_invisible"));
	static CHackVarBool* chams_colored = sperma_cast<CHackVarBool*>(g_pSettings->GetVar("chams_colored"));

	if (chams_enable->m_Value)
	{
		this->m_WireframeMaterial->ApplyMaterial();

		if (chams_colored->m_Value)
		{
			if (chams_walls->m_Value)
			{
				this->m_WireframeMaterial->DisableZ();
				this->m_WireframeMaterial->SetColor(chams_color_invisible->m_Red / 255.f, chams_color_invisible->m_Green / 255.f, chams_color_invisible->m_Blue / 255.f);

				oDrawModelExecute(Interfaces::ModelRender(), state, info, bones);

				this->m_WireframeMaterial->EnableZ();
			}
			
			this->m_WireframeMaterial->SetColor(chams_color_visible->m_Red / 255.f, chams_color_visible->m_Green / 255.f, chams_color_visible->m_Blue / 255.f);
		}
		else
		{
			this->m_WireframeMaterial->SetColor(1.f, 1.f, 1.f, 1.f);

			if (chams_walls->m_Value)
			{
				this->m_WireframeMaterial->DisableZ();

				oDrawModelExecute(Interfaces::ModelRender(), state, info, bones);

				this->m_WireframeMaterial->EnableZ();
			}
		}
	}

	oDrawModelExecute(Interfaces::ModelRender(), state, info, bones);
}
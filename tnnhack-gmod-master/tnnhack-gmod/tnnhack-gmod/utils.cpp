#include <stdexcept>
#define _USE_MATH_DEFINES
#include <math.h>
#include <string>
#include "utils.h"
#include "SourceSDK/iface.h"
#include "SourceSDK/Math/matrix.h"
#include "interface.h"
#include "sperma_cast.h"
#include "SourceSDK/entity.h"
#include "var.h"

CUtils* g_pUtils = nullptr;
extern bool g_Keys[256];

CUtils::CUtils()
{
	this->m_Modules = {};
}

CUtils::~CUtils()
{
	this->ClearModules();
}

DWORD CUtils::FindPattern(DWORD a_Start, DWORD a_End, const char* a_Pattern, const char* a_Mask) const
{
	unsigned len = strlen(a_Mask);

	for (DWORD addy = a_Start; addy < a_End; addy++)
	{
		bool found = true;

		for (unsigned c = 0; c < len; c++)
		{
			if (a_Mask[c] != 'x')
			{
				continue;
			}

			if (a_Pattern[c] != *sperma_cast<char*>(addy + c))
			{
				found = false;
				break;
			}
		}

		if (found)
		{
			return addy;
		}
	}

	return NULL;
}

CModule* CUtils::GetModule(std::string a_Name)
{
	bool exist = true;
	CModule* mod = nullptr;

	try 
	{
		mod = this->m_Modules.at(a_Name);
	}
	catch (std::out_of_range e)
	{
		exist = false;
	}

	if (!mod)
	{
		exist = false;
	}

	if (!exist)
	{
		mod = new CModule(a_Name);
		this->m_Modules.insert(std::make_pair(a_Name, mod));
		return mod;
	}
	else
	{
		return mod;
	}

	return nullptr;
}

void CUtils::ClearModules()
{
	for (std::pair<std::string, CModule*> p : this->m_Modules)
	{
		delete p.second;
	}

	this->m_Modules.clear();
}


void* CUtils::GetInterface(std::string a_ModuleName, std::string a_InterfaceName) const
{
	HMODULE moduleHandle = GetModuleHandleA(a_ModuleName.c_str());
	if (!moduleHandle)
	{
		return nullptr;
	}

	tCreateInterface pfnCreateInterface = sperma_cast<tCreateInterface>(GetProcAddress(moduleHandle, CREATEITF_EXTERN));
	if (!pfnCreateInterface)
	{
		return nullptr;
	}

	void* r = (pfnCreateInterface)(a_InterfaceName.c_str(), nullptr);
	return r;
}

/*
bool CUtils::GetD3D9Device(HWND a_Window, void** a_pTable, unsigned a_Size) const
{
	if (!a_pTable)
		return false;

	IDirect3D9* pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (!pD3D)
		return false;

	IDirect3DDevice9* pDummyDevice = nullptr;

	D3DPRESENT_PARAMETERS d3dpp = {};
	d3dpp.Windowed = false;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = a_Window;

	HRESULT dummyDevCreated = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3dpp.hDeviceWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pDummyDevice);

	if (dummyDevCreated != S_OK)
	{
		d3dpp.Windowed = !d3dpp.Windowed;
		HRESULT dummyDevCreated = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3dpp.hDeviceWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pDummyDevice);

		if (dummyDevCreated != S_OK)
		{
			pD3D->Release();
			return false;
		}
	}

	memcpy(a_pTable, *sperma_cast<void***>(pDummyDevice), a_Size);

	pDummyDevice->Release();
	pD3D->Release();

	return true;
}
*/

bool CUtils::World2Screen(Vector3f pos, Vector2i& screen) const
{
	Matrix4x4f viewMatrix = Interfaces::Engine()->WorldToScreenMatrix();

	Vector2i screenSize;
	Interfaces::Engine()->GetScreenSize(screenSize.x, screenSize.y);

	Vector4f clip;
	clip.x = pos.x * viewMatrix[0][0] + pos.y * viewMatrix[0][1] + pos.z * viewMatrix[0][2] + viewMatrix[0][3];
	clip.y = pos.x * viewMatrix[1][0] + pos.y * viewMatrix[1][1] + pos.z * viewMatrix[1][2] + viewMatrix[1][3];
	clip.z = pos.x * viewMatrix[2][0] + pos.y * viewMatrix[2][1] + pos.z * viewMatrix[2][2] + viewMatrix[2][3];
	clip.w = pos.x * viewMatrix[3][0] + pos.y * viewMatrix[3][1] + pos.z * viewMatrix[3][2] + viewMatrix[3][3];

	if (clip.w < 0.1f)
	{
		return false;
	}

	Vector3f NDC;
	NDC.x = clip.x / clip.w;
	NDC.y = clip.y / clip.w;
	NDC.z = clip.z / clip.w;

	screen.x = (screenSize.x / 2 * NDC.x) + (NDC.x + screenSize.x / 2);
	screen.y = -(screenSize.y / 2 * NDC.y) + (NDC.y + screenSize.y / 2);

	return true;
}

QAngle CUtils::VectorAngles(Vector3f diff) const
{
	float tmp, yaw, pitch;

	if (diff.y == 0 && diff.x == 0)
	{
		yaw = 0;
		if (diff.z > 0)
			pitch = 270;
		else
			pitch = 90;
	}
	else
	{
		yaw = (atan2(diff.y, diff.x) * 180 / M_PI);
		if (yaw < 0)
			yaw += 360;

		tmp = sqrt(diff.x * diff.x + diff.y * diff.y);
		pitch = (atan2(-diff.z, tmp) * 180 / M_PI);
		if (pitch < 0)
			pitch += 360;
	}

	while (pitch < -89) { pitch += 180; yaw += 180; }
	while (pitch > 89) { pitch -= 180; yaw += 180; }
	while (yaw < -180) { yaw += 360; }
	while (yaw > 180) { yaw -= 360; }

	return QAngle(pitch, yaw, 0);
}

void CUtils::AngleVectors(QAngle angle, Vector3f* forward, Vector3f* right, Vector3f* up) const
{
	float angles;
	float sr, sp, sy, cr, cp, cy;

	angles = angle.yaw * (M_PI * 2 / 360);
	sy = sin(angles);
	cy = cos(angles);
	angles = angle.pitch * (M_PI * 2 / 360);
	sp = sin(angles);
	cp = cos(angles);
	angles = angle.roll * (M_PI * 2 / 360);
	sr = sin(angles);
	cr = cos(angles);

	if (forward)
	{
		forward->x = cp * cy;
		forward->y = cp * sy;
		forward->z = -sp;
	}
	if (right)
	{
		right->x = (-1 * sr * sp * cy + -1 * cr * -sy);
		right->y = (-1 * sr * sp * sy + -1 * cr * cy);
		right->z = -1 * sr * cp;
	}
	if (up)
	{
		up->x = (cr * sp * cy + -sr * -sy);
		up->y = (cr * sp * sy + -sr * cy);
		up->z = cr * cp;
	}
}

float CUtils::CalcFOV(QAngle myAngle, Vector3f myPos, Vector3f target) const
{
	Vector3f diff = target - myPos;

	QAngle angleToTarget = this->VectorAngles(diff);
	QAngle angleDiff = (myAngle - angleToTarget).Normalize();

	return sqrtf(powf(angleDiff.pitch, 2.0f) + powf(angleDiff.yaw, 2.0f));
}
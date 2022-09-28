#pragma once

#include <map>
#include "module.h"
#include "SourceSDK/Math/vector.h"
#include "SourceSDK/Math/qangle.h"

class CUtils
{
private:
	std::map<std::string, CModule*> m_Modules;
public:
	CUtils();
	~CUtils();

	DWORD		FindPattern(DWORD a_Start, DWORD a_End, const char* a_Pattern, const char* a_Mask) const; // find byte pattern in memory

	CModule*	GetModule(std::string a_Name);
	void		ClearModules();

	
	void*		GetInterface(std::string a_ModuleName, std::string a_InterfaceName) const; // gets an interface from given module using CreateInterface with given name

	// bool		GetD3D9Device(HWND a_Window, void** a_pTable, unsigned a_Size) const; // generates a d3d9 device and outputs it's VMT to a_pTable

	bool		World2Screen(Vector3f pos, Vector2i& screen) const; // translate world coords to screen coords
	QAngle		VectorAngles(Vector3f diff) const; // get angle from a vector delta
	void		AngleVectors(QAngle angle, Vector3f* forward, Vector3f* right, Vector3f* up) const; // get forward, right and up directions from an angle
	float		CalcFOV(QAngle myAngle, Vector3f myPos, Vector3f target) const;
};

extern CUtils* g_pUtils;
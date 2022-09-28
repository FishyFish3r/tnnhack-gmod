#pragma once

#include <map>
#include <string>
#include "SourceSDK/Interfaces/ISurface.h"
#include "SourceSDK/Math/vector.h"

class CDrawing
{
private:
	int m_CurRed, m_CurGreen, m_CurBlue, m_CurAlpha;

	HFont m_CurFont;

	std::map<std::string, HFont> m_Fonts;
public:
	CDrawing();
	~CDrawing();

	void CreateFont(std::string a_Name, std::string a_BaseFont, int a_Height, int a_Boldness, int a_Flags);

	void PushColor(int a_Red, int a_Green, int a_Blue, int a_Alpha = 255);
	void PushFont(std::string a_FontName);

	void DrawLine(float a_X1, float a_Y1, float a_X2, float a_Y2) const;

	void DrawFilledRect(float a_X1, float a_Y1, float a_X2, float a_Y2) const;
	void DrawBorderedRect(float a_X1, float a_Y1, float a_X2, float a_Y2) const;
	void DrawBorderedRectOutline(float a_X1, float a_Y1, float a_X2, float a_Y2);
	void DrawCircle(float a_X, float a_Y, float a_Radius, int a_Segments = 360) const;

	void DrawString(float a_X, float a_Y, const char* a_FMT, ...) const;
	void DrawStringW(float a_X, float a_Y, const wchar_t* a_WFMT, ...) const;

	Vector2i GetTextSize(const char* a_Text) const;
	Vector2i GetTextSizeW(const wchar_t* a_WText) const;
};

extern CDrawing* g_pDrawing;
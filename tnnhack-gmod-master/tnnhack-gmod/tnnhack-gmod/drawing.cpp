#include <stdexcept>
#include "drawing.h"
#include "interface.h"

CDrawing* g_pDrawing = nullptr;

CDrawing::CDrawing()
{
	this->m_CurAlpha = this->m_CurBlue = this->m_CurGreen = this->m_CurRed = 255;
	this->m_CurFont = NULL;

	this->m_Fonts = {};
}

CDrawing::~CDrawing()
{
	this->m_Fonts.clear();
}

void CDrawing::CreateFont(std::string a_Name, std::string a_BaseFont, int a_Height, int a_Boldness, int a_Flags)
{
	HFont f = NULL;

	try
	{
		f = this->m_Fonts.at(a_Name);
	}
	catch (std::out_of_range e)
	{
		f = NULL;
	}

	if (f)
	{
		return;
	}

	HFont newFont = Interfaces::Surface()->CreateFont();
	Interfaces::Surface()->SetFontGlyphSet(newFont, a_BaseFont.c_str(), a_Height, a_Boldness, 0, 0, a_Flags, 0, 0);

	this->m_Fonts.insert(std::make_pair(a_Name, newFont));
}

void CDrawing::PushColor(int a_Red, int a_Green, int a_Blue, int a_Alpha)
{
	this->m_CurRed = a_Red;
	this->m_CurBlue = a_Blue;
	this->m_CurGreen = a_Green;
	this->m_CurAlpha = a_Alpha;

	Interfaces::Surface()->DrawSetColor(this->m_CurRed, this->m_CurGreen, this->m_CurBlue, this->m_CurAlpha);
	Interfaces::Surface()->DrawSetTextColor(this->m_CurRed, this->m_CurGreen, this->m_CurBlue, this->m_CurAlpha);
}

void CDrawing::PushFont(std::string a_FontName)
{
	HFont f = NULL;

	try
	{
		f = this->m_Fonts.at(a_FontName);
	}
	catch(std::out_of_range e)
	{
		f = NULL;
	}

	if (!f)
	{
		return;
	}
	
	this->m_CurFont = f;
	Interfaces::Surface()->DrawSetTextFont(this->m_CurFont);
}

void CDrawing::DrawLine(float a_X1, float a_Y1, float a_X2, float a_Y2) const
{
	Interfaces::Surface()->DrawLine(a_X1, a_Y1, a_X2, a_Y2);
}

void CDrawing::DrawFilledRect(float a_X1, float a_Y1, float a_X2, float a_Y2) const
{
	Interfaces::Surface()->DrawFilledRect(a_X1, a_Y1, a_X2, a_Y2);
}

void CDrawing::DrawBorderedRect(float a_X1, float a_Y1, float a_X2, float a_Y2) const
{
	Interfaces::Surface()->DrawLine(a_X1, a_Y1, a_X2, a_Y1);
	Interfaces::Surface()->DrawLine(a_X2, a_Y1, a_X2, a_Y2);
	Interfaces::Surface()->DrawLine(a_X2, a_Y2, a_X1, a_Y2);
	Interfaces::Surface()->DrawLine(a_X1, a_Y2, a_X1, a_Y1);
}

void CDrawing::DrawBorderedRectOutline(float a_X1, float a_Y1, float a_X2, float a_Y2)
{
	int oldR = this->m_CurRed;
	int oldG = this->m_CurGreen;
	int oldB = this->m_CurBlue;
	int oldA = this->m_CurAlpha;

	this->DrawBorderedRect(a_X1, a_Y1, a_X2, a_Y1);

	this->PushColor(0, 0, 0, 255);
	this->DrawBorderedRect(a_X1 + 1.f, a_Y1 + 1.f, a_X2 - 1.f, a_Y2 - 1.f);
	this->DrawBorderedRect(a_X1 - 1.f, a_Y1 - 1.f, a_X2 + 1.f, a_Y2 + 1.f);
	this->PushColor(oldR, oldG, oldB, oldA);
}

void CDrawing::DrawCircle(float a_X, float a_Y, float a_Radius, int a_Segments) const
{
	Interfaces::Surface()->DrawOutlinedCircle(a_X, a_Y, a_Radius, a_Segments);
}

void CDrawing::DrawString(float a_X, float a_Y, const char* a_FMT, ...) const
{
	char fmtBuf[1337];
	va_list varargs;

	va_start(varargs, a_FMT);
	vsprintf(fmtBuf, a_FMT, varargs);
	va_end(varargs);

	wchar_t wbuf[1337];
	wsprintfW(wbuf, L"%S", fmtBuf);

	Interfaces::Surface()->DrawSetTextPos(a_X, a_Y);
	Interfaces::Surface()->DrawPrintText(wbuf, lstrlenW(wbuf));
}

void CDrawing::DrawStringW(float a_X, float a_Y, const wchar_t* a_WFMT, ...) const
{
	wchar_t fmtBuf[1337];
	va_list varargs;

	va_start(varargs, a_WFMT);
	wvsprintfW(fmtBuf, a_WFMT, varargs);
	va_end(varargs);

	Interfaces::Surface()->DrawSetTextPos(a_X, a_Y);
	Interfaces::Surface()->DrawPrintText(fmtBuf, lstrlenW(fmtBuf));
}

Vector2i CDrawing::GetTextSize(const char* a_Text) const
{
	Vector2i r;

	wchar_t wbuf[1337];
	wsprintfW(wbuf, L"%S", a_Text);
	Interfaces::Surface()->GetTextSize(this->m_CurFont, wbuf, r.x, r.y);

	return r;
}

Vector2i CDrawing::GetTextSizeW(const wchar_t* a_WText) const
{
	Vector2i r;

	Interfaces::Surface()->GetTextSize(this->m_CurFont, a_WText, r.x, r.y);

	return r;
}
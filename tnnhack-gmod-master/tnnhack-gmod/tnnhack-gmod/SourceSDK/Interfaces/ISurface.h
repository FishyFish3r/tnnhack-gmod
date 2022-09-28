#pragma once

#include <windows.h>
#include "../color.h"

#ifdef CreateFont
#undef CreateFont
#endif

#ifdef PlaySound
#undef PlaySound
#endif

typedef unsigned long HCursor;
typedef unsigned long HTexture;
typedef unsigned long HFont;

typedef unsigned int VPANEL;

enum EFontFlags
{
	FONTFLAG_NONE,
	FONTFLAG_ITALIC = 0x001,
	FONTFLAG_UNDERLINE = 0x002,
	FONTFLAG_STRIKEOUT = 0x004,
	FONTFLAG_SYMBOL = 0x008,
	FONTFLAG_ANTIALIAS = 0x010,
	FONTFLAG_GAUSSIANBLUR = 0x020,
	FONTFLAG_ROTARY = 0x040,
	FONTFLAG_DROPSHADOW = 0x080,
	FONTFLAG_ADDITIVE = 0x100,
	FONTFLAG_OUTLINE = 0x200,
	FONTFLAG_CUSTOM = 0x400,
	FONTFLAG_BITMAP = 0x800,
};

enum FontDrawType_t
{
	FONT_DRAW_DEFAULT = 0,
	FONT_DRAW_NONADDITIVE,
	FONT_DRAW_ADDITIVE,
	FONT_DRAW_TYPE_COUNT = 2,
};

class ISurface
{
private:
	virtual void* Connect() = 0;
	virtual void* Disconnect() = 0;
	virtual void* QueryInterface() = 0;
	virtual void* Init() = 0;
	virtual void* Shutdown() = 0;
	virtual void* RunFrame() = 0;
	virtual void* GetEmbeddedPanel() = 0;
	virtual void* SetEmbeddedPanel() = 0;
public:
	virtual void PushMakeCurrent(VPANEL panel, bool useInsets) = 0;
	virtual void PopMakeCurrent(VPANEL panel) = 0;
	virtual void DrawSetColor(int r, int g, int b, int a = 255) = 0;
	virtual void DrawSetColor(SourceColor c) = 0;
	virtual void DrawFilledRect(int x1, int y1, int x2, int y2) = 0;
private:
	virtual void* DrawFilledRectArray() = 0;
public:
	virtual void DrawOutlinedRect(int x1, int y1, int x2, int y2) = 0;
	virtual void DrawLine(int x1, int y1, int x2, int y2) = 0;
private:
	virtual void* DrawPolyLine() = 0;
public:
	virtual void DrawSetTextFont(HFont font) = 0;
	virtual void DrawSetTextColor(int r, int g, int b, int a = 255) = 0;
	virtual void DrawSetTextColor(SourceColor c) = 0;
	virtual void DrawSetTextPos(int x, int y) = 0;
	virtual void DrawGetTextPos(int& x, int& y) = 0;
	virtual void DrawPrintText(const wchar_t* stri, int textLen, FontDrawType_t drawType = FONT_DRAW_DEFAULT) = 0;
	virtual void DrawUnicodeChar(wchar_t wch, FontDrawType_t drawType = FONT_DRAW_DEFAULT) = 0;
	virtual void DrawFlushText() = 0;
private:
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
public:
	virtual HFont CreateFont() = 0;
	virtual void SetFontGlyphSet(HFont font, const char* baseFont, int tall, int weight, int blur, int scanlines, int flags, int unk01, int unk02) = 0;
private:
	virtual void* PAD_068() = 0;
	virtual void* PAD_069() = 0;
	virtual void* PAD_070() = 0;
	virtual void* PAD_071() = 0;
	virtual void* PAD_072() = 0;
	virtual void* PAD_073() = 0;
	virtual void* PAD_074() = 0;
	virtual void* PAD_075() = 0;
public:
	virtual void GetTextSize(HFont font, const wchar_t* text, int& width, int& height) = 0;
private:
	virtual void* PAD_077() = 0;
	virtual void* PAD_078() = 0;
public:
	virtual void PlaySound(const char* filename) = 0;
private:
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
	virtual void* PAD_096() = 0;
	virtual void* PAD_097() = 0;
	virtual void* PAD_098() = 0;
	virtual void* PAD_099() = 0;
public:
	virtual void DrawOutlinedCircle(int x, int y, int radius, int segments) = 0;
};
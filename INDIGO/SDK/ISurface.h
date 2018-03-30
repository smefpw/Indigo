#pragma once

#include "SDK.h"

#ifdef PlaySound
#undef PlaySound
#endif

namespace SDK
{
	namespace TABLE
	{
		namespace ISurface
		{
			enum
			{
				DrawSetColor1 = 14,
				DrawSetColor2 = 15,
				DrawFilledRect = 16,
				DrawOutlinedRect = 18,
				DrawLine = 19,
				DrawSetTextFont = 23,
				DrawSetTextColor = 24,
				DrawSetTextPos = 26,
				DrawPrintText = 28,
				FontCreate = 71,
				SetFontGlyphSet = 72,
				GetTextSize = 79,
				PlaySound = 82
			};
		}
	}

	enum FontDrawType_t
	{
		FONT_DRAW_DEFAULT = 0,
		FONT_DRAW_NONADDITIVE,
		FONT_DRAW_ADDITIVE,
		FONT_DRAW_TYPE_COUNT = 2,
	};

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

	struct Vertex_t
	{
		Vector2D m_Position;
		Vector2D m_TexCoord;

		Vertex_t()
		{
		}

		Vertex_t(const Vector2D& pos, const Vector2D& coord = Vector2D(0, 0))
		{
			m_Position = pos;
			m_TexCoord = coord;
		}

		void Init(const Vector2D& pos, const Vector2D& coord = Vector2D(0, 0))
		{
			m_Position = pos;
			m_TexCoord = coord;
		}
	};

	typedef Vertex_t FontVertex_t;

	class ISurface
	{
	public:
		void DrawSetColor(Color col)
		{
			typedef void(__thiscall* oDrawSetColor)(PVOID, Color);
			return GetMethod<oDrawSetColor>(this, 14)(this, col);
		}

		void DrawSetColor(int r, int g, int b, int a)
		{
			typedef void(__thiscall* OriginalFn)(PVOID, int, int, int, int);
			GetMethod<OriginalFn>(this, 15)(this, r, g, b, a);
		}

		void DrawFilledRect(int x0, int y0, int x1, int y1)
		{
			typedef void(__thiscall* OriginalFn)(PVOID, int, int, int, int);
			GetMethod<OriginalFn>(this, 16)(this, x0, y0, x1, y1);
		}

		void DrawOutlinedRect(int x0, int y0, int x1, int y1)
		{
			typedef void(__thiscall* OriginalFn)(PVOID, int, int, int, int);
			GetMethod<OriginalFn>(this, 18)(this, x0, y0, x1, y1);
		}

		void DrawLine(int x0, int y0, int x1, int y1)
		{
			typedef void(__thiscall* OriginalFn)(PVOID, int, int, int, int);
			GetMethod<OriginalFn>(this, 19)(this, x0, y0, x1, y1);
		}

		void DrawPolyLine(int* x, int* y, int count)
		{
			typedef void(__thiscall* OriginalFn)(PVOID, int*, int*, int);
			GetMethod<OriginalFn>(this, 19 + 1)(this, x, y, count);
		}

		void DrawSetTextFont(unsigned long font)
		{
			typedef void(__thiscall* OriginalFn)(PVOID, unsigned long);
			GetMethod<OriginalFn>(this, 23)(this, font);
		}

		void DrawSetTextColor(int r, int g, int b, int a)
		{
			typedef void(__thiscall* OriginalFn)(PVOID, int, int, int, int);
			GetMethod<OriginalFn>(this, 25)(this, r, g, b, a);
		}

		void DrawSetTextColor(Color col)
		{
			typedef void(__thiscall* oDrawSetTextColor)(PVOID, Color);
			return GetMethod<oDrawSetTextColor>(this, 24)(this, col);
		}

		void DrawSetTextPos(int x, int y)
		{
			typedef void(__thiscall* OriginalFn)(PVOID, int, int);
			GetMethod<OriginalFn>(this, 26)(this, x, y);
		}

		void DrawPrintText(const wchar_t* text, int textLen)
		{
			typedef void(__thiscall* OriginalFn)(PVOID, const wchar_t*, int, int);
			GetMethod<OriginalFn>(this, 28)(this, text, textLen, 0);
		}

		void DrawSetTexture(int textureID)
		{
			typedef void(__thiscall* oDrawSetTextColor)(PVOID, int);
			return GetMethod<oDrawSetTextColor>(this, 38)(this, textureID);
		}

		void DrawTexturedRect(int x0, int y0, int x1, int y1)
		{
			typedef void(__thiscall* oDrawSetTextColor)(PVOID, int, int, int, int);
			return GetMethod<oDrawSetTextColor>(this, 37 + 4)(this, x0, y0, x1, y1);
		}

		bool IsTextureIDValid(int textureID)
		{
			typedef bool(__thiscall* oDrawSetTextColor)(PVOID, int);
			return GetMethod<oDrawSetTextColor>(this, 37 + 5)(this, textureID);
		}

		void DrawSetTextureRGBA(int textureID, unsigned char const* colors, int w, int h)
		{
			typedef void(__thiscall* oDrawSetTextColor)(PVOID, int, unsigned char const*, int, int);
			return GetMethod<oDrawSetTextColor>(this, 37)(this, textureID, colors, w, h);
		}

		int CreateNewTextureID(bool procedural)
		{
			typedef int(__thiscall* oDrawSetTextColor)(PVOID, bool);
			return GetMethod<oDrawSetTextColor>(this, 43)(this, procedural);
		}

		void DrawTexturedPolygon(int vtxCount, FontVertex_t* vtx, bool bClipVertices = true)
		{
			typedef void(__thiscall* oDrawSetTextColor)(PVOID, int, FontVertex_t*, bool);
			return GetMethod<oDrawSetTextColor>(this, 106)(this, vtxCount, vtx, bClipVertices);
		}

		unsigned long FontCreate()
		{
			typedef unsigned int(__thiscall* OriginalFn)(PVOID);
			return GetMethod<OriginalFn>(this, 71)(this);
		}

		void SetFontGlyphSet(unsigned long font, const char* windowsFontName, int tall, int weight, int blur, int scanlines, int flags)
		{
			typedef void(__thiscall* OriginalFn)(PVOID, unsigned long, const char*, int, int, int, int, int, int, int);
			GetMethod<OriginalFn>(this, 72)(this, font, windowsFontName, tall, weight, blur, scanlines, flags, 0, 0);
		}

		void GetTextSize(unsigned long font, const wchar_t* text, int& wide, int& tall)
		{
			typedef void(__thiscall* OriginalFn)(PVOID, unsigned long, const wchar_t*, int&, int&);
			GetMethod< OriginalFn >(this, 79)(this, font, text, wide, tall);
		}

		void DrawOutlinedCircle(int x, int y, int r, int seg)
		{
			typedef void(__thiscall* oDrawOutlinedCircle)(PVOID, int, int, int, int);
			return GetMethod<oDrawOutlinedCircle>(this, 103)(this, x, y, r, seg);
		}

		void SurfaceGetCursorPos(int& x, int& y)
		{
			typedef void(__thiscall* oSurfaceGetCursorPos)(PVOID, int&, int&);
			return GetMethod<oSurfaceGetCursorPos>(this, 66)(this, x, y);
		}

		void PlaySound(const char* pszSoundName)
		{
			VirtualFn(void)(PVOID, const char*);
			GetMethod< OriginalFn >(this, TABLE::ISurface::PlaySound)(this, pszSoundName);
		}
	};
}
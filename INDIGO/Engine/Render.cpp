#include "Render.h"

#define D3DFVF_CUSTOM_TEXT ( D3DFVF_XYZ | D3DFVF_DIFFUSE )
#define D3DFVF_CUSTOM_VERTEX ( D3DFVF_XYZRHW | D3DFVF_DIFFUSE )

namespace Engine
{
	CRender::CRender(IDirect3DDevice9* pDevice)
	{
		m_pFont = nullptr;
		LogFont = nullptr;
		m_pDevice = pDevice;

		m_pStateBlockDraw = nullptr;
		m_pStateBlockText = nullptr;

		if (!CreateObject())
		{
			MessageBoxA(0, "Failed to create D3D9 objects", "Error", MB_OK | MB_ICONERROR);
			TerminateProcess(GetCurrentProcess(), 0);
		}
	}

	CRender::~CRender()
	{
		if (m_pFont)
			m_pFont = nullptr;

		if (LogFont)
			LogFont = nullptr;

		if (m_pStateBlockDraw)
			m_pStateBlockDraw = nullptr;

		if (m_pStateBlockText)
			m_pStateBlockText = nullptr;

		m_pDevice = nullptr;
	}

	bool CRender::CreateObject()
	{
		if (!m_pFont)
		{
			HRESULT hFontRes = D3DXCreateFontA(m_pDevice, 15, 0, FW_NORMAL, 0, FALSE, RUSSIAN_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FW_DONTCARE, "Tahoma", &m_pFont);

			if (hFontRes != D3D_OK)
				return false;
		}

		if (!LogFont)
		{
			HRESULT lFont = D3DXCreateFontA(m_pDevice, 12, 0, 666, 0, FALSE, RUSSIAN_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FW_DONTCARE, "Vernada", &LogFont);
			if (lFont != D3D_OK)
				return false;
		}

		return true;
	}
	int CRender::GetTextHeight(const char* text)
	{
		RECT rcRect = { 0,0,0,0 };
		if (LogFont)
			LogFont->DrawTextA(NULL, text, strlen(text), &rcRect, DT_CALCRECT, D3DCOLOR_XRGB(0, 0, 0));
		return rcRect.bottom - rcRect.top;
	}
	void CRender::OnLostDevice()
	{
		if (m_pFont)
			m_pFont->OnLostDevice();

		if (LogFont)
			LogFont->OnLostDevice();

		if (m_pStateBlockDraw)
			m_pStateBlockDraw = nullptr;

		if (m_pStateBlockText)
			m_pStateBlockText = nullptr;
	}

	void CRender::OnResetDevice()
	{
		if (m_pFont)
			m_pFont->OnResetDevice();

		if (LogFont)
			LogFont->OnResetDevice();

		if (!CreateObject())
		{
			MessageBoxA(0, "Failed to create D3D9 objects", "Error", MB_OK | MB_ICONERROR);
			TerminateProcess(GetCurrentProcess(), 0);
		}
	}

	void CRender::BeginRender()
	{
		m_pDevice->CreateStateBlock(D3DSBT_ALL, &m_pStateBlockDraw);
		m_pDevice->CreateStateBlock(D3DSBT_ALL, &m_pStateBlockText);

		if (m_pStateBlockDraw)
			m_pStateBlockDraw->Capture();

		m_pDevice->GetRenderState(D3DRS_COLORWRITEENABLE, &dwOld_D3DRS_COLORWRITEENABLE);
		m_pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0xffffffff);
	}

	void CRender::EndRender()
	{
		m_pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, dwOld_D3DRS_COLORWRITEENABLE);

		if (m_pStateBlockDraw)
		{
			m_pStateBlockDraw->Apply();
			m_pStateBlockDraw->Release();
		}

		if (m_pStateBlockText)
			m_pStateBlockText->Release();
	}

	void CRender::DrawLine(int x1, int y1, int x2, int y2, Color color)
	{
		DWORD dxLineColor = D3DCOLOR_RGBA(color.r(), color.g(), color.b(), color.a());

		D3DTLVERTEX D3DTLV[2] = { 0 };

		CreateVertex(x1, y1, dxLineColor, PD3DTLVERTEX(&D3DTLV), 0);
		CreateVertex(x2, y2, dxLineColor, PD3DTLVERTEX(&D3DTLV), 1);

		SetVertexState();

		m_pDevice->DrawPrimitiveUP(D3DPT_LINELIST, 1, &D3DTLV, sizeof(D3DTLVERTEX));
	}

	void CRender::DrawBox(int x, int y, int w, int h, Color color)
	{
		DWORD dxBoxColor = D3DCOLOR_RGBA(color.r(), color.g(), color.b(), color.a());

		D3DTLVERTEX D3DTLV[8] = { 0 };

		// top
		CreateVertex(x, y, dxBoxColor, PD3DTLVERTEX(&D3DTLV), 0);
		CreateVertex(x + w, y, dxBoxColor, PD3DTLVERTEX(&D3DTLV), 1);

		// left
		CreateVertex(x, y, dxBoxColor, PD3DTLVERTEX(&D3DTLV), 2);
		CreateVertex(x, y + h, dxBoxColor, PD3DTLVERTEX(&D3DTLV), 3);

		// right
		CreateVertex(x + w - 1, y, dxBoxColor, PD3DTLVERTEX(&D3DTLV), 4);
		CreateVertex(x + w - 1, y + h, dxBoxColor, PD3DTLVERTEX(&D3DTLV), 5);

		// bottom
		CreateVertex(x, y + h - 1, dxBoxColor, PD3DTLVERTEX(&D3DTLV), 6);
		CreateVertex(x + w, y + h - 1, dxBoxColor, PD3DTLVERTEX(&D3DTLV), 7);

		SetVertexState();

		m_pDevice->DrawPrimitiveUP(D3DPT_LINELIST, 4, &D3DTLV, sizeof(D3DTLVERTEX));
	}

	void CRender::DrawFillBox(int x, int y, int w, int h, Color color)
	{
		DWORD dxFillBoxColor = D3DCOLOR_RGBA(color.r(), color.g(), color.b(), color.a());

		D3DTLVERTEX D3DTLV[4] = { 0 };

		CreateVertex(x, y, dxFillBoxColor, PD3DTLVERTEX(&D3DTLV), 0);
		CreateVertex(x + w, y, dxFillBoxColor, PD3DTLVERTEX(&D3DTLV), 1);
		CreateVertex(x, y + h, dxFillBoxColor, PD3DTLVERTEX(&D3DTLV), 2);
		CreateVertex(x + w, y + h, dxFillBoxColor, PD3DTLVERTEX(&D3DTLV), 3);

		SetVertexState();

		m_pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, &D3DTLV, sizeof(D3DTLVERTEX));
	}

	void CRender::DrawOutlineBox(int x, int y, int w, int h, Color color)
	{
		DrawBox(x, y, w, h, Color::Black());
		DrawBox(x + 1, y + 1, w - 2, h - 2, color);
		DrawBox(x + 2, y + 2, w - 4, h - 4, Color::Black());
	}

	void CRender::DrawCoalBox(int x, int y, int w, int h, Color color)
	{
		int iw = w / 4;
		int ih = h / 4;
		// top
		DrawLine(x, y, x + iw, y, color);					// left
		DrawLine(x + w - iw, y, x + w, y, color);			// right
		DrawLine(x, y, x, y + ih, color);					// top left
		DrawLine(x + w - 1, y, x + w - 1, y + ih, color);	// top right
															// bottom
		DrawLine(x, y + h, x + iw, y + h, color);			// left
		DrawLine(x + w - iw, y + h, x + w, y + h, color);	// right
		DrawLine(x, y + h - ih, x, y + h, color);			// bottom left
		DrawLine(x + w - 1, y + h - ih, x + w - 1, y + h, color);	// bottom right
	}

	void CRender::DrawOutlineCoalBox(int x, int y, int w, int h, Color color)
	{
		int iw = w / 4;
		int ih = h / 4;

		DrawCoalBox(x, y, w, h, Color::Black());

		// top left
		DrawLine(x + 1, y + 1, x + iw, y + 1, color);
		DrawLine(x + iw, y, x + iw, y + 2, Color::Black());
		DrawLine(x + 1, y + 1, x + 1, y + ih, color);
		DrawLine(x, y + ih, x + 2, y + ih, Color::Black());

		// top right
		DrawLine(x + w - iw, y + 1, x + w - 1, y + 1, color);
		DrawLine(x + w - iw - 1, y, x + w - iw, y + 2, Color::Black());
		DrawLine(x + w - 2, y + 1, x + w - 2, y + ih, color);
		DrawLine(x + w - 2, y + ih, x + w, y + ih, Color::Black());

		// bottom left
		DrawLine(x + 1, y + h - ih, x + 1, y + h, color);
		DrawLine(x, y + h - ih - 1, x + 2, y + h - ih - 1, Color::Black());
		DrawLine(x + 1, y + h - 1, x + iw, y + h - 1, color);
		DrawLine(x + iw, y + h - 1, x + iw, y + h + 1, Color::Black());

		// bottom right
		DrawLine(x + w - iw, y + h - 1, x + w - 1, y + h - 1, color);
		DrawLine(x + w - iw - 1, y + h - 1, x + w - iw, y + h + 1, Color::Black());
		DrawLine(x + w - 2, y + h - ih, x + w - 2, y + h, color);
		DrawLine(x + w - 2, y + h - ih - 1, x + w, y + h - ih - 1, Color::Black());

		DrawCoalBox(x + 2, y + 2, w - 4, h - 4, Color::Black());
	}

	void CRender::DrawWindow(int x, int y, int w, int h, Color Border, Color Background)
	{
		DrawBox(x, y, w, h, Border);
		DrawFillBox(x + 1, y + 1, w - 2, h - 2, Background);
	}

	void CRender::DrawHorBar(int x, int y, int w, int h, int val, Color color1, Color color2)
	{
		if (val > 100) val = 100;
		int size = (w * val) / 100;

		DrawBox(x, y, w, h, Color::Black());
		DrawFillBox(x + 1, y + 1, w - 2, h - 2, color2);

		if (size == w)
			DrawFillBox(x + 1, y + 1, size - 2, h - 2, color1);
		else
			DrawFillBox(x + 1, y + 1, size, h - 2, color1);
	}

	void CRender::DrawVerBar(int x, int y, int w, int h, int val, Color color1, Color color2)
	{
		if (val > 100) val = 100;
		int size = (h * val) / 100;

		DrawBox(x, y, w, h, Color(0, 0, 0));
		DrawFillBox(x + 1, y + 1, w - 2, h - 2, color2);

		if (size == h)
			DrawFillBox(x + 1, y + 1 + h - size, w - 2, size - 2, color1);
		else
			DrawFillBox(x + 1, y + 1 + h - size, w - 2, size, color1);
	}

	void CRender::Text(int x, int y, bool center, bool shadow, Color color, const char* format, ...)
	{
		if (m_pStateBlockText)
			m_pStateBlockText->Capture();

		m_pDevice->SetFVF(D3DFVF_CUSTOM_TEXT);

		m_pDevice->SetRenderState(D3DRS_LIGHTING, false);
		m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
		m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, false);
		m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		char Buffer[128] = { '\0' };
		va_list va_alist;
		va_start(va_alist, format);
		vsprintf_s(Buffer, format, va_alist);
		va_end(va_alist);

		BSTR text = CSX::Utils::ConvertStringToBSTR_UTF8(Buffer);

		DWORD dxTextColor = D3DCOLOR_XRGB(color.r(), color.g(), color.b());

		auto drawShadow = [&](RECT rect)
		{
			rect.left++;
			m_pFont->DrawTextW(NULL, text, -1, &rect, DT_TOP | DT_LEFT | DT_NOCLIP, 0xFF000000);
			rect.top++;
			m_pFont->DrawTextW(NULL, text, -1, &rect, DT_TOP | DT_LEFT | DT_NOCLIP, 0xFF000000);
		};

		if (center)
		{
			RECT rec = { 0,0,0,0 };

			m_pFont->DrawTextW(NULL, text, -1, &rec, DT_CALCRECT | DT_NOCLIP, dxTextColor);

			rec =
			{
				static_cast<LONG>(x) - rec.right / 2,
				static_cast<LONG>(y),
				0,
				0
			};

			if (shadow)
				drawShadow(rec);

			m_pFont->DrawTextW(NULL, text, -1, &rec, DT_TOP | DT_LEFT | DT_NOCLIP, dxTextColor);
		}
		else
		{
			RECT rec =
			{
				static_cast<LONG>(x),
				static_cast<LONG>(y),
				0,
				0
			};

			if (shadow)
				drawShadow(rec);

			m_pFont->DrawTextW(NULL, text, -1, &rec, DT_TOP | DT_LEFT | DT_NOCLIP, dxTextColor);
		}

		if (m_pStateBlockText)
			m_pStateBlockText->Apply();

		SysFreeString(text);
	}

	void CRender::TextToConsole(int x, int y, bool center, bool shadow, Color color, const char* format, ...)
	{
		if (m_pStateBlockText)
			m_pStateBlockText->Capture();

		m_pDevice->SetFVF(D3DFVF_CUSTOM_TEXT);

		m_pDevice->SetRenderState(D3DRS_LIGHTING, false);
		m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
		m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, false);
		m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		char Buffer[128] = { '\0' };
		va_list va_alist;
		va_start(va_alist, format);
		vsprintf_s(Buffer, format, va_alist);
		va_end(va_alist);

		BSTR text = CSX::Utils::ConvertStringToBSTR_UTF8(Buffer);

		DWORD dxTextColor = D3DCOLOR_XRGB(color.r(), color.g(), color.b());

		auto drawShadow = [&](RECT rect)
		{
			rect.left++;
			LogFont->DrawTextW(NULL, text, -1, &rect, DT_TOP | DT_LEFT | DT_NOCLIP, 0xFF000000);
			rect.top++;
			LogFont->DrawTextW(NULL, text, -1, &rect, DT_TOP | DT_LEFT | DT_NOCLIP, 0xFF000000);
		};

		if (center)
		{
			RECT rec = { 0,0,0,0 };

			LogFont->DrawTextW(NULL, text, -1, &rec, DT_CALCRECT | DT_NOCLIP, dxTextColor);

			rec =
			{
				static_cast<LONG>(x) - rec.right / 2,
				static_cast<LONG>(y),
				0,
				0
			};

			if (shadow)
				drawShadow(rec);

			LogFont->DrawTextW(NULL, text, -1, &rec, DT_TOP | DT_LEFT | DT_NOCLIP, dxTextColor);
		}
		else
		{
			RECT rec =
			{
				static_cast<LONG>(x),
				static_cast<LONG>(y),
				0,
				0
			};

			if (shadow)
				drawShadow(rec);

			LogFont->DrawTextW(NULL, text, -1, &rec, DT_TOP | DT_LEFT | DT_NOCLIP, dxTextColor);
		}

		if (m_pStateBlockText)
			m_pStateBlockText->Apply();

		SysFreeString(text);
	}

	void CRender::SetVertexState()
	{
		m_pDevice->SetTexture(0, 0);
		m_pDevice->SetPixelShader(0);

		m_pDevice->SetFVF(D3DFVF_CUSTOM_VERTEX);

		m_pDevice->SetRenderState(D3DRS_LIGHTING, false);
		m_pDevice->SetRenderState(D3DRS_FOGENABLE, false);

		m_pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
		m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);

		m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, false);
		m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		m_pDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, true);
		m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);

		m_pDevice->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, false);
		m_pDevice->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, false);

		m_pDevice->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, true);

		m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		m_pDevice->SetRenderState(D3DRS_SRCBLENDALPHA, D3DBLEND_INVDESTALPHA);
		m_pDevice->SetRenderState(D3DRS_DESTBLENDALPHA, D3DBLEND_ONE);
	}

	void CRender::CreateVertex(int x, int y, DWORD dxColor, PD3DTLVERTEX pVertext, DWORD index)
	{
		pVertext[index].x = (float)x;
		pVertext[index].y = (float)y;
		pVertext[index].z = 0.f;
		pVertext[index].rhw = 0.f;
		pVertext[index].dxColor = dxColor;
	}
}
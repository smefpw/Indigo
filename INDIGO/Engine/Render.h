#pragma once

#include "Engine.h"

namespace Engine
{
	typedef struct D3DTLVERTEX
	{
		float x;
		float y;
		float z;
		float rhw;
		D3DCOLOR dxColor;
	} *PD3DTLVERTEX;
	
	class CRender
	{
	public:

		CRender( IDirect3DDevice9* pDevice );
		~CRender();
//[swap_lines]
		void OnLostDevice();
		void OnResetDevice();

		void BeginRender();
		void EndRender();

		void DrawLine( int x1 , int y1 , int x2 , int y2 , Color color );

		void DrawBox( int x , int y , int w , int h , Color color );
		void DrawFillBox( int x , int y , int w , int h , Color color );
		void DrawOutlineBox( int x , int y , int w , int h , Color color );

		void DrawCoalBox( int x , int y , int w , int h , Color color );
		void DrawOutlineCoalBox( int x , int y , int w , int h , Color color );
		void DrawWave1(Vector loc, float radius, Color color);

		void DrawWindow( int x , int y , int w , int h , Color Border , Color Background );

		void DrawHorBar( int x , int y , int w , int h , int val , Color color1 , Color color2 );
		void DrawVerBar( int x , int y , int w , int h , int val , Color color1 , Color color2 );
		
		void Text( int x , int y , bool center , bool shadow , Color color , const char* format , ... );
//[/swap_lines]
		IDirect3DDevice9*		m_pDevice;

	private:
//[swap_lines]
		bool CreateObject();

		void SetVertexState();

		void CreateVertex( int x , int y , DWORD dxColor , PD3DTLVERTEX pVertext , DWORD index );

		ID3DXFont*				m_pFont;
		IDirect3DStateBlock9*	m_pStateBlockDraw;
		IDirect3DStateBlock9*	m_pStateBlockText;

		DWORD					dwOld_D3DRS_COLORWRITEENABLE;
//[/swap_lines]
	};

	namespace D3D9
	{
		enum TABLE
		{
			QueryInterface ,
			AddRef ,
			Release ,
			TestCooperativeLevel ,
			GetAvailableTextureMem ,
			EvictManagedResources ,
			GetDirect3D ,
			GetDeviceCaps ,
			GetDisplayMode ,
			GetCreationParameters ,
			SetCursorProperties ,
			SetCursorPosition ,
			ShowCursor ,
			CreateAdditionalSwapChain ,
			GetSwapChain ,
			GetNumberOfSwapChains ,
			Reset ,
			Present ,
			GetBackBuffer ,
			GetRasterStatus ,
			SetDialogBoxMode ,
			SetGammaRamp ,
			GetGammaRamp ,
			CreateTexture ,
			CreateVolumeTexture ,
			CreateCubeTexture ,
			CreateVertexBuffer ,
			CreateIndexBuffer ,
			CreateRenderTarget ,
			CreateDepthStencilSurface ,
			UpdateSurface ,
			UpdateTexture ,
			GetRenderTargetData ,
			GetFrontBufferData ,
			StretchRect ,
			ColorFill ,
			CreateOffscreenPlainSurface ,
			SetRenderTarget ,
			GetRenderTarget ,
			SetDepthStencilSurface ,
			GetDepthStencilSurface ,
			BeginScene ,
			EndScene ,
			Clear ,
			SetTransform ,
			GetTransform ,
			MultiplyTransform ,
			SetViewport ,
			GetViewport ,
			SetMaterial ,
			GetMaterial ,
			SetLight ,
			GetLight ,
			LightEnable ,
			GetLightEnable ,
			SetClipPlane ,
			GetClipPlane ,
			SetRenderState ,
			GetRenderState ,
			CreateStateBlock ,
			BeginStateBlock ,
			EndStateBlock ,
			SetClipStatus ,
			GetClipStatus ,
			GetTexture ,
			SetTexture ,
			GetTextureStageState ,
			SetTextureStageState ,
			GetSamplerState ,
			SetSamplerState ,
			ValidateDevice ,
			SetPaletteEntries ,
			GetPaletteEntries ,
			SetCurrentTexturePalette ,
			GetCurrentTexturePalette ,
			SetScissorRect ,
			GetScissorRect ,
			SetSoftwareVertexProcessing ,
			GetSoftwareVertexProcessing ,
			SetNPatchMode ,
			GetNPatchMode ,
			DrawPrimitive ,
			DrawIndexedPrimitive ,
			DrawPrimitiveUP ,
			DrawIndexedPrimitiveUP ,
			ProcessVertices ,
			CreateVertexDeclaration ,
			SetVertexDeclaration ,
			GetVertexDeclaration ,
			SetFVF ,
			GetFVF ,
			CreateVertexShader ,
			SetVertexShader ,
			GetVertexShader ,
			SetVertexShaderConstantF ,
			GetVertexShaderConstantF ,
			SetVertexShaderConstantI ,
			GetVertexShaderConstantI ,
			SetVertexShaderConstantB ,
			GetVertexShaderConstantB ,
			SetStreamSource ,
			GetStreamSource ,
			SetStreamSourceFreq ,
			GetStreamSourceFreq ,
			SetIndices ,
			GetIndices ,
			CreatePixelShader ,
			SetPixelShader ,
			GetPixelShader ,
			SetPixelShaderConstantF ,
			GetPixelShaderConstantF ,
			SetPixelShaderConstantI ,
			GetPixelShaderConstantI ,
			SetPixelShaderConstantB ,
			GetPixelShaderConstantB ,
			DrawRectPatch ,
			DrawTriPatch ,
			DeletePatch ,
			CreateQuery
		};
	}
}
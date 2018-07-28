#include "Hook.h"

//[enc_string_enable /]
//[junk_enable /]

namespace Engine
{
	namespace Hook
	{
		vfunc_hook panel;
		vfunc_hook client;
		vfunc_hook clientmode;
		vfunc_hook modelrender;
		vfunc_hook sound;
		vfunc_hook surface;
		vfunc_hook eventmanager;
		vfunc_hook device;
		IDirect3DDevice9* g_pDevice = nullptr;

		typedef HRESULT(WINAPI* Present_t)(IDirect3DDevice9* pDevice, CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion);
		Present_t Present_o;

		typedef HRESULT(WINAPI* Reset_t)(LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);
		Reset_t Reset_o;

		HRESULT WINAPI Hook_Present(IDirect3DDevice9* pDevice, CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion)
		{
			Client::OnRender();

			return Present_o(pDevice, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
		}
		void __stdcall Hook_Reset(LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters)
		{
			static auto ofunc = device.get_original<ResetHook>(D3D9::TABLE::Reset);
			ofunc(pDevice, pPresentationParameters);
			Client::OnLostDevice();
			HRESULT hRes = Reset_o(pDevice, pPresentationParameters);
			Client::OnResetDevice();
		}

		void __stdcall Hook_CreateMove(float flInputSampleTime, CUserCmd* pCmd)
		{
			static auto ofunc = clientmode.get_original<CreateMove>(TABLE::IClientMode::CreateMove);
			ofunc(Interfaces::ClientMode(), flInputSampleTime, pCmd);
			Client::OnCreateMove(pCmd);
		}

		void __fastcall Hook_OverrideView(CViewSetup* pSetup)
		{
			static auto ofunc = clientmode.get_original<GetFov>(TABLE::IClientMode::OverrideView);
			Client::OnOverrideView(pSetup);
			ofunc(pSetup);
		}
		float __stdcall Hook_GetViewModelFOV()
		{
			//i dont need that
			return 0;
		}

		bool __fastcall Hook_FireEventClientSideThink(void* ecx, void* edx, IGameEvent* pEvent)
		{
			static auto ofunc = eventmanager.get_original<GameEvent>(TABLE::IGameEventManager2::FireEventClientSide);
			if (!pEvent)
				return ofunc(ecx, pEvent);
			Client::OnFireEventClientSideThink(pEvent);
			return ofunc(ecx, pEvent);
		}

		void __stdcall Hook_FrameStageNotify(ClientFrameStage_t stage)
		{
			static auto ofunc = client.get_original<FrameStageNotify>(TABLE::IBaseClientDLL::FrameStageNotify);
			ofunc(Interfaces::Client(), stage);
			Skin_OnFrameStageNotify(stage);
			Gloves_OnFrameStageNotify(stage);
		}

		void __stdcall Hook_EmitSound(IRecipientFilter& filter, int iEntIndex, int iChannel, const char *pSoundEntry, unsigned int nSoundEntryHash, const char *pSample,
			float flVolume, soundlevel_t iSoundlevel, int nSeed, int iFlags = 0, int iPitch = PITCH_NORM,
			const Vector *pOrigin = NULL, const Vector *pDirection = NULL, CUtlVector< Vector >* pUtlVecOrigins = NULL, bool bUpdatePositions = true, float soundtime = 0.0f, int speakerentity = -1, int test = 0)
		{
			auto oEmitsound = sound.get_original<EmitsoundXD>(TABLE::IEngineSound::EmitSound1);
			if (pSample && Client::g_pPlayers->GetPlayer(iEntIndex)->Team != Client::g_pPlayers->GetLocal()->Team)
			{
				Client::OnPlaySound(pOrigin, pSample);
			}
			return oEmitsound(filter, iEntIndex, iChannel, pSoundEntry, nSoundEntryHash, pSample,
				flVolume, iSoundlevel, nSeed, iFlags, iPitch,
				pOrigin, pDirection, pUtlVecOrigins, bUpdatePositions, soundtime, speakerentity, 0);


		}
		void __stdcall Hook_EmitSound2(IRecipientFilter& filter, int iEntIndex, int iChannel, const char *pSoundEntry, unsigned int nSoundEntryHash, const char *pSample,
			float flVolume, soundlevel_t iSoundlevel, int nSeed, int iFlags = 0, int iPitch = PITCH_NORM,
			const Vector *pOrigin = NULL, const Vector *pDirection = NULL, CUtlVector< Vector >* pUtlVecOrigins = NULL, bool bUpdatePositions = true, float soundtime = 0.0f, int speakerentity = -1, int test = 0)
		{
			auto oEmitsound = sound.get_original<EmitsoundXD>(TABLE::IEngineSound::EmitSound2);
			if (pSample && Client::g_pPlayers->GetPlayer(iEntIndex)->Team != Client::g_pPlayers->GetLocal()->Team)
			{
				Client::OnPlaySound(pOrigin, pSample);
			}
			return oEmitsound(filter, iEntIndex, iChannel, pSoundEntry, nSoundEntryHash, pSample,
				flVolume, iSoundlevel, nSeed, iFlags, iPitch,
				pOrigin, pDirection, pUtlVecOrigins, bUpdatePositions, soundtime, speakerentity, 0);


		}

		void __fastcall Hook_DrawModelExecute(IMatRenderContext* ctx, const DrawModelState_t &state, const ModelRenderInfo_t &pInfo, matrix3x4_t *pCustomBoneToWorld)
		{
			// you need to rewrite chams or find some online
		}

		void __stdcall Hook_PlaySound(const char* pszSoundName)
		{
			static auto ofunc = surface.get_original<PlaySound>(TABLE::ISurface::PlaySound);
			ofunc(Interfaces::Surface(), pszSoundName);
			if (pszSoundName)
				Client::OnPlaySound(pszSoundName);
		}
		bool Initialize()
		{
			if (!CSX::Utils::IsModuleLoad(D3D9_DLL))
				return false;

			if (!CSX::Utils::IsModuleLoad(SHADERPIDX9_DLL))
				return false;

			if (!CSX::Utils::IsModuleLoad(GAMEOVERLAYRENDERER_DLL))
				return false;

			DWORD d3d9TablePtrPtr = CSX::Memory::FindPattern(SHADERPIDX9_DLL, D3D9_PATTERN, D3D9_MASK, 1);
			DWORD_PTR** dwPresent_o = (DWORD_PTR**)CSX::Memory::FindPattern(GAMEOVERLAYRENDERER_DLL, GMOR_PATTERN, GMOR_MASK, 1);

			if (d3d9TablePtrPtr && dwPresent_o)
			{
				g_pDevice = (IDirect3DDevice9*)(**(PDWORD*)d3d9TablePtrPtr);

				if (device.setup(g_pDevice))
				{
					//all undetect and rewrite
					DWORD_PTR* dwAddress = *dwPresent_o;
					Present_o = (Present_t)(*dwAddress);
					*dwAddress = (DWORD_PTR)(&Hook_Present);

					device.hook_index(D3D9::TABLE::Reset, Hook_Reset);

					if (!clientmode.setup(Interfaces::ClientMode()))
						return false;

					clientmode.hook_index(TABLE::IClientMode::CreateMove, Hook_CreateMove);
					clientmode.hook_index(TABLE::IClientMode::OverrideView, Hook_OverrideView);
					//clientmode.hook_index(TABLE::IClientMode::GetViewModelFOV, Hook_GetViewModelFOV);

					if (!eventmanager.setup(Interfaces::GameEvent()))
						return false;

					eventmanager.hook_index(TABLE::IGameEventManager2::FireEventClientSide, Hook_FireEventClientSideThink);

					if (!client.setup(Interfaces::Client()))
						return false;

					client.hook_index(TABLE::IBaseClientDLL::FrameStageNotify, Hook_FrameStageNotify);

					if (!sound.setup(Interfaces::Sound()))
						return false;

					sound.hook_index(TABLE::IEngineSound::EmitSound1, Hook_EmitSound);

					//if (!modelrender.setup(Interfaces::ModelRender()))
					//	return false;
					// dont like chams so shit
					//modelrender.hook_index(TABLE::IVModelRender::DrawModelExecute, Hook_DrawModelExecute);

					if (!surface.setup(Interfaces::Surface()))
						return false;

					surface.hook_index(TABLE::ISurface::PlaySound, Hook_PlaySound);

					if (Client::Initialize(g_pDevice))
						return true;
					//all works
				}
			}

			return false;
		}

		void Shutdown()
		{
			device.unhook_all();
			sound.unhook_all();
			clientmode.unhook_all();
			eventmanager.unhook_all();
			modelrender.unhook_all();
			client.unhook_all();
		}
	}
}
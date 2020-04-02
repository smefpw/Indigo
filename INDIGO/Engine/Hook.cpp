#include "Hook.h"
#include "intrin.h"

//[enc_string_enable /]
//[junk_enable /]

CAimbot* g_Aim;

namespace Engine {
	namespace Hook {
		/*
			WARNING: This cheat (like pretty much any other pastes out there) uses Virtual Method Table (VMT) hooking, which is now detected by Valve Anti-Cheat.
			We are NOT responsible for ANY bans that may occur during the process of using this cheat. This includes, but not limited to, VAC, Untrusted and
			Overwatch bans.
		*/
		CSX::Hook::VTable IDirect3DDevice9Table;
		CSX::Hook::VTable SoundTable;
		CSX::Hook::VTable ClientModeTable;
		CSX::Hook::VTable GameEventTable;
		CSX::Hook::VTable ModelRenderTable;
		CSX::Hook::VTable ClientTable;
		CSX::Hook::VTable SurfaceTable;
		CSX::Hook::VTable EngineTable;
		CSX::Hook::VTable SteamGameCoordinatorTable;
		IDirect3DDevice9* g_pDevice = nullptr;

		typedef HRESULT(WINAPI* Present_t)(IDirect3DDevice9* pDevice, CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion);
		Present_t Present_o;

		typedef HRESULT(WINAPI* Reset_t)(LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);
		Reset_t Reset_o;

		bool ps;
		HRESULT WINAPI Hook_Present(IDirect3DDevice9* pDevice, CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion) {
#if ENABLE_DEBUG_FILE == 1
			if (!ps) {
				CSX::Log::Add("\n[Hooked - Present]");
				ps = true;
			}
#endif
			Client::OnRender();
			return Present_o(pDevice, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
		}

		bool rs;
		HRESULT WINAPI Hook_Reset(LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters) {
#if ENABLE_DEBUG_FILE == 1
			if (!rs) {
				CSX::Log::Add("[Hooked - Reset]");
				rs = true;
			}
#endif
			Client::OnLostDevice();
			HRESULT hRes = Reset_o(pDevice, pPresentationParameters);
			Client::OnResetDevice();
			return hRes;
		}

		bool cm;
		bool WINAPI Hook_CreateMove(float flInputSampleTime, CUserCmd* pCmd) {
#if ENABLE_DEBUG_FILE == 1
			if (!cm) {
				CSX::Log::Add("[Hooked - CreateMove]");
				cm = true;
			}
#endif
			if (!pCmd->command_number) {
				return true;
			}
			ClientModeTable.UnHook();
			auto ebp = (uintptr_t*)(uintptr_t(_AddressOfReturnAddress()) - sizeof(void*));
			bool& bSendPacket = *reinterpret_cast<bool*>( *ebp - 0x1C );
			Client::OnCreateMove(pCmd, bSendPacket);
			bool ret = Interfaces::ClientMode()->CreateMove(flInputSampleTime, pCmd);
			ClientModeTable.ReHook();
			return false;
		}

		bool dpsse;
		int WINAPI Hook_DoPostScreenSpaceEffects(int callback) {
#if ENABLE_DEBUG_FILE == 1
			if (!dpsse) {
				CSX::Log::Add("[Hooked - DoPostScreenSpaceEffects]");
				dpsse = true;
			}
#endif
			ClientModeTable.UnHook();
			int ret = Interfaces::ClientMode()->DoPostScreenSpaceEffects(callback);
			ClientModeTable.ReHook();
			auto glow_target = [](GlowObjectDefinition_t& glowObject, Color color) -> void {
				glowObject.m_flRed = color.r() / 255.f;
				glowObject.m_flGreen = color.g() / 255.f;
				glowObject.m_flBlue = color.b() / 255.f;
				glowObject.m_flAlpha = color.a() / 255.f;
				glowObject.m_bRenderWhenOccluded = true;
				glowObject.m_bRenderWhenUnoccluded = false;
			};
			if (Interfaces::GlowManager && Interfaces::Engine()->IsConnected()) {
				if (Settings::Esp::glow) {
					for (auto i = 0; i < Interfaces::GlowManager()->m_GlowObjectDefinitions.Count(); i++) {
						auto& glowObject = Interfaces::GlowManager()->m_GlowObjectDefinitions[i];
						auto entity = reinterpret_cast<CBaseEntity*>(glowObject.m_pEntity);
						if (!entity || glowObject.IsUnused()) {
							continue;
						}
						switch (entity->GetClientClass()->m_ClassID) {
							case (int)CLIENT_CLASS_ID::CCSPlayer: {
								if (entity->GetTeam() != Client::g_pPlayers->GetLocal()->m_pEntity->GetTeam()) {
									glow_target(glowObject, Color(255, 255, 255, 255));
								}
							}
							break;
							default:
								break;
						}
					}
				}
			}
			return ret;
		}

		int ic1; //is connected 1
		bool WINAPI Hook_IsConnected() {
			//1st April 2020
			static void* unk = CSX::Memory::NewPatternScan(GetModuleHandleA(CLIENT_DLL), "75 04 B0 01 5F") - 2;
#if ENABLE_DEBUG_FILE == 1
			if (!ic1) {
				CSX::Log::Add("[FindPattern/Hooked - IsConnected = %X]", unk);
				ic1 = true;
			}
#endif
			/*if (_ReturnAddress() == unk && Settings::Misc::misc_inventory) {
				return false;
			}*/ //not called anyway
			EngineTable.UnHook();
			bool ret = Interfaces::Engine()->IsConnected();
			EngineTable.ReHook();
			return ret;
		}

		bool orv; //ovr
		bool WINAPI Hook_OverrideView(CViewSetup* pSetup) {
#if ENABLE_DEBUG_FILE == 1
			if (!orv) {
				CSX::Log::Add("[Hooked - OverrideView]");
				orv = true;
			}
#endif
			Client::OnOverrideView(pSetup);
			ClientModeTable.UnHook();
			bool ret = Interfaces::ClientMode()->OverrideView(pSetup);
			ClientModeTable.ReHook();
			return ret;
		}

		bool gvm; //gvm
		float WINAPI Hook_GetViewModelFOV() {
#if ENABLE_DEBUG_FILE == 1
			if (!gvm) {
				CSX::Log::Add("[Hooked - GetViewModelFOV]");
				gvm = true;
			}
#endif
			ClientModeTable.UnHook();
			float fov = Interfaces::ClientMode()->GetViewModelFOV();
			ClientModeTable.ReHook();
			Client::OnGetViewModelFOV(fov);
			return fov;
		}

		bool rm;
		EGCResults __fastcall Hook_RetrieveMessage(void* ecx, void* edx, uint32_t *punMsgType, void *pubDest, uint32_t cubDest, uint32_t *pcubMsgSize) {
#if ENABLE_DEBUG_FILE == 1
			if (!rm) {
				CSX::Log::Add("[Hooked - RetrieveMessage]");
				rm = true;
			}
#endif
			SteamGameCoordinatorTable.UnHook();
			EGCResults status = Interfaces::SteamGameCoordinator()->RetrieveMessage(punMsgType, pubDest, cubDest, pcubMsgSize);
			SteamGameCoordinatorTable.ReHook();
			if (status != k_EGCResultOK) {
				return status;
			}
			Client::OnRetrieveMessage(ecx, edx, punMsgType, pubDest, cubDest, pcubMsgSize);
			return status;
		}

		bool sm;
		EGCResults __fastcall Hook_SendMessage(void* ecx, void* edx, uint32_t unMsgType, const void* pubData, uint32_t cubData) {
#if ENABLE_DEBUG_FILE == 1
			if (!sm) {
				CSX::Log::Add("\n[Hooked - SendMessage]");
				sm = true;
			}
#endif
			uint32_t messageType = unMsgType & 0x7FFFFFFF;
			void* pubDataMutable = const_cast<void*>(pubData);
			Client::OnSendMessage(ecx, edx, unMsgType, pubData, cubData);
			SteamGameCoordinatorTable.UnHook();
			EGCResults status = Interfaces::SteamGameCoordinator()->SendMessage(unMsgType, pubData, cubData);
			SteamGameCoordinatorTable.ReHook();
			return status;
		}

		bool fecst;
		bool WINAPI Hook_FireEventClientSideThink(IGameEvent* pEvent) {
#if ENABLE_DEBUG_FILE == 1
			if (!fecst) {
				CSX::Log::Add("[Hooked - FireEventClientSideThink]");
				fecst = true;
			}
#endif
			bool ret = false;
			if (!pEvent) {
				GameEventTable.UnHook();
				ret = Interfaces::GameEvent()->FireEventClientSide(pEvent);
				GameEventTable.ReHook();
				return ret;
			}
			Client::OnFireEventClientSideThink(pEvent);
			GameEventTable.UnHook();
			ret = Interfaces::GameEvent()->FireEventClientSide(pEvent);
			GameEventTable.ReHook();
			return ret;
		}

		bool fsn;
		void WINAPI Hook_FrameStageNotify(ClientFrameStage_t Stage) {
#if ENABLE_DEBUG_FILE == 1
			if (!fsn) {
				CSX::Log::Add("[Hooked - FrameStageNotify]");
				fsn = true;
			}
#endif
			Client::OnFrameStageNotify(Stage);
			ClientTable.UnHook();
			Interfaces::Client()->FrameStageNotify(Stage);
			ClientTable.ReHook();
		}

		bool es1;
		int WINAPI Hook_EmitSound1(IRecipientFilter& filter, int iEntIndex, int iChannel, const char *pSoundEntry, unsigned int nSoundEntryHash, const char *pSample,
			float flVolume, soundlevel_t iSoundlevel, int nSeed, int iFlags = 0, int iPitch = PITCH_NORM,
			const Vector *pOrigin = NULL, const Vector *pDirection = NULL, CUtlVector< Vector >* pUtlVecOrigins = NULL, bool bUpdatePositions = true, float soundtime = 0.0f, int speakerentity = -1, int unklown = 0)
		{
#if ENABLE_DEBUG_FILE == 1
			if (!es1) {
				CSX::Log::Add("\n[Hooked - EmitSound1]\n");
				es1 = true;
			}
#endif
			if (pSample) {
				Client::OnPlaySound(pOrigin, pSample);
			}
			SoundTable.UnHook();
			int ret = Interfaces::Sound()->EmitSound1(filter, iEntIndex, iChannel, pSoundEntry, nSoundEntryHash, pSample,
				flVolume, iSoundlevel, nSeed, iFlags, iPitch,
				pOrigin, pDirection, pUtlVecOrigins, bUpdatePositions, soundtime, speakerentity);
			SoundTable.ReHook();
			return ret;
		}

		bool es2;
		int WINAPI Hook_EmitSound2(IRecipientFilter& filter, int iEntIndex, int iChannel, const char *pSoundEntry, unsigned int nSoundEntryHash, const char *pSample,
			float flVolume, float flAttenuation, int nSeed, int iFlags = 0, int iPitch = PITCH_NORM,
			const Vector *pOrigin = NULL, const Vector *pDirection = NULL, CUtlVector< Vector >* pUtlVecOrigins = NULL, bool bUpdatePositions = true, float soundtime = 0.0f, int speakerentity = -1, int unklown = 0)
		{
#if ENABLE_DEBUG_FILE == 1
			if (!es2) {
				CSX::Log::Add("\n[Hooked - EmitSound2]\n");
				es2 = true;
			}
#endif
			if (pSample) {
				Client::OnPlaySound(pOrigin, pSample);
			}
			SoundTable.UnHook();
			int ret = Interfaces::Sound()->EmitSound2(filter, iEntIndex, iChannel, pSoundEntry, nSoundEntryHash, pSample,
				flVolume, flAttenuation, nSeed, iFlags, iPitch,
				pOrigin, pDirection, pUtlVecOrigins, bUpdatePositions, soundtime, speakerentity);
			SoundTable.ReHook();
			return ret;
		}

		bool dme;
		void WINAPI Hook_DrawModelExecute(IMatRenderContext* ctx, const DrawModelState_t &state,
			const ModelRenderInfo_t &pInfo, matrix3x4_t *pCustomBoneToWorld = NULL)
		{
#if ENABLE_DEBUG_FILE == 1
			if (!dme) {
				CSX::Log::Add("[Hooked - DrawModelExecute]\n");
				dme = true;
			}
#endif
			ModelRenderTable.UnHook();
			if (ctx && pCustomBoneToWorld) {
				Client::OnDrawModelExecute(ctx, state, pInfo, pCustomBoneToWorld);
			}
			Interfaces::ModelRender()->DrawModelExecute(ctx, state, pInfo, pCustomBoneToWorld);
			if (ctx && pCustomBoneToWorld && Client::g_pEsp && Settings::Esp::esp_Chams) {
				Interfaces::ModelRender()->ForcedMaterialOverride(0);
			}
			ModelRenderTable.ReHook();
		}

		bool plays;
		void WINAPI Hook_PlaySound(const char* pszSoundName) {
#if ENABLE_DEBUG_FILE == 1
			if (!plays) {
				CSX::Log::Add("\n[Hooked - PlaySound]\n");
				plays = true;
			}
#endif
			SurfaceTable.UnHook();
			if (pszSoundName) {
				Client::OnPlaySound(pszSoundName);
			}
			Interfaces::Surface()->PlaySound(pszSoundName);
			SurfaceTable.ReHook();
		}

		bool lc;
		void Hook_LockCursor() {
#if ENABLE_DEBUG_FILE == 1
			if (!lc) {
				CSX::Log::Add("[Hooked - LockCursor]\n");
				lc = true;
			}
#endif
			SurfaceTable.UnHook();
			Interfaces::Surface()->LockCursor();
			SurfaceTable.ReHook();
			if (bIsGuiVisible) {
				Interfaces::Surface()->UnlockCursor();
			}
		}

		bool Initialize() {
			if (!CSX::Utils::IsModuleLoad(D3D9_DLL)) {
#if ENABLE_DEBUG_FILE == 1
				CSX::Log::Add("[Hooks - module %s not loaded!]", D3D9_DLL);
#endif
				return false;
			}

			if (!CSX::Utils::IsModuleLoad(SHADERPIDX9_DLL)) {
#if ENABLE_DEBUG_FILE == 1
				CSX::Log::Add("[Hooks - module %s not loaded!]", SHADERPIDX9_DLL);
#endif
				return false;
			}

			if (!CSX::Utils::IsModuleLoad(GAMEOVERLAYRENDERER_DLL)) {
#if ENABLE_DEBUG_FILE == 1
				CSX::Log::Add("[Hooks - module %s not loaded!]", GAMEOVERLAYRENDERER_DLL);
#endif
				return false;
			}

			DWORD d3d9TablePtrPtr = CSX::Memory::FindPattern(SHADERPIDX9_DLL, D3D9_PATTERN, D3D9_MASK, 1);
#if ENABLE_DEBUG_FILE == 1
			CSX::Log::Add("[FindPattern - pD3D9Table = %X]", d3d9TablePtrPtr);
#endif
			//30th March 2020 - GameOverlayRenderer.dll v5.78.0.89
			DWORD_PTR** dwPresent_o = (DWORD_PTR**)CSX::Memory::FindPattern(GAMEOVERLAYRENDERER_DLL, GMOR_PATTERN, GMOR_MASK, 1);
#if ENABLE_DEBUG_FILE == 1
			CSX::Log::Add("[FindPattern - dwPresent = %X]", dwPresent_o);
#endif

			if (d3d9TablePtrPtr && dwPresent_o) {
				g_pDevice = (IDirect3DDevice9*)(**(PDWORD*)d3d9TablePtrPtr);
				if (IDirect3DDevice9Table.InitTable(g_pDevice)) {
					DWORD_PTR* dwAddress = *dwPresent_o;
					Present_o = (Present_t)(*dwAddress);
					*dwAddress = (DWORD_PTR)(&Hook_Present);

					//works
					Reset_o = (Reset_t)IDirect3DDevice9Table.GetHookIndex(D3D9::TABLE::Reset, Hook_Reset);

					if (!ClientModeTable.InitTable(Interfaces::ClientMode())) {
#if ENABLE_DEBUG_FILE == 1
						CSX::Log::Add("\n[Clientmode - failed to init!]");
#endif*/
						return false;
					}

					//works
					ClientModeTable.HookIndex(TABLE::IClientMode::CreateMove, Hook_CreateMove);
					ClientModeTable.HookIndex(TABLE::IClientMode::OverrideView, Hook_OverrideView);
					ClientModeTable.HookIndex(TABLE::IClientMode::GetViewModelFOV, Hook_GetViewModelFOV);
					ClientModeTable.HookIndex(44, Hook_DoPostScreenSpaceEffects);

					if (!SteamGameCoordinatorTable.InitTable(Interfaces::SteamGameCoordinator())) {
#if ENABLE_DEBUG_FILE == 1
						CSX::Log::Add("\n[SteamGameCoordinator - failed to init!]");
#endif
						return false;
					}

					//works
					SteamGameCoordinatorTable.HookIndex(0, Hook_SendMessage);
					SteamGameCoordinatorTable.HookIndex(2, Hook_RetrieveMessage);

					if (!GameEventTable.InitTable(Interfaces::GameEvent())) {
#if ENABLE_DEBUG_FILE == 1
						CSX::Log::Add("\n[EventManager - failed to init!]");
#endif
						return false;
					}

					//works
					GameEventTable.HookIndex(TABLE::IGameEventManager2::FireEventClientSide, Hook_FireEventClientSideThink);

					if (!ClientTable.InitTable(Interfaces::Client())) {
#if ENABLE_DEBUG_FILE == 1
						CSX::Log::Add("\n[Client - failed to init!]");
#endif
						return false;
					}

					//works
					ClientTable.HookIndex(TABLE::IBaseClientDLL::FrameStageNotify, Hook_FrameStageNotify);

					if (!SoundTable.InitTable(Interfaces::Sound())) {
#if ENABLE_DEBUG_FILE == 1
						CSX::Log::Add("\n[Sound - failed to init!]");
#endif
						return false;
					}

					//works
					SoundTable.HookIndex(TABLE::IEngineSound::EmitSound2, Hook_EmitSound2);

					if (!ModelRenderTable.InitTable(Interfaces::ModelRender())) {
#if ENABLE_DEBUG_FILE == 1
						CSX::Log::Add("\n[Model - failed to init!]");
#endif
						return false;
					}

					//works
					ModelRenderTable.HookIndex(TABLE::IVModelRender::DrawModelExecute, Hook_DrawModelExecute);

					if (!SurfaceTable.InitTable(Interfaces::Surface())) {
#if ENABLE_DEBUG_FILE == 1
						CSX::Log::Add("\n[Surface - failed to init!]");
#endif
						return false;
					}

					//doesn't work
					SurfaceTable.HookIndex(TABLE::ISurface::PlaySound, Hook_PlaySound);

					//works
					SurfaceTable.HookIndex(TABLE::ISurface::LockCursor, Hook_LockCursor);

					if (Client::Initialize(g_pDevice)) {
#if ENABLE_DEBUG_FILE == 1
						CSX::Log::Add("\n[Client - initialized!]");
#endif
						return true;
					}
				}
			}
			return false;
		}

		void Shutdown() {
			IDirect3DDevice9Table.UnHook();
			SoundTable.UnHook();
			ClientModeTable.UnHook();
			GameEventTable.UnHook();
			ModelRenderTable.UnHook();
			ClientTable.UnHook();
			SurfaceTable.UnHook();
			EngineTable.UnHook();
			SteamGameCoordinatorTable.UnHook();
		}
	}
}

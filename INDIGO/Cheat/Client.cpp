/*
	WARNING: This cheat (like pretty much any other pastes out there) uses Virtual Method Table (VMT) hooking, which is now detected by Valve Anti-Cheat.
	We are NOT responsible for ANY bans that may occur during the process of using this cheat. This includes, but not limited to, VAC, Untrusted and
	Overwatch bans.
*/

#include "Client.h"
#include <ctime>

bool Aimbot, Triggerbot, Visuals, Misc, Radar, Colors;

float	
	SpaceLineOne = 140.f,
	SpaceLineTwo = 280.f,
	SpaceLineThr = 420.f;

namespace Client
{
	int	iScreenWidth = 0;
	int	iScreenHeight = 0;

	string BaseDir = "C:\\Indigo";
	string LogFile = "";
	string GuiFile = "";
	string IniFile = "";

	vector<string> ConfigList;

	Vector2D	g_vCenterScreen = Vector2D(0.f, 0.f);

	CPlayers*	g_pPlayers = nullptr;
	CRender*	g_pRender = nullptr;
	CGui*		g_pGui = nullptr;

	CAimbot*	g_pAimbot = nullptr;
	CTriggerbot* g_pTriggerbot = nullptr;
	CEsp*		g_pEsp = nullptr;
	CRadar*		g_pRadar = nullptr;
	CKnifebot*	g_pKnifebot = nullptr;
	CSkin*		g_pSkin = nullptr;
	CMisc*		g_pMisc = nullptr;
	CInventoryChanger* g_pInventoryChanger = nullptr;

	bool		bC4Timer = false;
	int			iC4Timer = 40;

	int			iWeaponID = 0;
	int			iWeaponSelectIndex = WEAPON_DEAGLE;
	int			iWeaponSelectSkinIndex = -1;

	void ReadConfigs(LPCTSTR lpszFileName)
	{
		if (!strstr(lpszFileName, "gui.smef"))
			ConfigList.push_back(lpszFileName);
	}

	void RefreshConfigs()
	{
		ConfigList.clear();
		string ConfigDir = BaseDir + "\\" + "*.smef";
		GuiFile = BaseDir + "\\" + "gui.smef";
		CreateDirectoryW(L"C:\\Indigo", NULL);
		SearchFiles(ConfigDir.c_str(), ReadConfigs, FALSE);
	}

	//Menu Stuff

	namespace Global
	{
		int MenuTab;
	}

	void TextColor(bool active)
	{
		auto& style = ImGui::GetStyle();

		if (active)
			style.Colors[ImGuiCol_Text] = ImVec4(0.98f, 0.98f, 0.98f, 1.f);
		else
			style.Colors[ImGuiCol_Text] = ImVec4(0.39f, 0.39f, 0.39f, 1.f);
	}

	void DefineTab()
	{
		if (Global::MenuTab == 0)
		{
			Aimbot = true;
			Triggerbot = false;
			Visuals = false;
			Misc = false;
			Colors = false;
			Radar = false;
		}
		else if (Global::MenuTab == 1)
		{
			Aimbot = false;
			Triggerbot = true;
			Visuals = false;
			Misc = false;
			Colors = false;
			Radar = false;
		}
		else if (Global::MenuTab == 2)
		{
			Aimbot = false;
			Triggerbot = false;
			Visuals = true;
			Misc = false;
			Colors = false;
			Radar = false;
		}
		else if (Global::MenuTab == 3)
		{
			Aimbot = false;
			Triggerbot = false;
			Visuals = false;
			Misc = false;
			Colors = false;
			Radar = true;
		}
		else if (Global::MenuTab == 4)
		{
			Aimbot = false;
			Triggerbot = false;
			Visuals = false;
			Misc = true;
			Colors = false;
			Radar = false;
		}
		else if (Global::MenuTab == 5)
		{
			Aimbot = false;
			Triggerbot = false;
			Visuals = false;
			Misc = false;
			Colors = true;
			Radar = false;
		}
	}

	void BtnNormal()
	{
		auto& style = ImGui::GetStyle();

		style.Colors[ImGuiCol_Button] = ImVec4(0.10, 0.10, 0.10, .98f);
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.20f, 0.20f, 0.20f, 1.f);
		style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.33f, 0.33f, 0.33f, 1.f);
	}

	void BtnColor(bool active)
	{
		auto& style = ImGui::GetStyle();

		if (active)
		{
			style.Colors[ImGuiCol_Button] = ImVec4(.78f, 0.f, 0.f, 1.f);
			style.Colors[ImGuiCol_ButtonHovered] = ImVec4(.78f, 0.f, 0.f, 1.f);
			style.Colors[ImGuiCol_ButtonActive] = ImVec4(.78f, 0.f, 0.f, 1.f);
		}
		else
		{
			style.Colors[ImGuiCol_Button] = ImVec4(0.12, 0.12, 0.12, .98f);
			style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.12, 0.12, 0.12, .98f);
			style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.12, 0.12, 0.12, .98f);
		}
	}

	bool Initialize(IDirect3DDevice9* pDevice)
	{
		g_pPlayers = new CPlayers();
		g_pRender = new CRender(pDevice);
		g_pGui = new CGui();

		g_pAimbot = new CAimbot();
		g_pTriggerbot = new CTriggerbot();
		g_pEsp = new CEsp();
		g_pKnifebot = new CKnifebot();
		g_pRadar = new CRadar();
	
		g_pSkin = new CSkin();
		g_pMisc = new CMisc();
		g_pInventoryChanger = new CInventoryChanger();

		GuiFile = BaseDir + "\\" + "gui.smef";
		IniFile = BaseDir + "\\" + "settings.smef";

		g_pSkin->InitalizeSkins();

		Settings::LoadSettings(IniFile);

		iWeaponSelectSkinIndex = GetWeaponSkinIndexFromPaintKit(g_SkinChangerCfg[iWeaponSelectIndex].nFallbackPaintKit);

		g_pGui->GUI_Init(pDevice);

		RefreshConfigs();

		SendClientHello();

		EventLog->AddToLog("Successfully injected! Enjoy cheating with smef's Indigo");
		EventLog->AddToLog("We are aware of some saving bugs and will be fixed soon!");
		EventLog->AddToLog("Older configs since 4.4.3 are now BROKEN!");

		return true;
	}

	void SendMMHello()
	{
		CMsgGCCStrike15_v2_MatchmakingClient2GCHello Message;

		void* ptr = malloc(Message.ByteSize() + 8);
		if (!ptr)
			return;

		auto unMsgType = k_EMsgGCCStrike15_v2_MatchmakingClient2GCHello | ((DWORD)1 << 31);
		((uint32_t*)ptr)[0] = unMsgType;
		((uint32_t*)ptr)[1] = 0;

		Message.SerializeToArray((void*)((DWORD)ptr + 8), Message.ByteSize());
		bool result = Interfaces::SteamGameCoordinator()->SendMessage(unMsgType, ptr, Message.ByteSize() + 8) == k_EGCResultOK;

		free(ptr);
	}
	void SendClientHello()
	{
		CMsgClientHello Message;

		Message.set_client_session_need(1);
		Message.clear_socache_have_versions();

		void* ptr = malloc(Message.ByteSize() + 8);
		if (!ptr)
			return;

		((uint32_t*)ptr)[0] = k_EMsgGCClientHello | ((DWORD)1 << 31);
		((uint32_t*)ptr)[1] = 0;

		Message.SerializeToArray((void*)((DWORD)ptr + 8), Message.ByteSize());
		bool result = Interfaces::SteamGameCoordinator()->SendMessage(k_EMsgGCClientHello | ((DWORD)1 << 31), ptr, Message.ByteSize() + 8) == k_EGCResultOK;

		free(ptr);
	}

	void Shutdown()
	{
		DELETE_MOD(g_pPlayers);
		DELETE_MOD(g_pRender);
		DELETE_MOD(g_pGui);

		DELETE_MOD(g_pAimbot);
		DELETE_MOD(g_pKnifebot);
		DELETE_MOD(g_pTriggerbot);
		DELETE_MOD(g_pEsp);
		DELETE_MOD(g_pRadar);
		DELETE_MOD(g_pSkin);
		DELETE_MOD(g_pMisc);
	}

	void OnRender()
	{
		if (g_pRender && !Interfaces::Engine()->IsTakingScreenshot() && Interfaces::Engine()->IsActiveApp())
		{
			g_pRender->BeginRender();

			if (g_pGui)
				g_pGui->GUI_Draw_Elements();

			Interfaces::Engine()->GetScreenSize(iScreenWidth, iScreenHeight);

			g_vCenterScreen.x = iScreenWidth / 2.f;
			g_vCenterScreen.y = iScreenHeight / 2.f;

			EventLog->DrawLogs();

			if (Settings::Esp::esp_Watermark)
			{
				static float rainbow;
				rainbow += 0.005f;
				if (rainbow > 1.f) rainbow = 0.f;
				g_pRender->Text(30, 30, false, true, Color::FromHSB(rainbow, 1.f, 1.f), WATER_MARK);
			}

			g_pGui->MenuColor();

			if (g_pEsp)
				g_pEsp->OnRender();

			if (g_pMisc)
			{
				g_pMisc->OnRender();
				g_pMisc->OnRenderSpectatorList();
			}

			std::time_t result = std::time(nullptr);
			
			if (Settings::Misc::misc_Spectators)
			{
				g_pRender->Text(150, 500, false, true, Color::White(), "Spectators List:");
				g_pRender->Text(150, 501, false, true, Color::White(), "____________");
			}

			if (Settings::Esp::esp_Time)
				g_pRender->Text(30, 45, false, true, Color::White(), std::asctime(std::localtime(&result)));

			//if (Settings::Aimbot::aim_Backtrack)
				//g_pRender->Text(15, 66, false, true, Color::White(), to_string(BacktrackTicks()).c_str());
			g_pRender->EndRender();
		}
	}

	void OnLostDevice()
	{
		if (g_pRender)
			g_pRender->OnLostDevice();

		if (g_pGui)
			ImGui_ImplDX9_InvalidateDeviceObjects();
	}

	void OnResetDevice()
	{
		if (g_pRender)
			g_pRender->OnResetDevice();

		if (g_pGui)
			ImGui_ImplDX9_CreateDeviceObjects();
	}

	void OnRetrieveMessage(void* ecx, void* edx, uint32_t *punMsgType, void *pubDest, uint32_t cubDest, uint32_t *pcubMsgSize)
	{
		g_pInventoryChanger->PostRetrieveMessage(punMsgType, pubDest, cubDest, pcubMsgSize);
	}
	void OnSendMessage(void* ecx, void* edx, uint32_t unMsgType, const void* pubData, uint32_t cubData)
	{
		void* pubDataMutable = const_cast<void*>(pubData);
		g_pInventoryChanger->PreSendMessage(unMsgType, pubDataMutable, cubData);
	}

	void OnCreateMove(CUserCmd* pCmd, bool& bSendPacket)
	{
		if (g_pPlayers && Interfaces::Engine()->IsInGame())
		{
			g_pPlayers->Update();

			if (g_pEsp)
				g_pEsp->OnCreateMove(pCmd);

			if (IsLocalAlive())
			{
				QAngle view;
				Interfaces::Engine()->GetViewAngles(view);
				
					if (bSendPacket)
					Settings::Misc::qLastTickAngle = pCmd->viewangles;
				
				if (!bIsGuiVisible)
				{
					int iWeaponSettingsSelectID = GetWeaponSettingsSelectID();

					if (iWeaponSettingsSelectID >= 0)
						iWeaponID = iWeaponSettingsSelectID;
				}

				if (g_pAimbot)
					g_pAimbot->OnCreateMove(pCmd, g_pPlayers->GetLocal());

				if (g_pTriggerbot)
					g_pTriggerbot->OnCreateMove(pCmd, g_pPlayers->GetLocal());

				if (g_pKnifebot)
					g_pKnifebot->OnCreateMove(pCmd);

				if (g_pMisc)
					g_pMisc->OnCreateMove(pCmd);

				if (Settings::Aimbot::aim_Backtrack)
					 backtracking->legitBackTrack(pCmd);
				
				if (Settings::Misc::misc_LegitAA)
					 AntiAim().LegitAA(pCmd, bSendPacket);
				
				correct_movement(view, pCmd, pCmd->Move.x, pCmd->Move.y);
				if (stub_68616b65) if(!sanitize_angles(pCmd->viewangles)) return;
			}
		}
	}

	void OnFireEventClientSideThink(IGameEvent* pEvent)
	{
		if (!strcmp(pEvent->GetName(), "player_connect_full") ||
			!strcmp(pEvent->GetName(), "round_start") ||
			!strcmp(pEvent->GetName(), "cs_game_disconnected"))
		{
			if (g_pPlayers)
				g_pPlayers->Clear();

			if (g_pEsp)
				g_pEsp->OnReset();
		}

		if (Interfaces::Engine()->IsConnected())
		{
			hitmarker::singleton()->initialize();

			if (g_pEsp)
				g_pEsp->OnEvents(pEvent);

			if (g_pSkin)
				g_pSkin->OnEvents(pEvent);
		}
	}

	void OnFrameStageNotify(ClientFrameStage_t Stage)
	{
		if (Interfaces::Engine()->IsInGame() && Interfaces::Engine()->IsConnected())
		{
			SpoofedConvar* sv_cheats_spoofed = nullptr;
			if (Settings::Untrusted)
			{
				if (sv_cheats_spoofed == nullptr)
					sv_cheats_spoofed = new SpoofedConvar(Interfaces::GetConVar()->FindVar("sv_cheats"));
				sv_cheats_spoofed->SetInt(1);
			}
			else 
			{
				if (sv_cheats_spoofed != nullptr) {
					sv_cheats_spoofed->SetInt(0);
					sv_cheats_spoofed->~SpoofedConvar();
					DELETE_MOD(sv_cheats_spoofed);
				}
			}
			if (g_pMisc)
				g_pMisc->FrameStageNotify(Stage);

			Skin_OnFrameStageNotify(Stage);
			Gloves_OnFrameStageNotify(Stage);
		}
	}

	void OnDrawModelExecute(IMatRenderContext* ctx, const DrawModelState_t &state,
		const ModelRenderInfo_t &pInfo, matrix3x4_t *pCustomBoneToWorld)
	{
		if (Interfaces::Engine()->IsInGame() && ctx && pCustomBoneToWorld)
		{
			if (g_pEsp)
				g_pEsp->OnDrawModelExecute(ctx, state, pInfo, pCustomBoneToWorld);

			if (g_pMisc)
				g_pMisc->OnDrawModelExecute();
		}
	}

	void OnPlaySound(const Vector* pOrigin, const char* pszSoundName)
	{
		if (!pszSoundName || !Interfaces::Engine()->IsInGame())
			return;

		if (!strstr(pszSoundName, "bulletLtoR") &&
			!strstr(pszSoundName, "rics/ric") &&
			!strstr(pszSoundName, "impact_bullet"))
		{
			if (g_pEsp && IsLocalAlive() && Settings::Esp::esp_Sound && pOrigin)
			{
				if (!GetVisibleOrigin(*pOrigin))
					g_pEsp->SoundEsp.AddSound(*pOrigin);
			}
		}
	}

	void OnPlaySound(const char* pszSoundName)
	{
		if (g_pMisc)
			g_pMisc->OnPlaySound(pszSoundName);
	}

	void OnOverrideView(CViewSetup* pSetup)
	{
		if (g_pMisc)
			g_pMisc->OnOverrideView(pSetup);
	}

	void OnGetViewModelFOV(float& fov)
	{
		if (g_pMisc)
			g_pMisc->OnGetViewModelFOV(fov);
	}

	void DrawAimbot()
	{
		ImGui::BeginGroup();

		static int otherpages = 0;
		const char* tabNames[] = { // change these to what you like
			"Aimbot Options",
			"Weapon Options", 
			"Trigger Options" 
		};

		static int tabOrder[] = { 0 , 1 , 2 };
		const bool tabChanged = ImGui::TabLabels(tabNames, sizeof(tabNames) / sizeof(tabNames[0]), otherpages, tabOrder);

		if (otherpages == 0)
		{
			if (Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_FovType > 1)
				Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_FovType = 1;

			if (Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_BestHit > 1)
				Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_BestHit = 1;

			if (Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_Spot > 5)
				Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_Spot = 5;

			ImGui::PushItemWidth(110.f);
			ImGui::Text("Current Weapon: ");
			ImGui::SameLine();
			ImGui::Combo("##AimWeapon", &iWeaponID, pWeaponData, IM_ARRAYSIZE(pWeaponData));
			ImGui::PopItemWidth();
			ImGui::Separator();
			ImGui::Spacing();

			ImGui::Checkbox("Active", &Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_Active);

			if (iWeaponID <= 9)
			{
				ImGui::SameLine(SpaceLineOne);
				ImGui::Checkbox("Autopistol", &Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_AutoPistol);
			}
			ImGui::PushItemWidth(362.f);
			ImGui::SliderInt("Smooth", &Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_Smooth, 1, 300);
			ImGui::SliderInt("Fov", &Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_Fov, 1, 300);
			ImGui::PopItemWidth();

			const char* AimFovType[] = { "Dynamic" , "Static" };
			ImGui::PushItemWidth(362.f);
			ImGui::Combo("Fov Type", &Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_FovType, AimFovType, IM_ARRAYSIZE(AimFovType));
			ImGui::PopItemWidth();

			const char* BestHit[] = { "Prefer Manually" , "Nearest" , "Ignore Head" , "Ignore Arms/Legs" , "Ignore Arms/Legs/Neck" };
			ImGui::PushItemWidth(362.f);
			ImGui::Combo("Hit Scan", &Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_BestHit, BestHit, IM_ARRAYSIZE(BestHit));
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("If 'Prefer Manually' then use Hitbox");

			ImGui::PopItemWidth();

			const char* Aimspot[] = { "Head" , "Neck" , "Body" , "Thorax" , "Chest" , "Right Thigh" ,
				"Left Thigh" , "Right Hand" , "Left Hand" , "Right Upperarm" , "Right Forearm" ,
				"Left Upperarm" , "Left Forearm" };
			ImGui::PushItemWidth(362.f);
			ImGui::Combo("Hitbox", &Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_Spot, Aimspot, IM_ARRAYSIZE(Aimspot));
			ImGui::PopItemWidth();

			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();

			ImGui::PushItemWidth(362.f);
			ImGui::SliderInt("ShotDelay", &Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_Delay, 0, 200);
			ImGui::SliderInt("RCS", &Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_Rcs, 0, 100);
			ImGui::PopItemWidth();

			ImGui::Spacing();
			ImGui::Separator();

			if (iWeaponID >= 10 && iWeaponID <= 30)
			{
				ImGui::PushItemWidth(362.f);
				ImGui::SliderInt("RCS FOV", &Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_RcsFov, 1, 300);
				ImGui::PushItemWidth(362.f);
				ImGui::SliderInt("RCS Smooth", &Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_RcsSmooth, 1, 300);
				ImGui::PopItemWidth();

				const char* ClampType[] = { "On Target" , "On Shot" , "On Shot & Target" };
				ImGui::PushItemWidth(362.f);
				ImGui::Combo("RCS Clamp", &Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_RcsClampType, ClampType, IM_ARRAYSIZE(ClampType));
				ImGui::PopItemWidth();
			}
		}

		if (otherpages == 1)
		{
			ImGui::Text("Aimbot Options");
			ImGui::Separator();
			ImGui::Spacing();
			ImGui::Checkbox("Friendly Fire", &Settings::Aimbot::aim_Deathmatch);
			ImGui::Checkbox("Auto Wall", &Settings::Aimbot::aim_WallAttack);
			ImGui::Checkbox("Check Smoke", &Settings::Aimbot::aim_CheckSmoke);
			ImGui::Checkbox("In Air", &Settings::Aimbot::aim_AntiJump);

			ImGui::Text("Backtrack Options");
			ImGui::Separator();
			ImGui::Spacing();
			ImGui::Checkbox("Backtrack", &Settings::Aimbot::aim_Backtrack);
			ImGui::PushItemWidth(362.f);
			ImGui::SliderInt("Time (ms)", &Settings::Aimbot::aim_Backtracktime, 1, 200);

			ImGui::Text("Anti Aim Options");
			ImGui::Separator();
			ImGui::Spacing();
			ImGui::Checkbox("Legit Anti-Aim", &Settings::Misc::misc_LegitAA);
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("left + right arrow key to change angles");
			ImGui::Checkbox("Silent Aim", &Settings::Misc::misc_LegitAAToggle);

			ImGui::Text("Aimbot ESP");
			ImGui::Separator();
			ImGui::Spacing();
			ImGui::Checkbox("Draw FOV", &Settings::Aimbot::aim_DrawFov);
			ImGui::Checkbox("Draw Hitbox", &Settings::Aimbot::aim_DrawSpot);
			ImGui::Checkbox("Draw Backtrack Ticks", &Settings::Aimbot::aim_DrawBacktrack);

			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();
		}
		if (otherpages == 2)
		{
			const char* TriggerEnable[] = { "Disable" , "FOV" , "Trace" };
			ImGui::PushItemWidth(130.f);
			ImGui::Combo("Type", &Settings::Triggerbot::trigger_Enable, TriggerEnable, IM_ARRAYSIZE(TriggerEnable));

			ImGui::PushItemWidth(110.f);
			ImGui::Text("Current Weapon: ");
			ImGui::SameLine();
			ImGui::Combo("##AimWeapon", &iWeaponID, pWeaponData, IM_ARRAYSIZE(pWeaponData));
			ImGui::PopItemWidth();
			ImGui::Separator();
			ImGui::Spacing();

			ImGui::Separator();
			ImGui::Spacing();

			ImGui::Checkbox("Friendly Fire", &Settings::Triggerbot::trigger_Deathmatch);
			ImGui::SameLine(SpaceLineOne);
			ImGui::Checkbox("Auto Wall", &Settings::Triggerbot::trigger_WallAttack);
			ImGui::SameLine(SpaceLineTwo);
			ImGui::Checkbox("Quick Scope", &Settings::Triggerbot::trigger_FastZoom);
			ImGui::SameLine(SpaceLineThr);
			ImGui::Checkbox("Head Only", &Settings::Triggerbot::weapon_trigger_settings[iWeaponID].trigger_HeadOnly);


			ImGui::Checkbox("Check Smoke", &Settings::Triggerbot::trigger_SmokCheck);
			ImGui::SameLine(SpaceLineOne);
			ImGui::Checkbox("Draw FOV", &Settings::Triggerbot::trigger_DrawFov);
			ImGui::SameLine(SpaceLineTwo);
			ImGui::Checkbox("Draw Hitbox", &Settings::Triggerbot::trigger_DrawSpot);
			ImGui::SameLine(SpaceLineThr);
			ImGui::Checkbox("Draw FOV Assist", &Settings::Triggerbot::trigger_DrawFovAssist);

			ImGui::Separator();
			ImGui::Spacing();

			const char* items1[] = { CVAR_KEY_MOUSE5 , CVAR_KEY_MOUSE4 , CVAR_KEY_MOUSE3 };
			ImGui::PushItemWidth(130.f);
			ImGui::Combo("Key", &Settings::Triggerbot::trigger_Key, items1, IM_ARRAYSIZE(items1));
			ImGui::PopItemWidth();
			ImGui::SameLine(SpaceLineTwo);

			const char* items2[] = { "Held" , "Toggle" };
			ImGui::PushItemWidth(130.f);
			ImGui::Combo("Key Mode", &Settings::Triggerbot::trigger_KeyMode, items2, IM_ARRAYSIZE(items2));
			ImGui::PopItemWidth();

			ImGui::Separator();
			ImGui::Spacing();

			ImGui::PushItemWidth(362.f);
			//ImGui::SliderInt("Min Distance", &Settings::Triggerbot::weapon_trigger_settings[iWeaponID].trigger_DistanceMin, 0, 5000);
			//ImGui::SliderInt("Max Distance", &Settings::Triggerbot::weapon_trigger_settings[iWeaponID].trigger_DistanceMax, 0, 5000);
			ImGui::SliderInt("FOV", &Settings::Triggerbot::weapon_trigger_settings[iWeaponID].trigger_Fov, 1, 100);
			ImGui::SliderInt("First Shot Delay", &Settings::Triggerbot::weapon_trigger_settings[iWeaponID].trigger_DelayBefore, 0, 200);
			ImGui::SliderInt("Next Shot Delay", &Settings::Triggerbot::weapon_trigger_settings[iWeaponID].trigger_DelayAfter, 0, 1000);
			ImGui::PopItemWidth();

			ImGui::Separator();
			ImGui::Spacing();

			const char* AssistMode[] = { "Disable" , "One Shot" , "Auto" };
			ImGui::PushItemWidth(130.f);
			ImGui::Combo("Assist", &Settings::Triggerbot::weapon_trigger_settings[iWeaponID].trigger_Assist, AssistMode, IM_ARRAYSIZE(AssistMode));
			ImGui::PopItemWidth();

			ImGui::SameLine(SpaceLineTwo);
			const char* AssistFovType[] = { "Dynamic" , "Static" };
			ImGui::PushItemWidth(130.f);
			ImGui::Combo("Assist FOV Type", &Settings::Triggerbot::weapon_trigger_settings[iWeaponID].trigger_AssistFovType, AssistFovType, IM_ARRAYSIZE(AssistFovType));
			ImGui::PopItemWidth();
			ImGui::PushItemWidth(355.f);
			const char* HitGroup[] = { "All" , "Head + Body" , "Head" };
			ImGui::Combo("HitGroup", &Settings::Triggerbot::weapon_trigger_settings[iWeaponID].trigger_HitGroup, HitGroup, IM_ARRAYSIZE(HitGroup));
			ImGui::PopItemWidth();
			ImGui::Separator();
			ImGui::Spacing();

			ImGui::PushItemWidth(362.f);
			ImGui::SliderInt("Assist RCS", &Settings::Triggerbot::weapon_trigger_settings[iWeaponID].trigger_AssistRcs, 0, 100);
			ImGui::SliderInt("Assist FOV", &Settings::Triggerbot::weapon_trigger_settings[iWeaponID].trigger_AssistFov, 1, 300);
			ImGui::SliderInt("Assist Smooth", &Settings::Triggerbot::weapon_trigger_settings[iWeaponID].trigger_AssistSmooth, 1, 300);
			ImGui::PopItemWidth();

		}
	}

	void DrawSkins() // Skins
	{
		static int otherpages = 0;
		const char* tabNames[] = { // change these to what you like
			"Skinchanger", 
			"Inventory", 
			"Profile" 
		};
		static int tabOrder[] = { 0 , 1 , 2 };
		const bool tabChanged = ImGui::TabLabels(tabNames, sizeof(tabNames) / sizeof(tabNames[0]), otherpages, tabOrder);

		if (otherpages == 0)
		{
			const char* knife_models_items[] =
			{
				"Default","Bayonet","Flip","Gut","Karambit" ,"M9 Bayonet",
				"Huntsman","Falchion","Bowie","Butterfly","Shadow Daggers"
			};

			const char* quality_items[] =
			{
				"Use PaintKit Default",
				"Consumer Grade (White)",
				"Industrial Grade (Light Blue)",
				"Mil-Spec (Darker Blue)",
				"Restricted (Purple)",
				"Classified (Pinkish Purple)",
				"Covert (Red)",
				"Exceedingly Rare (Gold)",
			};
			//[enc_string_enable /]
			const char* GlovesType[] = {
				"Default",
				"Bloodhound",
				"Default (Terrorists)",
				"Default (Counter-Terrorists)",
				"Sporty",
				"Slick",
				"Handwrap",
				"Motorcycle",
				"Specialist",
				"Hydra"
			};

			ImGui::Separator();
			ImGui::Text("Knife Changer");
			ImGui::Separator();

			static int iSelectKnifeCTSkinIndex = -1;
			static int iSelectKnifeTTSkinIndex = -1;

			int iKnifeCTModelIndex = Settings::Skin::knf_ct_model;
			int iKnifeTTModelIndex = Settings::Skin::knf_tt_model;

			static int iOldKnifeCTModelIndex = -1;
			static int iOldKnifeTTModelIndex = -1;

			if (iOldKnifeCTModelIndex != iKnifeCTModelIndex && Settings::Skin::knf_ct_model)
				iSelectKnifeCTSkinIndex = GetKnifeSkinIndexFromPaintKit(Settings::Skin::knf_ct_skin, false);

			if (iOldKnifeTTModelIndex != iKnifeTTModelIndex && Settings::Skin::knf_tt_model)
				iSelectKnifeTTSkinIndex = GetKnifeSkinIndexFromPaintKit(Settings::Skin::knf_ct_skin, true);

			iOldKnifeCTModelIndex = iKnifeCTModelIndex;
			iOldKnifeTTModelIndex = iKnifeTTModelIndex;

			string KnifeCTModel = knife_models_items[Settings::Skin::knf_ct_model];
			string KnifeTTModel = knife_models_items[Settings::Skin::knf_tt_model];

			KnifeCTModel += " Skin##KCT";
			KnifeTTModel += " Skin##KTT";

			ImGui::PushItemWidth(362.f);
			ImGui::Combo("Knife CT Model", &Settings::Skin::knf_ct_model, knife_models_items, IM_ARRAYSIZE(knife_models_items));
			ImGui::PushItemWidth(362.f);
			ImGui::ComboBoxArray("Knife CT Skin", &iSelectKnifeCTSkinIndex, KnifeSkins[iKnifeCTModelIndex].SkinNames);
			ImGui::PushItemWidth(362.f);
			ImGui::SliderFloat("Knife CT Wear", &g_SkinChangerCfg[WEAPON_KNIFE].flFallbackWear, 0.001f, 1.f);
			ImGui::Spacing();

			ImGui::PushItemWidth(362.f);
			ImGui::Combo("Knife T Model", &Settings::Skin::knf_tt_model, knife_models_items, IM_ARRAYSIZE(knife_models_items));
			ImGui::PushItemWidth(362.f);
			ImGui::ComboBoxArray("Knife T Skin", &iSelectKnifeTTSkinIndex, KnifeSkins[iKnifeTTModelIndex].SkinNames);
			ImGui::PushItemWidth(362.f);
			ImGui::SliderFloat("Knife T Wear", &g_SkinChangerCfg[WEAPON_KNIFE_T].flFallbackWear, 0.001f, 1.f);

			ImGui::Separator();
			ImGui::Text("Skin Changer");
			ImGui::Separator();

			static int iOldWeaponID = -1;

			ImGui::Combo("Weapon##WeaponSelect", &iWeaponID, pWeaponData, IM_ARRAYSIZE(pWeaponData));

			iWeaponSelectIndex = pWeaponItemIndexData[iWeaponID];

			if (iOldWeaponID != iWeaponID)
				iWeaponSelectSkinIndex = GetWeaponSkinIndexFromPaintKit(g_SkinChangerCfg[iWeaponSelectIndex].nFallbackPaintKit);

			iOldWeaponID = iWeaponID;

			string WeaponSkin = pWeaponData[iWeaponID];
			WeaponSkin += " Skin";
			static int weaponskin = 0;
			ImGui::PushItemWidth(362.f);
			ImGui::ComboBoxArray(WeaponSkin.c_str(), &iWeaponSelectSkinIndex, WeaponSkins[iWeaponID].SkinNames);
			ImGui::PushItemWidth(362.f);
			ImGui::Combo("Weapon Quality", &g_SkinChangerCfg[pWeaponItemIndexData[iWeaponID]].iEntityQuality, quality_items, IM_ARRAYSIZE(quality_items));
			ImGui::PushItemWidth(362.f);
			ImGui::SliderFloat("Weapon Wear", &g_SkinChangerCfg[pWeaponItemIndexData[iWeaponID]].flFallbackWear, 0.001f, 1.f);
			ImGui::PushItemWidth(362.f);
			ImGui::InputInt("Weapon StatTrak", &g_SkinChangerCfg[pWeaponItemIndexData[iWeaponID]].nFallbackStatTrak, 1, 100, ImGuiInputTextFlags_CharsDecimal);

			ImGui::Separator();
			ImGui::Text("Glove Changer");
			ImGui::Separator();

			ImGui::PushItemWidth(362.f);
			ImGui::Combo("Gloves Type", &Settings::Skin::gloves_model, GlovesType, IM_ARRAYSIZE(GlovesType));
			ImGui::PushItemWidth(362.f);
			ImGui::ComboBoxArray("Gloves Skin", &Settings::Skin::gloves_skin, GloveSkin[Settings::Skin::gloves_model].Names);
			ImGui::Separator();


			if (ImGui::Button("Apply##Skin"))
			{
				if (iWeaponSelectSkinIndex >= 0) {
					g_SkinChangerCfg[iWeaponSelectIndex].nFallbackPaintKit = WeaponSkins[iWeaponID].SkinPaintKit[iWeaponSelectSkinIndex];
				}

				if (iSelectKnifeCTSkinIndex >= 0 && Settings::Skin::knf_ct_model > 0) {
					Settings::Skin::knf_ct_skin = KnifeSkins[iKnifeCTModelIndex].SkinPaintKit[iSelectKnifeCTSkinIndex];
				}
				else
				{
					Settings::Skin::knf_ct_skin = 0;
					iSelectKnifeCTSkinIndex = -1;
				}

				if (iSelectKnifeTTSkinIndex >= 0 && Settings::Skin::knf_tt_model > 0) {
					Settings::Skin::knf_tt_skin = KnifeSkins[iKnifeTTModelIndex].SkinPaintKit[iSelectKnifeTTSkinIndex];
				}
				else
				{
					Settings::Skin::knf_tt_skin = 0;
					iSelectKnifeTTSkinIndex = -1;
				}

				ForceFullUpdate();
			}

			//fix it if you want sticker changer
			/*

			if (ImGui::Checkbox("Sticker Changer", &Settings::Aimbot::weapon_aim_settings[iWeaponID].StickersEnabled))
				CL_FullUpdate();


			if (Settings::Aimbot::weapon_aim_settings[iWeaponID].StickersEnabled)
			{
				ImGui::Combo("Slot", &iSlot, Slot, ARRAYSIZE(Slot));
				ImGui::Combo("ID", &Settings::Aimbot::weapon_aim_settings[iWeaponID].Stickers[iSlot].iID, [](void* data, int idx, const char** out_text)
				{
					*out_text = k_stickers.at(idx).name.c_str();
					return true;
				}, nullptr, k_stickers.size());

				ImGui::SliderFloat("Wear ", &Settings::Aimbot::weapon_aim_settings[iWeaponID].Stickers[iSlot].flWear, 0.f, 1.f);
				ImGui::SliderInt("Rotation", &Settings::Aimbot::weapon_aim_settings[iWeaponID].Stickers[iSlot].iRotation, 0, 360);
			}


			if (ImGui::Button(("Apply"), ImVec2(93.f, 20.f)))
				CL_FullUpdate();
				*/
		}

		if (otherpages == 1)
		{
			//ImGui::Text("Other Changers");
			ImGui::Checkbox("Inventory Changer", &Settings::InvChanger::Inventory_Changer);
			if (Settings::InvChanger::Inventory_Changer) 
			{
				ImGui::Text("Skins");
				ImGui::Separator();
				ImGui::Spacing();
				ImGui::Columns(2, nullptr, false);
				static int itemidtmp = 0, itemDefinitionIndex = 0, paintKit = 0, paintkit_temp_skin = 0, paintkit_temp_gloves = 0, rarity = 0, seed = 0, raritypick = 0;
				static float wear = 0.f;
				const char* raritynames[] = { "Default (Gray)", "Consumer Grade (White)", "Industrial Grade (Light Blue)", "Mil-Spec (Darker Blue)", "Restricted (Purple)", "Classified (Pinkish Purple)", "Covert (Red)", "Exceedingly Rare (Gold)" };
				const char* itemnames[] = { "Desert Eagle", "Dual Berettas", "Five-Seven", "Glock-18", "AK-47", "AUG", "AWP", "FAMAS", "G3SG1", "Galil AR", "M249", "M4A4",
					"MAC-10", "P90", "UMP-45", "XM1014", "PP-Bizon", "MAG-7", "Negev", "Sawed-Off", "Tec-9", "P2000", "MP7", "MP9", "Nova", "P250", "SCAR-20", "SG 553", "SSG 08",
					"M4A1-S", "USP-S", "CZ75-Auto", "R8 Revolver", "Bayonet", "Flip Knife", "Gut Knife", "Karambit", "M9 Bayonet", "Huntsman Knife", "Falchion Knife", "Bowie Knife", "Butterfly Knife",
					"Shadow Daggers", "Sport Gloves", "Driver Gloves", "Hand Wraps", "Moto Gloves", "Specialist Gloves", "Hydra Gloves" };
				const int weapons_id[] = { WEAPON_DEAGLE, WEAPON_ELITE, WEAPON_FIVESEVEN, WEAPON_GLOCK, WEAPON_AK47, WEAPON_AUG, WEAPON_AWP, WEAPON_FAMAS, WEAPON_G3SG1, WEAPON_GALILAR, WEAPON_M249,
					WEAPON_M4A1, WEAPON_MAC10, WEAPON_P90, WEAPON_UMP45, WEAPON_XM1014, WEAPON_BIZON, WEAPON_MAG7, WEAPON_NEGEV, WEAPON_SAWEDOFF, WEAPON_TEC9, WEAPON_HKP2000, WEAPON_MP7, WEAPON_MP9,
					WEAPON_NOVA, WEAPON_P250, WEAPON_SCAR20, WEAPON_SG553, WEAPON_SSG08, WEAPON_M4A1_SILENCER, WEAPON_USP_SILENCER, WEAPON_CZ75A, WEAPON_REVOLVER, WEAPON_KNIFE_BAYONET, WEAPON_KNIFE_FLIP,
					WEAPON_KNIFE_GUT, WEAPON_KNIFE_KARAMBIT, WEAPON_KNIFE_M9_BAYONET, WEAPON_KNIFE_TACTICAL, WEAPON_KNIFE_FALCHION, WEAPON_KNIFE_SURVIVAL_BOWIE, WEAPON_KNIFE_SURVIVAL_BOWIE,
					WEAPON_KNIFE_BUTTERFLY, WEAPON_KNIFE_PUSH, 5030, 5031, 5032, 5033, 5034, 5035
				};
				ImGui::Combo(("Item"), &itemidtmp, itemnames, ARRAYSIZE(itemnames));
				itemDefinitionIndex = weapons_id[itemidtmp];

				if (itemDefinitionIndex < 5000) {
					ImGui::ComboBoxArray("Skin", &paintkit_temp_skin, KnifeSkins[0].SkinNames);
					paintKit = WeaponSkins[0].SkinPaintKit[paintkit_temp_skin];
				} else {
					ImGui::ComboBoxArray("Skin", &paintkit_temp_gloves, GloveSkin[0].Names);
					paintKit = GloveSkin[0].PaintKit[paintkit_temp_gloves];
				}

				ImGui::Combo(("Rarity"), &rarity, raritynames, ARRAYSIZE(raritynames));
				ImGui::InputInt("Seed", &seed);
				ImGui::SliderFloat("Wear", &wear, FLT_MIN, 1.f, "%.10f", 5);

				if (ImGui::Button("Add")) {
					Settings::InvChanger::weapons.insert(Settings::InvChanger::weapons.end(), { itemDefinitionIndex , paintKit , rarity , seed, wear });
					Settings::InvChanger::CustomWeaponCount = Settings::InvChanger::weapons.size();
				} ImGui::SameLine();

				if (ImGui::Button("Apply##Skin"))
					SendClientHello();

				ImGui::NextColumn();

				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 1, 1));
				ImGui::ListBoxHeader("Skins");
				int weapon_index = 0;
				for (auto weapon : Settings::InvChanger::weapons) {
					if (ImGui::Selectable(std::string(std::to_string(weapon.itemDefinitionIndex) + " " + std::to_string(weapon.paintKit)).c_str())) {
						Settings::InvChanger::weapons.erase(Settings::InvChanger::weapons.begin() + weapon_index);
						Settings::InvChanger::CustomWeaponCount = Settings::InvChanger::weapons.size();
					}
					weapon_index++;
				}
				ImGui::ListBoxFooter();
				ImGui::PopStyleColor();
				ImGui::Columns(1, nullptr, false);
			}
		}

			if (otherpages == 2)
			{
				ImGui::Checkbox("Profile Changer", &Settings::InvChanger::Profile_Info);
				if (Settings::InvChanger::Profile_Info)
				{
					const char* MMRank[] = {
						"Silver I",
						"Silver II",
						"Silver III",
						"Silver IV",
						"Silver Elite",
						"Silver Elite Master",

						"Gold Nova I",
						"Gold Nova II",
						"Gold Nova III",
						"Gold Nova Master",

						"Master Guardian I",
						"Master Guardian II",
						"Master Guardian Elite",
						"Distinguished Master Guardian",

						"Legendary Eagle",
						"Legendary Eagle Master",
						"Supreme Master First Class",
						"The Global Elite" };
					static int selected = 0;
					ImGui::Combo(("Rank"), &selected, MMRank, ARRAYSIZE(MMRank));
					Settings::InvChanger::Profile_Info_Rank = selected + 1; // could you be less retarded?

					ImGui::SliderInt("Level", &Settings::InvChanger::Profile_Info_Level, 1, 40);
					ImGui::InputInt("XP", &Settings::InvChanger::Profile_Info_XP);
					ImGui::InputInt("Wins", &Settings::InvChanger::Profile_Info_Win);
					ImGui::Spacing();
					ImGui::Text("Commends");
					ImGui::InputInt("Friendly", &Settings::InvChanger::Profile_Info_Friendly);
					ImGui::InputInt("Leader", &Settings::InvChanger::Profile_Info_Leader);
					ImGui::InputInt("Teacher", &Settings::InvChanger::Profile_Info_Teacher);
					ImGui::Spacing();
					if (ImGui::Button("Apply Changes"))
						SendMMHello();
				}
			}
	}

	/*void DrawTrigger() // Trigger
	{
		const char* TriggerEnable[] = { "Disable" , "Fov" , "Trace" };
		ImGui::PushItemWidth(80.f);
		ImGui::Combo("Enable", &Settings::Triggerbot::trigger_Enable, TriggerEnable, IM_ARRAYSIZE(TriggerEnable));
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		ImGui::Checkbox("Deathmatch", &Settings::Triggerbot::trigger_Deathmatch);
		ImGui::Checkbox("WallAttack", &Settings::Triggerbot::trigger_WallAttack);
		ImGui::Checkbox("FastZoom", &Settings::Triggerbot::trigger_FastZoom);

		ImGui::Checkbox("SmokCheck", &Settings::Triggerbot::trigger_SmokCheck);
		ImGui::Checkbox("DrawFov", &Settings::Triggerbot::trigger_DrawFov);
		ImGui::Checkbox("DrawSpot", &Settings::Triggerbot::trigger_DrawSpot);
		ImGui::Checkbox("DrawFovAssist", &Settings::Triggerbot::trigger_DrawFovAssist);

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		const char* items1[] = { CVAR_KEY_MOUSE3 , CVAR_KEY_MOUSE4 , CVAR_KEY_MOUSE5 };
		ImGui::PushItemWidth(80.f);
		ImGui::Combo("Key", &Settings::Triggerbot::trigger_Key, items1, IM_ARRAYSIZE(items1));
		ImGui::PopItemWidth();
		ImGui::SameLine();

		const char* items2[] = { "Hold" , "Press" };
		ImGui::PushItemWidth(80.f);
		ImGui::Combo("Key Mode", &Settings::Triggerbot::trigger_KeyMode, items2, IM_ARRAYSIZE(items2));
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushItemWidth(110.f);
		ImGui::Combo("Weapon", &iWeaponID, pWeaponData, IM_ARRAYSIZE(pWeaponData));
		ImGui::PopItemWidth();

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		ImGui::PushItemWidth(362.f);
		ImGui::SliderInt("Min Disstance", &Settings::Triggerbot::weapon_trigger_settings[iWeaponID].trigger_DistanceMin, 0, 5000);
		ImGui::SliderInt("Max Disstance", &Settings::Triggerbot::weapon_trigger_settings[iWeaponID].trigger_DistanceMax, 0, 5000);
		ImGui::SliderInt("Fov", &Settings::Triggerbot::weapon_trigger_settings[iWeaponID].trigger_Fov, 1, 100);
		ImGui::SliderInt("Delay Before", &Settings::Triggerbot::weapon_trigger_settings[iWeaponID].trigger_DelayBefore, 0, 200);
		ImGui::SliderInt("Delay After", &Settings::Triggerbot::weapon_trigger_settings[iWeaponID].trigger_DelayAfter, 0, 1000);
		ImGui::PopItemWidth();

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		ImGui::Checkbox("HeadOnly", &Settings::Triggerbot::weapon_trigger_settings[iWeaponID].trigger_HeadOnly);
		ImGui::SameLine();

		const char* AssistMode[] = { "Disable" , "One Shot" , "Auto" };
		ImGui::PushItemWidth(80.f);
		ImGui::Combo("Assist", &Settings::Triggerbot::weapon_trigger_settings[iWeaponID].trigger_Assist, AssistMode, IM_ARRAYSIZE(AssistMode));
		ImGui::PopItemWidth();
		ImGui::SameLine();

		const char* AssistFovType[] = { "Dynamic" , "Static" };
		ImGui::PushItemWidth(80.f);
		ImGui::Combo("Assist Fov Type", &Settings::Triggerbot::weapon_trigger_settings[iWeaponID].trigger_AssistFovType, AssistFovType, IM_ARRAYSIZE(AssistFovType));
		ImGui::PopItemWidth();

		const char* HitGroup[] = { "All" , "Head + Body" , "Head" };
		ImGui::Combo("HitGroup", &Settings::Triggerbot::weapon_trigger_settings[iWeaponID].trigger_HitGroup, HitGroup, IM_ARRAYSIZE(HitGroup));
		//ImGui::PopItemWidth();

		ImGui::PushItemWidth(362.f);
		ImGui::SliderInt("Assist Rcs", &Settings::Triggerbot::weapon_trigger_settings[iWeaponID].trigger_AssistRcs, 0, 100);
		ImGui::SliderInt("Assist Fov", &Settings::Triggerbot::weapon_trigger_settings[iWeaponID].trigger_AssistFov, 1, 300);
		ImGui::SliderInt("Assist Smooth", &Settings::Triggerbot::weapon_trigger_settings[iWeaponID].trigger_AssistSmooth, 1, 300);
		ImGui::PopItemWidth();
	}*/
	void DrawVisuals() // Visuals
	{
		ImGui::BeginGroup();
		static int otherpages = 0;
		const char* tabNames[] = { // change these to what you like
			"Part I",
			"Part II",
			"Part III"
		};
		static int tabOrder[] = { 0 , 1 , 2 };
		const bool tabChanged = ImGui::TabLabels(tabNames, sizeof(tabNames) / sizeof(tabNames[0]), otherpages, tabOrder);

		if (otherpages == 0)
		{
			const char* items1[] = { "None" , "Box", "Corners" };

			ImGui::Text("Visuals");
			ImGui::Separator();
			ImGui::Spacing();

			ImGui::Checkbox("Team", &Settings::Esp::esp_Team);
			ImGui::SameLine(SpaceLineOne);
			ImGui::Checkbox("Enemy", &Settings::Esp::esp_Enemy);
			ImGui::SameLine(SpaceLineTwo);
			ImGui::Checkbox("Bomb", &Settings::Esp::esp_Bomb);
			ImGui::SameLine(SpaceLineThr);
			ImGui::Checkbox("Chicken", &Settings::Esp::esp_Chicken);

			ImGui::Checkbox("Sound", &Settings::Esp::esp_Sound);
			ImGui::SameLine(SpaceLineOne);
			ImGui::Checkbox("Line", &Settings::Esp::esp_Line);
			ImGui::SameLine(SpaceLineTwo);
			ImGui::Checkbox("OutLine", &Settings::Esp::esp_Outline);
			ImGui::SameLine(SpaceLineThr);
			ImGui::Checkbox("Name", &Settings::Esp::esp_Name);

			ImGui::Checkbox("Rank", &Settings::Esp::esp_Rank);
			ImGui::SameLine(SpaceLineOne);
			ImGui::Checkbox("Weapon", &Settings::Esp::esp_Weapon);
			ImGui::SameLine(SpaceLineTwo);
			ImGui::Checkbox("Ammo", &Settings::Esp::esp_Ammo);
			ImGui::SameLine(SpaceLineThr);
			ImGui::Checkbox("Player Status", &Settings::Esp::esp_Infoz);

			ImGui::Checkbox("Distance", &Settings::Esp::esp_Distance);
			ImGui::SameLine(SpaceLineOne);
			ImGui::Checkbox("Skeleton", &Settings::Esp::esp_Skeleton);
			ImGui::SameLine(SpaceLineTwo);
			ImGui::Checkbox("World Weapon", &Settings::Esp::esp_WorldWeapons);
			ImGui::SameLine(SpaceLineThr);
			ImGui::Checkbox("Radar", &Settings::Radar::rad_Active);

			ImGui::Checkbox("World Grenade", &Settings::Esp::esp_WorldGrenade);
			ImGui::SameLine(SpaceLineOne);
			ImGui::Checkbox("Grenade Boxes", &Settings::Esp::esp_BoxNade);
			ImGui::SameLine(SpaceLineTwo);
			ImGui::Checkbox("Dynamic Lights", &Settings::Esp::esp_Dlightz);
			ImGui::SameLine(SpaceLineThr);
			ImGui::Checkbox("Watermarks", &Settings::Esp::esp_Watermark);
			Settings::Esp::esp_Time = Settings::Esp::esp_Watermark;
			// ImGui::Checkbox("Glow (BETA)", &Settings::Esp::glow);
			/// lmao glow is pp needs p fix
			
			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();

			ImGui::PushItemWidth(339.f);
			ImGui::Combo("Box", &Settings::Esp::esp_Style, items1, IM_ARRAYSIZE(items1));
			ImGui::PopItemWidth();
		}

		if (otherpages == 1)
		{
			ImGui::Text("Visuals");
			ImGui::Separator();
			ImGui::Spacing();
			const char* items2[] = { "Enemy" , "Team" , "Both" }; // Only Visible /// need XQZ on ro be invisible

			const char* items5[] = { "Disable" , "Flat" , "Textured" };
			ImGui::PushItemWidth(362.f);
			ImGui::Combo("Chams", &Settings::Esp::esp_Chams, items5, IM_ARRAYSIZE(items5));
			ImGui::PushItemWidth(362.f);
			ImGui::Combo("Chams Visible", &Settings::Esp::esp_ChamsVisible, items2, IM_ARRAYSIZE(items2));

			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();

			ImGui::PushItemWidth(362.f);
			ImGui::Combo("ESP Visible", &Settings::Esp::esp_Visible, items2, IM_ARRAYSIZE(items2));

			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();

			ImGui::PushItemWidth(362.f);
			ImGui::SliderInt("Custom Bomb Timer", &Settings::Esp::esp_BombTimer, 0, 65);
			ImGui::PushItemWidth(362.f);
			ImGui::SliderInt("Bullet Trace", &Settings::Esp::esp_BulletTrace, 0, 3000);

			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();

			const char* items3[] = { "None" , "Number Bottom" , "Bar Bottom" , "Bar Left" };
			ImGui::PushItemWidth(362.f);
			ImGui::Combo("Health", &Settings::Esp::esp_Health, items3, IM_ARRAYSIZE(items3));

			const char* items4[] = { "None" , "Number Bottom" , "Bar Bottom" , "Bar Right" };
			ImGui::PushItemWidth(362.f);
			ImGui::Combo("Armor", &Settings::Esp::esp_Armor, items4, IM_ARRAYSIZE(items4));

		}

		if (otherpages == 2)
		{
			ImGui::Text("Visuals");
			ImGui::Separator();
			ImGui::Spacing();

			const char* iHitSound[] = {
				"Off",
				"Default",
				"Gamesense",
				"Bameware",
				"Penguware" // you CAN add old ones back
			};

			const char* material_items[] = {
				"Glass",
				"Crystal",
				"Gold",
				"Dark Chrome",
				"Plastic Glass",
				"Velvet",
				"Crystal Blue",
				"Detailed Gold"
			};

			const char* armtype_items[] = {
				"Hands",
				"Hands & Weapon"
			};

			ImGui::Checkbox("XQZ Chams", &Settings::Esp::esp_XQZ); //invisible
			ImGui::PushItemWidth(362.f);
			ImGui::Checkbox("Material Cham", &Settings::Misc::misc_ChamsMaterials);
			ImGui::PushItemWidth(362.f);
			ImGui::Combo("##CHAMSMATERIALS", &Settings::Misc::misc_ChamsMaterialsList, material_items, ARRAYSIZE(material_items));

			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();

			ImGui::Checkbox("Hand Materials", &Settings::Misc::misc_ArmMaterials);
			ImGui::PushItemWidth(362.f);
			ImGui::Combo("##ARMMATERIALTYPE", &Settings::Misc::misc_ArmMaterialsType, armtype_items, ARRAYSIZE(armtype_items));
			ImGui::SameLine();
			ImGui::Text("Type");
			ImGui::PushItemWidth(362.f);
			ImGui::Combo("##ARMMATERIALS", &Settings::Misc::misc_ArmMaterialsList, material_items, ARRAYSIZE(material_items));
			ImGui::SameLine();
			ImGui::Text("Material");

			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();

			ImGui::Checkbox("Hitmarker", &Settings::Esp::esp_HitMarker);
			ImGui::PushItemWidth(362.f);
			ImGui::Combo("##HITSOUND", &Settings::Esp::esp_HitMarkerSound, iHitSound, ARRAYSIZE(iHitSound));
			ImGui::SameLine();
			ImGui::Text("Hitsound");
			ImGui::Checkbox("Hitlogs", &Settings::Esp::esp_hitevent);
		}
	}
	void DrawConfig()
	{
		ImGui::Separator();
		ImGui::Text("Configs");
		ImGui::Separator();
		static int iConfigSelect = 0;
		static int iMenuSheme = 1;
		static char ConfigName[11] = { 0 };

		ImGui::PushItemWidth(128.f);

		ImGui::ComboBoxArray("Select Config", &iConfigSelect, ConfigList);

		if (ImGui::Button("Load Config")) {
			Settings::LoadSettings(BaseDir + "\\" + ConfigList[iConfigSelect]);
			EventLog->AddToLog("Selected Config Loaded.");
		}
		ImGui::SameLine();

		if (ImGui::Button("Save Config")) {
			Settings::SaveSettings(BaseDir + "\\" + ConfigList[iConfigSelect]);
			EventLog->AddToLog("Selected Config Saved.");
		}
		ImGui::SameLine();
		if (ImGui::Button("Refresh Config List"))
			RefreshConfigs();


		ImGui::InputText("", ConfigName, 11);
		ImGui::SameLine();
		if (ImGui::Button("Make New Config"))
		{
			string ConfigFileName = ConfigName;

			if (ConfigFileName.size() < 1)
				ConfigFileName = "default";

			Settings::SaveSettings(BaseDir + "\\" + ConfigFileName + ".smef");
			EventLog->AddToLog("Created New Config.");
			RefreshConfigs();
		}

		//ImGui::Separator();

		//const char* ThemesList[] = { "Purple" , "Default" , "Light Pink" , "Dark Blue" , "MidNight" , "Night" , "Dunno" , "Blue"  , "Black" , "Green" , "Yellow" , "Light Blue" , "Light Grey" , "pHooK" };

		//ImGui::PushItemWidth(362.f);
		//ImGui::Combo("Menu Color", &iMenuSheme, ThemesList, IM_ARRAYSIZE(ThemesList));

		//ImGui::Separator();

		/*if (ImGui::Button("Apply Color"))
		{
			if (iMenuSheme == 0)
				g_pGui->purple();
			else if (iMenuSheme == 1)
				g_pGui->DefaultSheme1();
			else if (iMenuSheme == 2)
				g_pGui->RedSheme();
			else if (iMenuSheme == 3)
				g_pGui->darkblue();
			else if (iMenuSheme == 4)
				g_pGui->MidNightSheme();
			else if (iMenuSheme == 5)
				g_pGui->NightSheme();
			else if (iMenuSheme == 6)
				g_pGui->DunnoSheme();
			else if (iMenuSheme == 7)
				g_pGui->BlueSheme();
			else if (iMenuSheme == 8)
				g_pGui->BlackSheme2();
			else if (iMenuSheme == 9)
				g_pGui->green();
			else if (iMenuSheme == 10)
				g_pGui->pink();
			else if (iMenuSheme == 11)
				g_pGui->blue();
			else if (iMenuSheme == 12)
				g_pGui->yellow();
			else if (iMenuSheme == 13)
				g_pGui->BlackSheme();
		}*/
	}

	void DrawMisc() // Misc
	{
		const char* skybox_items[] = {
			"cs_baggage_skybox_",
			"cs_tibet",
			"embassy",
			"italy",
			"jungle",
			"nukeblank",
			"office",
			"sky_cs15_daylight01_hdr",
			"sky_cs15_daylight02_hdr",
			"sky_cs15_daylight03_hdr",
			"sky_cs15_daylight04_hdr",
			"sky_csgo_cloudy01",
			"sky_csgo_night02",
			"sky_csgo_night02b",
			"sky_csgo_night_flat",
			"sky_day02_05_hdr",
			"sky_day02_05",
			"sky_dust",
			"sky_l4d_rural02_ldr",
			"sky_venice",
			"vertigo_hdr",
			"vertigoblue_hdr",
			"vertigo",
			"vietnam"
		};
		static int otherpages = 0;
		const char* tabNames[] = { // change these to what you like
			"Misc",
			"Untrusted Features",
		};
		static int tabOrder[] = { 0 , 1 };
		const bool tabChanged = ImGui::TabLabels(tabNames, sizeof(tabNames) / sizeof(tabNames[0]), otherpages, tabOrder);

		if (otherpages == 0)
		{
			ImGui::Checkbox("Auto Accept", &Settings::Misc::misc_AutoAccept);
			ImGui::SameLine(SpaceLineOne);
			ImGui::Checkbox("Auto Bhop", &Settings::Misc::misc_Bhop);
			ImGui::SameLine(SpaceLineTwo);
			ImGui::Checkbox("NightMode", &Settings::Esp::esp_NightMode);

			ImGui::Checkbox("Show Spectators", &Settings::Misc::misc_Spectators);
			ImGui::SameLine(SpaceLineOne);
			ImGui::Checkbox("Recoil Crosshair", &Settings::Misc::misc_Punch);
			//ImGui::SameLine(SpaceLineTwo);
			//ImGui::Checkbox("Inventory Unlocker", &Settings::Misc::misc_inventory); // doesn't work

			ImGui::Checkbox("No Flash", &Settings::Misc::misc_NoFlash);
			ImGui::SameLine(SpaceLineOne);
			ImGui::Checkbox("No Smoke", &Settings::Misc::misc_NoSmoke);
			ImGui::SameLine(SpaceLineTwo);
			ImGui::Checkbox("No Hands", &Settings::Misc::misc_NoHands);
			ImGui::Checkbox("Wire Hands", &Settings::Misc::misc_WireHands);

			ImGui::Separator();

			ImGui::Checkbox("Chat Spam", &Settings::Misc::misc_spamregular);
			ImGui::SameLine(SpaceLineOne);
			ImGui::Checkbox("Random Chat Spam", &Settings::Misc::misc_spamrandom);
			ImGui::SameLine(SpaceLineTwo);
			ImGui::Checkbox("Static Name Spam", &Settings::Misc::misc_namespamidkmemes_static);
			ImGui::Separator();
			ImGui::Checkbox("FOV Changer", &Settings::Misc::misc_FovChanger);
			if (Settings::Misc::misc_FovChanger)
			{
				ImGui::PushItemWidth(362.f);
				ImGui::SliderInt("vFoV", &Settings::Misc::misc_FovView, 1, 170);
				ImGui::SliderInt("mFoV", &Settings::Misc::misc_FovModelView, 1, 190);
				ImGui::Separator();
			}
			
			//ImGui::Checkbox("Sniper Crosshair", &Settings::Misc::misc_AwpAim); // doesn't work
			//ImGui::SameLine(SpaceLineOne);
			ImGui::Checkbox("Disable Postprocess", &Settings::Misc::misc_Postprocess);
			ImGui::Separator();

			const char* items5[] = { "None" , "Reset" , "smef.cc" , "smef.cc Remove name" , "Valve" , "Valve Remove name" , "Aninmated" };
			ImGui::Combo("Clantag", &Settings::Misc::misc_Clan, items5, IM_ARRAYSIZE(items5));

			ImGui::Separator();
			ImGui::Text("Custom");
			ImGui::Separator();
			ImGui::Spacing();

			static char name[128] = { 0 };
			ImGui::PushItemWidth(362.f);
			ImGui::InputText("Custom Name", name, 16, Settings::Misc::misc_NameChanger);
			ImGui::PopItemWidth();
			if (ImGui::Button("Apply"))
			{
				ConVar* Name = Interfaces::GetConVar()->FindVar("name");
				*(int*)((DWORD)&Name->fnChangeCallback + 0xC) = 0;
				Name->SetValue(name);
			}
/*			ImGui::Separator();

			static char clantag[128] = { 0 };
			ImGui::PushItemWidth(362.f);
			ImGui::InputText("Clan Tag", clantag, 128, Settings::Misc::misc_ClanTagChanger);
			ImGui::PopItemWidth();
			if (ImGui::Button("Apply"))
			{
				Engine::ClanTagApply(clantag);
			}*/
		}

		if (otherpages == 1)
		{
			ImGui::Text("Experimental features");
			ImGui::Separator();
			ImGui::Spacing();
			ImGui::Checkbox("Enable Untrusted Features", &Settings::Untrusted);
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("sv_cheats 1 (will cause SMAC and possibly trigger vacnet)");
			if (Settings::Untrusted)
			{
				ImGui::Checkbox("No Sky", &Settings::Misc::misc_NoSky);
				ImGui::SameLine(SpaceLineOne);
				ImGui::Checkbox("Third Person", &Settings::Misc::misc_ThirdPerson);
				ImGui::SameLine(SpaceLineTwo);
				ImGui::PushItemWidth(362.f);
				if (Settings::Misc::misc_ThirdPerson)
					ImGui::SliderFloat("##THIRDPERSONRANGE", &Settings::Misc::misc_ThirdPersonRange, 0.f, 500.f, "Range: %0.f");

				ImGui::Separator();
				ImGui::Spacing();

				ImGui::PushItemWidth(362.f);
				if (ImGui::Combo("SkyBox", &Settings::Misc::misc_CurrentSky, skybox_items, IM_ARRAYSIZE(skybox_items)))
					Settings::Misc::misc_SkyName = skybox_items[Settings::Misc::misc_CurrentSky];
			}
		}
	}

	void DrawColors()
	{
		// uncomment if ur gay 
		//ImGui::ColorEdit3("Color CT", Settings::Esp::esp_Color_CT);
		//ImGui::ColorEdit3("Color T", Settings::Esp::esp_Color_TT);
		//ImGui::ColorEdit3("Color Visible CT", Settings::Esp::esp_Color_VCT);
		//ImGui::ColorEdit3("Color Visible T", Settings::Esp::esp_Color_VTT);
		//ImGui::ColorEdit3("Chams Color CT", Settings::Esp::chams_Color_CT);
		//ImGui::ColorEdit3("Chams Color T", Settings::Esp::chams_Color_TT);
		//ImGui::ColorEdit3("Chams Color Visible CT", Settings::Esp::chams_Color_VCT);
		//ImGui::ColorEdit3("Chams Color Visible T", Settings::Esp::chams_Color_VTT);
		//ImGui::ColorEdit3("Color Hitmarker", Settings::Esp::esp_HitMarkerColor);
		//ImGui::ColorEdit3("Color Dynamic Lights", Settings::Esp::esp_Dlight);

		const char* espcolornames[] = { "ESP Invisible CT", "ESP Invisible T", "ESP Visible CT", "ESP Visible T", "Chams Invisible CT",
			"Chams Invisible T", "Chams Visible CT", "Chams Visible T", "Hitmarkers", "Dynamic Lights"};

		static int selected = 0;
		static bool done = false;
		ImGui::Columns(2, nullptr, true);
		ImGui::PushItemWidth(-1);
		ImGui::ListBox("", &selected, espcolornames, IM_ARRAYSIZE(espcolornames), 20);
		ImGui::NextColumn();
		if (!done) {
			ImGui::SetColumnOffset(1, 150);
			done = true;
		}
		switch (selected) {
		case 0:
			ImGui::ColorPicker3(espcolornames[0], Settings::Esp::esp_Color_CT);
			break;
		case 1:
			ImGui::ColorPicker3(espcolornames[1], Settings::Esp::esp_Color_TT);
			break;
		case 2:
			ImGui::ColorPicker3(espcolornames[2], Settings::Esp::esp_Color_VCT);
			break;
		case 3:
			ImGui::ColorPicker3(espcolornames[3], Settings::Esp::esp_Color_VTT);
			break;
		case 4:
			ImGui::ColorPicker3(espcolornames[4], Settings::Esp::chams_Color_CT);
			break;
		case 5:
			ImGui::ColorPicker3(espcolornames[5], Settings::Esp::chams_Color_TT);
			break;
		case 6:
			ImGui::ColorPicker3(espcolornames[6], Settings::Esp::chams_Color_VCT);
			break;
		case 7:
			ImGui::ColorPicker3(espcolornames[7], Settings::Esp::chams_Color_VTT);
			break;
		case 8:
			ImGui::ColorPicker3(espcolornames[8], Settings::Esp::esp_HitMarkerColor);
			break;
		case 9:
			ImGui::ColorPicker3(espcolornames[9], Settings::Esp::esp_Dlight);
			break;
		case 10:
			ImGui::ColorPicker3(espcolornames[10], Settings::Esp::esp_Ambient);
			break;
		}
		ImGui::Columns(1, nullptr, false);
	}

	void OnRenderGUI()
	{
		auto& style = ImGui::GetStyle();
		ImGuiIO& io = ImGui::GetIO();
		io.IniFilename = GuiFile.c_str();

		int w;
		int h;

		//fading stuff
		if (bIsGuiVisible)
			if (style.Alpha > 1.f)
				style.Alpha = 1.f;
			else if (style.Alpha != 1.f)
				style.Alpha += 0.01f;
			else if (!bIsGuiVisible)
				if (style.Alpha < 0.f)
					style.Alpha = 0.f;
				else if (style.Alpha != 0.f)
					style.Alpha -= 0.01f;

		Interfaces::Engine()->GetScreenSize(w, h);
		ImVec2 mainWindowPos;
		ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiSetCond_Appearing);
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.1f, 0.1, 0.1f, 1.f);

		style.WindowPadding = ImVec2(8, 8);
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.1f, 0.1f, 0.1f, 0.95f);
		ImGui::SetNextWindowPos(ImVec2(500, 500), ImGuiSetCond_Appearing);
		BtnNormal();
		ImGui::Begin("!smef.cc", &bIsGuiVisible, ImVec2(828, 450), 0.98f, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar ); //ImGuiWindowFlags_ShowBorders);
		mainWindowPos = ImGui::GetWindowPos();

		if (Global::MenuTab == 0)
			DrawAimbot();
		if (Global::MenuTab == 1)
			DrawVisuals();
		if (Global::MenuTab == 2)
			DrawSkins();
		if (Global::MenuTab == 3)
			DrawMisc();
		if (Global::MenuTab == 4)
			DrawConfig();
		if (Global::MenuTab == 5)
			DrawColors();
		ImGui::End();
		ImGui::SetNextWindowPos(ImVec2(mainWindowPos.x - 6, mainWindowPos.y - 6));
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.f);
		ImGui::Begin("##border2", &bIsGuiVisible, ImVec2(840, 462), 0.98f, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus); // | ImGuiWindowFlags_ShowBorders);
		ImGui::End();

		//left bar with buttons
		ImGui::SetNextWindowPos(ImVec2(0, -30));
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.1f, 0.1f, 0.1f, 1.f);
		ImGui::Begin("##exdee", &bIsGuiVisible, ImVec2(180, 1110), 0.5f, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoInputs);
		ImGui::End();

		ImGui::SetNextWindowPos(ImVec2(10, 150));
		style.Colors[ImGuiCol_WindowBg] = ImVec4(1.f, 1.f, 1.f, 0.1f);
		style.WindowPadding = ImVec2(0, 0);//not
		style.ItemSpacing = ImVec2(8, 4);//not
		ImGui::Begin("##tabarea", &bIsGuiVisible, ImVec2(150, 620), 0.f, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoTitleBar);
		ImGui::PushFont(tabfont);

		//TextColor(true);
		BtnColor(false); //if you enable this (true) , buttons get color
		if (ImGui::Button("A", ImVec2(133.3, 100))) Global::MenuTab = 0;
		if (ImGui::Button("D", ImVec2(133.3, 100))) Global::MenuTab = 1;
		if (ImGui::Button("B", ImVec2(133.3, 100))) Global::MenuTab = 2;
		if (ImGui::Button("G", ImVec2(133.3, 100))) Global::MenuTab = 3;
		if (ImGui::Button("C", ImVec2(133.3, 100))) Global::MenuTab = 4;
		if (ImGui::Button("H", ImVec2(133.3, 100))) Global::MenuTab = 5;
		ImGui::PopFont();
		ImGui::End();
	}
}

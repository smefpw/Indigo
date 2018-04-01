#include "Client.h"
#include <ctime>
#include "../Gui/memoryfonts.h"

//[enc_string_enable /]
//[junk_enable /]
ImFont* tabfont;
ImFont* font;

static int iSlot;
const char* Slot[] =
{
	"1",
	"2",
	"3",
	"4"
};

void CL_FullUpdate()
{
	Interfaces::Engine()->ClientCmd_Unrestricted2("record x; stop");
}

bool Aimbot;
bool Triggerbot;
bool Visuals;
bool Misc;
bool Radar;
bool Colors;

float SpaceLineOne = 140.f;
float SpaceLineTwo = 280.f;
float SpaceLineThr = 420.f;

namespace Client
{
	//[swap_lines]
	int	iScreenWidth = 0;
	int	iScreenHeight = 0;

	string BaseDir = "";
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
	CSkin*		g_pSkin = nullptr;
	CMisc*		g_pMisc = nullptr;
	CInventoryChanger* g_pInventoryChanger = nullptr;

	bool		bC4Timer = false;
	int			iC4Timer = 40;

	int			iWeaponID = 0;
	int			iWeaponSelectIndex = WEAPON_DEAGLE;
	int			iWeaponSelectSkinIndex = -1;
	//[/swap_lines]

	void ReadConfigs(LPCTSTR lpszFileName)
	{
		if (!strstr(lpszFileName, "gui.ini"))
		{
			ConfigList.push_back(lpszFileName);
		}
	}

	void RefreshConfigs()
	{
		ConfigList.clear();
		string ConfigDir = BaseDir + "\\*.ini";
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
		{
			style.Colors[ImGuiCol_Text] = ImVec4(0.98f, 0.98f, 0.98f, 1.f);
		}
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
		style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.21f, 0.21f, 0.21f, 1.f);
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
			style.Colors[ImGuiCol_Button] = ImVec4(0.12, 0.12, 0.12, .98f);
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.12, 0.12, 0.12, .98f);
		style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.12, 0.12, 0.12, .98f);
	}

	bool Initialize(IDirect3DDevice9* pDevice)
	{
		g_pPlayers = new CPlayers();
		g_pRender = new CRender(pDevice);
		g_pGui = new CGui();

		g_pAimbot = new CAimbot();
		g_pTriggerbot = new CTriggerbot();
		g_pEsp = new CEsp();
		g_pRadar = new CRadar();
	
		g_pSkin = new CSkin();
		g_pMisc = new CMisc();
		g_pInventoryChanger = new CInventoryChanger();

		GuiFile = BaseDir + "\\" + "gui.ini";
		IniFile = BaseDir + "\\" + "settings.ini";

		g_pSkin->InitalizeSkins();

		Settings::LoadSettings(IniFile);

		iWeaponSelectSkinIndex = GetWeaponSkinIndexFromPaintKit(g_SkinChangerCfg[iWeaponSelectIndex].nFallbackPaintKit);

		g_pGui->GUI_Init(pDevice);

		RefreshConfigs();

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




			if (Settings::Esp::esp_Watermark)
			{
				//bool rainbow; 
				static float rainbow;
				rainbow += 0.005f;
				if (rainbow > 1.f) rainbow = 0.f;
				g_pRender->Text(15, 15, false, true, Color::FromHSB(rainbow, 1.f, 1.f), WATER_MARK);
			}

			g_pGui->MenuColor();

			{
				if (g_pEsp)
					g_pEsp->OnRender();

				if (g_pMisc)
				{
					g_pMisc->OnRender();
					g_pMisc->OnRenderSpectatorList();
				}
			}

			std::time_t result = std::time(nullptr);

			if (Settings::Esp::esp_Time)
				g_pRender->Text(15, 30, false, true, Color::White(), std::asctime(std::localtime(&result)));

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

	void OnCreateMove(CUserCmd* pCmd)
	{
		if (g_pPlayers && Interfaces::Engine()->IsInGame())
		{
			g_pPlayers->Update();

			if (g_pEsp)
				g_pEsp->OnCreateMove(pCmd);

			if (IsLocalAlive())
			{
				if (!bIsGuiVisible)
				{
					int iWeaponSettingsSelectID = GetWeaponSettingsSelectID();

					if (iWeaponSettingsSelectID >= 0)
						iWeaponID = iWeaponSettingsSelectID;
				}

				if (g_pAimbot)
					g_pAimbot->OnCreateMove(pCmd, g_pPlayers->GetLocal());

				if (g_pMisc)
					g_pMisc->OnCreateMove(pCmd);

				backtracking->legitBackTrack(pCmd);

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
			
			ConVar* sv_cheats = Interfaces::GetConVar()->FindVar("sv_cheats");
			SpoofedConvar* sv_cheats_spoofed = new SpoofedConvar(sv_cheats);
			sv_cheats_spoofed->SetInt(1);
			
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

		if (ImGui::Button("Aimbot Options", ImVec2(255.0f, 35.0f))) // <---- customize these to your liking.
		{
			otherpages = 0;
		}
		ImGui::SameLine();
		if (ImGui::Button("Weapon Options", ImVec2(255.0f, 35.0f))) // <---- again, customize to your liking.
		{
			otherpages = 1;
		}

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

			const char* BestHit[] = { "Prefer Manually" , "All Spots" , "No Headshot" , "No Arms/Legs" , "No Arms/Legs/Neck" };
			ImGui::PushItemWidth(362.f);
			ImGui::Combo("Hit Scan", &Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_BestHit, BestHit, IM_ARRAYSIZE(BestHit));
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("if disabled then used Aimspot");

			ImGui::PopItemWidth();

			const char* Aimspot[] = { "Head" , "Neck" , "Lower Neck" , "Body" , "Thorax" , "Chest" , "Right Thigh" ,
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

				const char* ClampType[] = { "All Target" , "Shot" , "Shot + Target" };
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

			ImGui::Text("Backtrack Options");
			ImGui::Separator();
			ImGui::Spacing();
			ImGui::Checkbox("Backtrack", &Settings::Aimbot::aim_Backtrack);
			ImGui::PushItemWidth(362.f);
			ImGui::SliderInt("Ticks", &Settings::Aimbot::aim_Backtracktickrate, 1, 12);

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
	}

	void DrawSkins() // Skins
	{
		static int otherpages = 0;

		if (ImGui::Button("SKINCHANGER", ImVec2(233.3f, 35.0f))) // <---- customize these to your liking.
		{
			otherpages = 0;
		}
		ImGui::SameLine();
		if (ImGui::Button("INVENTORY CHANGER", ImVec2(233.3f, 35.0f))) // <---- again, customize to your liking.
		{
			otherpages = 1;
		}
		ImGui::SameLine();
		if (ImGui::Button("PROFILE CHANGER", ImVec2(233.3f, 35.0f))) // <---- again, customize to your liking.
		{
			otherpages = 2;
		}

		if (otherpages == 0)
		{

		const char* quality_items[] =
		{
			"None",
			"1 - Consumer grade (white)",
			"2 - Industrial grade (light blue)",
			"3 - Mil-Spec (darker blue)",
			"4 - Restricted (purple)",
			"5 - Classified (pinkish purple)",
			"6 - Covert (red)",
			"7 - Exceedingly Rare (gold)",
		};

		const char* gloves_listbox_items[49] =
		{
			"None",
			"Sport Gloves | Superconductor",
			"Sport Gloves | Pandora's Box",
			"Sport Gloves | Hedge Maze",
			"Sport Gloves | Arid",
			"Sport Gloves | Vice",
			"Sport Gloves | Omega",
			"Sport Gloves | Bronze Morph",
			"Sport Gloves | Amphibious",
			"Moto Gloves | Eclipse",
			"Moto Gloves | Spearmint",
			"Moto Gloves | Boom!",
			"Moto Gloves | Cool Mint",
			"Moto Gloves | Polygon",
			"Moto Gloves | Transport",
			"Moto Gloves | Turtle",
			"Moto Gloves | Pow",
			"Specialist Gloves | Crimson Kimono",
			"Specialist Gloves | Emerald Web",
			"Specialist Gloves | Foundation",
			"Specialist Gloves | Forest DDPAT",
			"Specialist Gloves | Mogul",
			"Specialist Gloves | Fade",
			"Specialist Gloves | Buckshot",
			"Specialist Gloves | Crimson Web",
			"Driver Gloves | Lunar Weave",
			"Driver Gloves | Convoy",
			"Driver Gloves | Crimson Weave",
			"Driver Gloves | Diamondback",
			"Driver Gloves | Racing Green",
			"Driver Gloves | Overtake",
			"Driver Gloves | Imperial Plad",
			"Driver Gloves | King Snake",
			"Hand Wraps | Leather",
			"Hand Wraps | Spruce DDPAT",
			"Hand Wraps | Badlands",
			"Hand Wraps | Slaughter",
			"Hand Wraps | Aboreal",
			"Hand Wraps | Duct Tape",
			"Hand Wraps | Overprint",
			"Hand Wraps | Cobalt Skulls",
			"Bloodhound Gloves | Charred",
			"Bloodhound Gloves | Snakebite",
			"Bloodhound Gloves | Bronzed",
			"Bloodhound Gloves | Guerrilla",
			"Hydra Gloves | Case Hardened",
			"Hydra Gloves | Rattler",
			"Hydra Gloves | Mangrove",
			"Hydra Gloves | Emerald",
		};
		//[enc_string_enable /]

		ImGui::Separator();

		ImGui::Text("Skin Changer");

		ImGui::Separator();

		ImGui::Text("Current Weapon: %s", pWeaponData[iWeaponID]);

		ImGui::PushItemWidth(362.f);

		static int iOldWeaponID = -1;

		ImGui::Combo("Weapon##WeaponSelect", &iWeaponID, pWeaponData, IM_ARRAYSIZE(pWeaponData));

		iWeaponSelectIndex = pWeaponItemIndexData[iWeaponID];

		if (iOldWeaponID != iWeaponID)
			iWeaponSelectSkinIndex = GetWeaponSkinIndexFromPaintKit(g_SkinChangerCfg[iWeaponSelectIndex].nFallbackPaintKit);

		iOldWeaponID = iWeaponID;

		string WeaponSkin = pWeaponData[iWeaponID];
		WeaponSkin += " Skin";

		ImGui::ComboBoxArray(WeaponSkin.c_str(), &iWeaponSelectSkinIndex, WeaponSkins[iWeaponID].SkinNames);

		ImGui::Combo("Weapon Quality", &g_SkinChangerCfg[pWeaponItemIndexData[iWeaponID]].iEntityQuality, quality_items, IM_ARRAYSIZE(quality_items));
		ImGui::SliderFloat("Weapon Wear", &g_SkinChangerCfg[pWeaponItemIndexData[iWeaponID]].flFallbackWear, 0.001f, 1.f);
		ImGui::InputInt("Weapon StatTrak", &g_SkinChangerCfg[pWeaponItemIndexData[iWeaponID]].nFallbackStatTrak, 1, 100, ImGuiInputTextFlags_CharsDecimal);

		ImGui::Separator();

		ImGui::Combo("Gloves Skin", &Settings::Skin::gloves_skin, gloves_listbox_items,
			IM_ARRAYSIZE(gloves_listbox_items));

		ImGui::Separator();

		ImGui::PopItemWidth();

		const char* knife_models_items[] =
		{
			"Default","Bayonet","Flip","Gut","Karambit" ,"M9 Bayonet",
			"Huntsman","Falchion","Bowie","Butterfly","Shadow Daggers"
		};

		ImGui::Text("Knife Changer");
		ImGui::Separator();

		ImGui::PushItemWidth(362.f);
		ImGui::Combo("Knife CT Model", &Settings::Skin::knf_ct_model, knife_models_items, IM_ARRAYSIZE(knife_models_items));
		ImGui::PushItemWidth(362.f);
		ImGui::Combo("Knife T Model", &Settings::Skin::knf_tt_model, knife_models_items, IM_ARRAYSIZE(knife_models_items));

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
		ImGui::SliderFloat("Knife CT Wear", &g_SkinChangerCfg[WEAPON_KNIFE].flFallbackWear, 0.001f, 1.f);
		ImGui::PushItemWidth(362.f);
		ImGui::Combo("Knife CT Quality", &g_SkinChangerCfg[WEAPON_KNIFE].iEntityQuality, quality_items, IM_ARRAYSIZE(quality_items));
		ImGui::ComboBoxArray(KnifeCTModel.c_str(), &iSelectKnifeCTSkinIndex, KnifeSkins[iKnifeCTModelIndex].SkinNames);

		ImGui::Separator();

		ImGui::PushItemWidth(362.f);
		ImGui::SliderFloat("Knife T Wear", &g_SkinChangerCfg[WEAPON_KNIFE_T].flFallbackWear, 0.001f, 1.f);
		ImGui::PushItemWidth(362.f);
		ImGui::Combo("Knife T Quality", &g_SkinChangerCfg[WEAPON_KNIFE_T].iEntityQuality, quality_items, IM_ARRAYSIZE(quality_items));
		ImGui::ComboBoxArray(KnifeTTModel.c_str(), &iSelectKnifeTTSkinIndex, KnifeSkins[iKnifeTTModelIndex].SkinNames);

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
			ImGui::Columns(2, nullptr, false);
			ImGui::Checkbox("Enable Inventory Changer", &Settings::InventoryChanger::enabled);
			static int itemDefinitionIndex = 0;
			static int paintKit = 0;
			static int rarity = 0;
			static int seed = 0;
			static float wear = 0.f;

			static const char* weapons[] = {
				"None",
				"1 - Desert Eagle",
				"2 - Dual Berettas",
				"3 - Five SeveN",
				"4 - Glock",
				"",
				"",
				"7 - AK47",
				"8 - AUG",
				"9 - AWP",
				"10 - FAMAS",
				"11 - G3SG1",
				"",
				"13 - Galil AR",
				"14 - M249",
				"",
				"16 - M4A4",
				"17 - MAC10",
				"",
				"19 - P90",
				"",
				"",
				"",
				"",
				"24 - UMP45",
				"25 - XM1014",
				"26 - PPBizon",
				"27 - MAG7",
				"28 - Negev",
				"29 - SawedOff",
				"30 - Tec9",
				" ",
				"32 - P2000",
				"33 - MP7",
				"34 - MP9",
				"35 - Nova",
				"36 - P250",
				"38 - SCAR20",
				"39 - SG553",
				"40 - SCOUT",
			};
			ImGui::Combo("Weapon", &itemDefinitionIndex, weapons, IM_ARRAYSIZE(weapons));
//			ImGui::InputInt("Weapon ID", &itemDefinitionIndex);

		    ImGui::InputInt("Paint Kit", &paintKit);
			static const char* rare[] = 
			{
				"None",
				"1 - Consumer grade (white)",
				"2 - Industrial grade (light blue)",
				"3 - Mil-Spec (darker blue)",
			    "4 - Restricted (purple)",
				"5 - Classified (pinkish purple)",
				"6 - Covert (red)",
				"7 - Exceedingly Rare (gold)",
			};
			ImGui::Combo("Rarity", &rarity, rare, IM_ARRAYSIZE(rare));
			//ImGui::InputInt("Rarity", &rarity);
			ImGui::InputInt("Seed", &seed);
			ImGui::SliderFloat("Wear", &wear, FLT_MIN, 1.f, "%.10f", 5);
			if (ImGui::Button("Add")) {
				Settings::InventoryChanger::weapons.insert(Settings::InventoryChanger::weapons.end(), { itemDefinitionIndex , paintKit , rarity , seed, wear });
			} ImGui::SameLine();
			if (ImGui::Button("Apply##Skin")) {
				SendClientHello();
			}
			ImGui::NextColumn();

			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 1, 1));
			ImGui::ListBoxHeader("Skins");
			int weapon_index = 0;
			for (auto weapon : Settings::InventoryChanger::weapons) {
				if (ImGui::Selectable(std::string(std::to_string(weapon.itemDefinitionIndex) + " " + std::to_string(weapon.paintKit)).c_str())) {
					Settings::InventoryChanger::weapons.erase(Settings::InventoryChanger::weapons.begin() + weapon_index);
				}
				weapon_index++;
			}
			ImGui::ListBoxFooter();
			ImGui::PopStyleColor();
			ImGui::Columns(1, nullptr, false);
		}

		if (otherpages == 2)
		{
			ImGui::Columns(2, nullptr, false);
			ImGui::Checkbox("Enable Medal Changer", &Settings::MedalChanger::enabled);
			static int medal_id = 0;
			ImGui::InputInt("Medal ID", &medal_id);
			if (ImGui::Button("Add") && medal_id != 0) {
				Settings::MedalChanger::medals.insert(Settings::MedalChanger::medals.end(), medal_id);
				medal_id = 0;
			}
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 1, 1));
			ImGui::ListBoxHeader("Medal List");
			for (int m = 0; m < Settings::MedalChanger::medals.size(); m++) {
				if (ImGui::Selectable(std::to_string(Settings::MedalChanger::medals[m]).c_str())) {
					if (Settings::MedalChanger::equipped_medal == Settings::MedalChanger::medals[m]) {
						Settings::MedalChanger::equipped_medal = 0;
						Settings::MedalChanger::equipped_medal_override = false;
					}
					Settings::MedalChanger::medals.erase(Settings::MedalChanger::medals.begin() + m);
				}
			}
			ImGui::ListBoxFooter();
			ImGui::PopStyleColor();
			ImGui::Checkbox("Equipped Medal Override", &Settings::MedalChanger::equipped_medal_override);
			if (Settings::MedalChanger::equipped_medal_override) {
				static int equipped_medal = 0;
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 1, 1));
				if (ImGui::Combo("Equipped Medal", &equipped_medal, [](void* data, int idx, const char** out_text)
				{
					*out_text = std::to_string(Settings::MedalChanger::medals[idx]).c_str();
					return true;
				}, nullptr, Settings::MedalChanger::medals.size(), 5)) {
					Settings::MedalChanger::equipped_medal = Settings::MedalChanger::medals[equipped_medal];
				}
				ImGui::PopStyleColor();
			}
			if (ImGui::Button("Apply##Medals")) {
				SendClientHello();
			}
			ImGui::NextColumn();
			ImGui::Checkbox("Enable Profile Changer", &Settings::ProfileChanger::enabled);
			static const char* ranks[] = {
				"Unranked",
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
				"The Global Elite"
			};
			ImGui::Combo("Rank", &Settings::ProfileChanger::rank_id, ranks, IM_ARRAYSIZE(ranks));
			ImGui::SliderInt("Level", &Settings::ProfileChanger::level, 0, 40);
			ImGui::SliderInt("XP", &Settings::ProfileChanger::xp, 0, 5000);
			ImGui::InputInt("Wins", &Settings::ProfileChanger::wins);
			ImGui::InputInt("Friendly", &Settings::ProfileChanger::cmd_friendly);
			ImGui::InputInt("Teaching", &Settings::ProfileChanger::cmd_teaching);
			ImGui::InputInt("Leader", &Settings::ProfileChanger::cmd_leader);
			if (ImGui::Button("Apply##Profile"))
			{
				SendMMHello();
			}
			ImGui::Columns(1, nullptr, false);
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

		if (ImGui::Button("Part 1", ImVec2(255.0f, 35.0f))) // <---- customize these to your liking.
		{
			otherpages = 0;
		}
		ImGui::SameLine();
		if (ImGui::Button("Part 2", ImVec2(255.0f, 35.0f))) // <---- again, customize to your liking.
		{
			otherpages = 1;
		}
		ImGui::SameLine();
		if (ImGui::Button("Part 3", ImVec2(255.0f, 35.0f))) // <---- customize these to your liking.
		{
			otherpages = 2;
		}

		if (otherpages == 0)
		{
			string style_1 = "None";
			string style_2 = "Box";
			string style_3 = "Corners";

			const char* items1[] = { style_1.c_str() , style_2.c_str(), style_3.c_str() };

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
			ImGui::Checkbox("Time", &Settings::Esp::esp_Time);

			ImGui::Checkbox("Distance", &Settings::Esp::esp_Distance);
			ImGui::SameLine(SpaceLineOne);
			ImGui::Checkbox("Skeleton", &Settings::Esp::esp_Skeleton);
			ImGui::SameLine(SpaceLineTwo);
			ImGui::Checkbox("World Weapon", &Settings::Esp::esp_WorldWeapons);
			ImGui::SameLine(SpaceLineThr);
			ImGui::Checkbox("Chams XQZ", &Settings::Esp::esp_XQZ);

			ImGui::Checkbox("World Grenade", &Settings::Esp::esp_WorldGrenade);
			ImGui::SameLine(SpaceLineOne);
			ImGui::Checkbox("Grenade Boxes", &Settings::Esp::esp_BoxNade);
			ImGui::SameLine(SpaceLineTwo);
			ImGui::Checkbox("Dynamic Lights", &Settings::Esp::esp_Dlightz);

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
			string visible_1 = "Enemy";
			string visible_2 = "Team";
			string visible_3 = "All";
			string visible_4 = "Only Visible";

			const char* items2[] = { visible_1.c_str() , visible_2.c_str() , visible_3.c_str() , visible_4.c_str() };

			string chams_1 = "None";
			string chams_2 = "Flat";
			string chams_3 = "Texture";

			const char* items5[] = { chams_1.c_str() , chams_2.c_str() , chams_3.c_str() };
			ImGui::PushItemWidth(362.f);
			ImGui::Combo("Chams", &Settings::Esp::esp_Chams, items5, IM_ARRAYSIZE(items5));
			ImGui::PushItemWidth(362.f);
			ImGui::Combo("Chams Visible", &Settings::Esp::esp_ChamsVisible, items2, IM_ARRAYSIZE(items2));

			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();

			ImGui::PushItemWidth(362.f);
			ImGui::Combo("Visible", &Settings::Esp::esp_Visible, items2, IM_ARRAYSIZE(items2));

			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();

			ImGui::PushItemWidth(362.f);
			ImGui::SliderInt("Size", &Settings::Esp::esp_Size, 0, 10);
			ImGui::PushItemWidth(362.f);
			ImGui::SliderInt("Bomb Timer", &Settings::Esp::esp_BombTimer, 0, 65);
			ImGui::PushItemWidth(362.f);
			ImGui::SliderInt("Bullet Trace", &Settings::Esp::esp_BulletTrace, 0, 3000);

			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();

			string hpbar_1 = "None";
			string hpbar_2 = "Number";
			string hpbar_3 = "Bottom";
			string hpbar_4 = "Left";

			const char* items3[] = { hpbar_1.c_str() , hpbar_2.c_str() , hpbar_3.c_str() , hpbar_4.c_str() };
			ImGui::PushItemWidth(362.f);
			ImGui::Combo("Health", &Settings::Esp::esp_Health, items3, IM_ARRAYSIZE(items3));

			string arbar_1 = "None";
			string arbar_2 = "Number";
			string arbar_3 = "Bottom";
			string arbar_4 = "Right";

			const char* items4[] = { arbar_1.c_str() , arbar_2.c_str() , arbar_3.c_str() , arbar_4.c_str() };
			ImGui::PushItemWidth(362.f);
			ImGui::Combo("Armor", &Settings::Esp::esp_Armor, items4, IM_ARRAYSIZE(items4));

		}

		if (otherpages == 2)
		{
			ImGui::Text("Visuals");
			ImGui::Separator();
			ImGui::Spacing();

			const char* iHitSound[] =
			{
				"Off",
				"Default",
				"Anime",
				"Roblox",
				"Gamesense",
			};

			const char* material_items[] =
			{
				"Glass",
				"Crystal",
				"Gold",
				"Dark Chrome",
				"Plastic Glass",
				"Velvet",
				"Crystal Blue",
				"Detailed Gold"
			};

			const char* armtype_items[] =
			{
				"Arms Only",
				"Arms + Weapon"
			};

			ImGui::Checkbox("Chams Materials", &Settings::Misc::misc_ChamsMaterials);
			ImGui::PushItemWidth(362.f);
			ImGui::Combo("##CHAMSMATERIALS", &Settings::Misc::misc_ChamsMaterialsList, material_items, ARRAYSIZE(material_items));

			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();

			ImGui::Checkbox("Arms Materials", &Settings::Misc::misc_ArmMaterials);
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
			ImGui::Text("Hitmarker Sound");

			ImGui::Spacing();
			ImGui::Text("Radar");
			ImGui::Separator();
			ImGui::Spacing();

			ImGui::Checkbox("Active", &Settings::Radar::rad_Active);
			ImGui::SameLine();
			ImGui::Checkbox("Team", &Settings::Radar::rad_Team);
			ImGui::SameLine();
			ImGui::Checkbox("Enemy", &Settings::Radar::rad_Enemy);
			ImGui::SameLine();
			ImGui::Checkbox("Sound", &Settings::Radar::rad_Sound);
			ImGui::SameLine();
			ImGui::Checkbox("InGame", &Settings::Radar::rad_InGame);

			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();

			ImGui::PushItemWidth(362.f);
			ImGui::SliderInt("Range", &Settings::Radar::rad_Range, 1, 5000);
			ImGui::PushItemWidth(362.f);
			ImGui::SliderInt("Alpha", &Settings::Radar::rad_Alpha, 1, 255);
		}
	}
	void DrawRadar() // Radar
	{
		ImGui::Separator();
		ImGui::Text("Configs");
		ImGui::Separator();
		static int iConfigSelect = 0;
		static int iMenuSheme = 1;
		static char ConfigName[64] = { 0 };

		ImGui::ComboBoxArray("Select Config", &iConfigSelect, ConfigList);

		if (ImGui::Button("Load Config"))
		{
			Settings::LoadSettings(BaseDir + "\\" + ConfigList[iConfigSelect]);
		}
		ImGui::SameLine();
		if (ImGui::Button("Save Config"))
		{
			Settings::SaveSettings(BaseDir + "\\" + ConfigList[iConfigSelect]);
		}
		ImGui::SameLine();
		if (ImGui::Button("Refresh Config List"))
		{
			RefreshConfigs();
		}

		ImGui::Separator();

		ImGui::PushItemWidth(362.f);
		ImGui::InputText("Config Name", ConfigName, 64);

		if (ImGui::Button("Create & Save new Config"))
		{
			string ConfigFileName = ConfigName;

			if (ConfigFileName.size() < 1)
			{
				ConfigFileName = "settings";
			}

			Settings::SaveSettings(BaseDir + "\\" + ConfigFileName + ".ini");
			RefreshConfigs();
		}

		ImGui::Separator();

		const char* ThemesList[] = { "Purple" , "Default" , "Light Pink" , "Dark Blue" , "MidNight" , "Night" , "Dunno" , "Blue"  , "Black" , "Green" , "Yellow" , "Light Blue" , "Light Grey" , "pHooK" };

		ImGui::PushItemWidth(362.f);
		ImGui::Combo("Menu Color", &iMenuSheme, ThemesList, IM_ARRAYSIZE(ThemesList));

		ImGui::Separator();

		if (ImGui::Button("Apply Color"))
		{
			if (iMenuSheme == 0)
			{
				g_pGui->purple();
			}
			else if (iMenuSheme == 1)
			{
				g_pGui->DefaultSheme1();
			}
			else if (iMenuSheme == 2)
			{
				g_pGui->RedSheme();
			}
			else if (iMenuSheme == 3)
			{
				g_pGui->darkblue();
			}
			else if (iMenuSheme == 4)
			{
				g_pGui->MidNightSheme();
			}
			else if (iMenuSheme == 5)
			{
				g_pGui->NightSheme();
			}
			else if (iMenuSheme == 6)
			{
				g_pGui->DunnoSheme();
			}
			else if (iMenuSheme == 7)
			{
				g_pGui->BlueSheme();
			}
			else if (iMenuSheme == 8)
			{
				g_pGui->BlackSheme2();
			}
			else if (iMenuSheme == 9)
			{
				g_pGui->green();
			}
			else if (iMenuSheme == 10)
			{
				g_pGui->pink();
			}
			else if (iMenuSheme == 11)
			{
				g_pGui->blue();
			}
			else if (iMenuSheme == 12)
			{
				g_pGui->yellow();
			}
			else if (iMenuSheme == 13)
			{
				g_pGui->BlackSheme();
			}
		}
	}

	void DrawMisc() // Misc
	{
		const char* skybox_items[] =
		{
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

		//		ImGui::Checkbox("Skin Changer", &Settings::Misc::misc_SkinChanger);
		//		ImGui::Checkbox("Knife Changer", &Settings::Misc::misc_KnifeChanger);
		ImGui::Checkbox("Auto Accept", &Settings::Misc::misc_AutoAccept);
		ImGui::SameLine(SpaceLineOne);
		ImGui::Checkbox("Auto Bhop", &Settings::Misc::misc_Bhop);
		ImGui::SameLine(SpaceLineTwo);
		ImGui::Checkbox("Auto Strafe", &Settings::Misc::misc_AutoStrafe);

		ImGui::Checkbox("Show Spectators", &Settings::Misc::misc_Spectators);
		ImGui::SameLine(SpaceLineOne);
		ImGui::Checkbox("Recoil Crosshair", &Settings::Misc::misc_Punch);
		ImGui::SameLine(SpaceLineTwo);
		ImGui::Checkbox("Sniper Crosshair", &Settings::Misc::misc_AwpAim);
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("sv_cheats 1/smac ban");

		ImGui::Checkbox("No Flash", &Settings::Misc::misc_NoFlash);
		ImGui::SameLine(SpaceLineOne);
		ImGui::Checkbox("No Smoke", &Settings::Misc::misc_NoSmoke);
		ImGui::SameLine(SpaceLineTwo);
		ImGui::Checkbox("No Hands", &Settings::Misc::misc_NoHands);

		ImGui::Checkbox("NightMode", &Settings::Esp::esp_NightMode);
		ImGui::SameLine(SpaceLineOne);
		ImGui::Checkbox("Slide Walk", &Settings::Misc::misc_Moonwalk);
		ImGui::SameLine(SpaceLineTwo);
		ImGui::Checkbox("Wire Hands", &Settings::Misc::misc_WireHands);

		ImGui::Separator();

		ImGui::Checkbox("Chat Spam", &Settings::Misc::misc_spamregular);
		ImGui::SameLine(SpaceLineOne);
		ImGui::Checkbox("Random Chat Spam", &Settings::Misc::misc_spamrandom);
		ImGui::SameLine(SpaceLineTwo);
		ImGui::Checkbox("Static Name Spam", &Settings::Misc::misc_namespamidkmemes_static);
		ImGui::Separator();
		ImGui::Checkbox("FOV Changer", &Settings::Misc::misc_FovChanger);
		ImGui::PushItemWidth(362.f);
		ImGui::SliderInt("FOV View", &Settings::Misc::misc_FovView, 1, 170);
		ImGui::SliderInt("FOV Model View", &Settings::Misc::misc_FovModelView, 1, 190);
		ImGui::Separator();

		ImGui::Checkbox("Third Person", &Settings::Misc::misc_ThirdPerson);
		ImGui::SliderFloat("##THIRDPERSONRANGE", &Settings::Misc::misc_ThirdPersonRange, 0.f, 500.f, "Range: %0.f");
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("sv_cheats 1/smac ban");


		ImGui::SliderFloat("##FAKELAG_AMOUNT", &Settings::Misc::misc_fakelag_amount, 0.f, 20.f, "Fake Lag Amount: %0.f");

		ImGui::Separator();

		string clan_1 = "None";
		string clan_2 = "Clear";
		string clan_3 = "smef.pw";
		string clan_4 = "smef.pw No-name";
		string clan_5 = "Valve";
		string clan_6 = "Valve No-name";
		string clan_7 = "Animation";
		const char* items5[] = { clan_1.c_str() , clan_2.c_str() , clan_3.c_str() , clan_4.c_str() , clan_5.c_str() , clan_6.c_str() , clan_7.c_str() };
		ImGui::Combo("Clantag Changer", &Settings::Misc::misc_Clan, items5, IM_ARRAYSIZE(items5));
		ImGui::Separator();
		ImGui::Spacing();

		if (ImGui::Combo("", &Settings::Misc::misc_CurrentSky, skybox_items, IM_ARRAYSIZE(skybox_items)))
		{
			Settings::Misc::misc_SkyName = skybox_items[Settings::Misc::misc_CurrentSky];
		}
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("sv_cheats 1/smac ban");
		ImGui::SameLine();
		ImGui::Text("SkyBox");

		ImGui::Checkbox("No Sky", &Settings::Misc::misc_NoSky);
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("sv_cheats 1/smac ban");
		ImGui::SameLine(SpaceLineOne);
		ImGui::Checkbox("Disable Postprocess", &Settings::Misc::misc_EPostprocess);
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("sv_cheats 1/smac ban");
	}

	void DrawColors()
	{
		ImGui::MyColorEdit3("Color CT", Settings::Esp::esp_Color_CT);
		ImGui::MyColorEdit3("Color T", Settings::Esp::esp_Color_TT);
		ImGui::MyColorEdit3("Color Visible CT", Settings::Esp::esp_Color_VCT);
		ImGui::MyColorEdit3("Color Visible T", Settings::Esp::esp_Color_VTT);
		ImGui::MyColorEdit3("Chams Color CT", Settings::Esp::chams_Color_CT);
		ImGui::MyColorEdit3("Chams Color T", Settings::Esp::chams_Color_TT);
		ImGui::MyColorEdit3("Chams Color Visible CT", Settings::Esp::chams_Color_VCT);
		ImGui::MyColorEdit3("Chams Color Visible T", Settings::Esp::chams_Color_VTT);
		ImGui::MyColorEdit3("Hitmarker", Settings::Esp::esp_HitMarkerColor);
		ImGui::MyColorEdit3("Dynamic Lights", Settings::Esp::esp_Dlight);

		/*		ImGui::Text("Radar");
		ImGui::Separator();
		ImGui::ColorEdit3("Color CT", Settings::Radar::rad_Color_CT);
		ImGui::ColorEdit3("Color T", Settings::Radar::rad_Color_TT);
		ImGui::ColorEdit3("Color Visible CT", Settings::Radar::rad_Color_VCT);
		ImGui::ColorEdit3("Color Visible T", Settings::Radar::rad_Color_VTT);*/
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
		ImGui::SetNextWindowPosCenter(ImGuiSetCond_Appearing);
		BtnNormal();
		//style.WindowPadding = ImVec2(0, 0);
		ImGui::Begin("!smef.pw", &bIsGuiVisible, ImVec2(828, 450), 0.98f, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_ShowBorders | ImGuiWindowFlags_NoMove);
		{
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
				DrawRadar();
			if (Global::MenuTab == 5)
				DrawColors();
		}ImGui::End();
		ImGui::SetNextWindowPos(ImVec2(mainWindowPos.x - 6, mainWindowPos.y - 6));
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.f);
		ImGui::Begin("##border2", &bIsGuiVisible, ImVec2(840, 462), 0.98f, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_ShowBorders);
		{
		}
		ImGui::End();

		//left bar with buttons
		ImGui::SetNextWindowPos(ImVec2(0, -30));
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.1f, 0.1f, 0.1f, 1.f);
		ImGui::Begin("##exdee", &bIsGuiVisible, ImVec2(180, 1110), 0.5f, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoInputs);
		{
		}
		ImGui::End();

		ImGui::SetNextWindowPos(ImVec2(10, 150));
		style.Colors[ImGuiCol_WindowBg] = ImVec4(1.f, 1.f, 1.f, 0.1f);
		style.WindowPadding = ImVec2(0, 0);//not
		style.ItemSpacing = ImVec2(8, 4);//not
		ImGui::Begin("##tabarea", &bIsGuiVisible, ImVec2(150, 620), 0.f, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoTitleBar);
		{
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
		}
		ImGui::End();
	}
}

using namespace Client;



bool bIsGuiInitalize = false;
bool bIsGuiVisible = false;
WNDPROC WndProc_o = nullptr;

#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))

LRESULT WINAPI GUI_WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

CGui::CGui() {}

CGui::~CGui()
{
	ImGui_ImplDX9_Shutdown();
}

void CGui::GUI_Init(IDirect3DDevice9 * pDevice)
{
	HWND hWindow = FindWindowA("Valve001", 0);


	ImGui_ImplDX9_Init(hWindow, pDevice);

	ImGuiIO& io = ImGui::GetIO();
	ImGuiStyle& style = ImGui::GetStyle();

	io.IniFilename = GuiFile.c_str();

	font = io.Fonts->AddFontFromMemoryCompressedTTF(MyFont_compressed_data2, MyFont_compressed_size2, 14.f);

	tabfont = io.Fonts->AddFontFromMemoryCompressedTTF(MyFont_compressed_data, MyFont_compressed_size, 62.f);

	style.Alpha = 0.0f;
	style.WindowPadding = ImVec2(8, 8);
	style.WindowMinSize = ImVec2(32, 32);
	style.WindowRounding = 0.5f;
	style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
	style.ChildWindowRounding = 0.0f;
	style.FramePadding = ImVec2(4, 2);
	style.FrameRounding = 0.0f;
	style.ItemSpacing = ImVec2(8, 4);
	style.ItemInnerSpacing = ImVec2(4, 4);
	style.TouchExtraPadding = ImVec2(0, 0);
	style.IndentSpacing = 21.0f;
	style.ColumnsMinSpacing = 3.0f;
	style.ScrollbarSize = 12.0f;
	style.ScrollbarRounding = 0.0f;
	style.GrabMinSize = 0.1f;
	style.GrabRounding = 0.0f;
	style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
	style.DisplayWindowPadding = ImVec2(22, 22);
	style.DisplaySafeAreaPadding = ImVec2(4, 4);
	style.AntiAliasedLines = true;
	style.AntiAliasedShapes = true;
	style.CurveTessellationTol = 1.25f;

	MenuColor();

	ImGui_ImplDX9_CreateDeviceObjects();

	WndProc_o = (WNDPROC)SetWindowLongA(hWindow, GWL_WNDPROC, (LONG)(LONG_PTR)GUI_WndProc);

	bIsGuiInitalize = true;
}

void CGui::GUI_Begin_Render()
{
	ImGui_ImplDX9_NewFrame();
}

void CGui::GUI_End_Render()
{
	ImGui::Render();
}

LRESULT WINAPI GUI_WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static bool is_down = false;
	static bool is_clicked = false;
	static bool check_closed = false;

	if (GUI_KEY_DOWN(VK_INSERT))
	{
		is_clicked = false;
		is_down = true;
	}
	else if (!GUI_KEY_DOWN(VK_INSERT) && is_down)
	{
		is_clicked = true;
		is_down = false;
	}
	else
	{
		is_clicked = false;
		is_down = false;
	}

	if (!bIsGuiVisible && !is_clicked && check_closed)
	{
		string msg = "cl_mouseenable " + to_string(!bIsGuiVisible);
		Interfaces::Engine()->ClientCmd_Unrestricted2(msg.c_str());
		check_closed = false;
	}

	if (is_clicked)
	{
		bIsGuiVisible = !bIsGuiVisible;

		string msg = "cl_mouseenable " + to_string(!bIsGuiVisible);
		Interfaces::Engine()->ClientCmd_Unrestricted2(msg.c_str());

		if (!check_closed)
			check_closed = true;
	}

	if (bIsGuiVisible && ImGui_ImplDX9_WndProcHandler(hwnd, uMsg, wParam, lParam))
		return true;

	return CallWindowProcA(WndProc_o, hwnd, uMsg, wParam, lParam);
}

void CGui::GUI_Draw_Elements()
{
	if (!bIsGuiInitalize || Interfaces::Engine()->IsTakingScreenshot() || !Interfaces::Engine()->IsActiveApp())
		return;

	g_pGui->GUI_Begin_Render();

	ImGui::GetIO().MouseDrawCursor = bIsGuiVisible;

	bool bOpenTimer = (bIsGuiVisible || (bC4Timer && iC4Timer));

	if (Settings::Misc::misc_Spectators) g_pMisc->OnRenderSpectatorList();

	if (g_pEsp && Settings::Esp::esp_BombTimer && bOpenTimer)
	{
		ImVec2 OldMinSize = ImGui::GetStyle().WindowMinSize;

		ImGui::GetStyle().WindowMinSize = ImVec2(0.f, 0.f);

		ImGui::SetNextWindowSize(ImVec2(125.f, 30.f));

		if (ImGui::Begin("Bomb Timer", &bOpenTimer,
			ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar))
		{
			ImGui::Text(C4_TIMER_STRING, g_pEsp->fC4Timer);
			ImGui::End();
		}
		ImGui::GetStyle().WindowMinSize = OldMinSize;
	}

	if (bIsGuiVisible)
	{
		int pX, pY;
		Interfaces::InputSystem()->GetCursorPosition(&pX, &pY);

		ImGui::GetIO().MousePos.x = (float)pX;
		ImGui::GetIO().MousePos.y = (float)pY;

		Client::OnRenderGUI();
	}

	if (g_pRadar)
		g_pRadar->OnRender();

	g_pGui->GUI_End_Render();
}

//Change color stuff \/

void CGui::MenuColor()
{
	ImGuiStyle& style = ImGui::GetStyle();

	static int hue = 140;

	ImVec4 col_text = ImColor::HSV(hue / 255.f, 20.f / 255.f, 235.f / 255.f);
	ImVec4 col_main = ImColor(9, 82, 128);
	ImVec4 col_back = ImColor(31, 44, 54);
	ImVec4 col_area = ImColor(4, 32, 41);
}

void CGui::purple()
{
	ImGuiStyle& style = ImGui::GetStyle();

	style.Colors[ImGuiCol_Text] = ImVec4(0.87f, 0.85f, 0.92f, 1.00f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.87f, 0.85f, 0.92f, 0.58f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.13f, 0.12f, 0.16f, 0.71f);
	style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.27f, 0.20f, 0.39f, 0.00f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.05f, 0.05f, 0.10f, 0.90f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.87f, 0.85f, 0.92f, 0.30f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.27f, 0.20f, 0.39f, 1.00f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.34f, 0.19f, 0.63f, 0.68f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.46f, 0.27f, 0.80f, 1.00f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.34f, 0.19f, 0.63f, 0.45f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.34f, 0.19f, 0.63f, 0.35f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.34f, 0.19f, 0.63f, 0.78f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.27f, 0.20f, 0.39f, 0.57f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.27f, 0.20f, 0.39f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.34f, 0.19f, 0.63f, 0.31f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.34f, 0.19f, 0.63f, 0.78f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.34f, 0.19f, 0.63f, 1.00f);
	style.Colors[ImGuiCol_ComboBg] = ImVec4(0.27f, 0.20f, 0.39f, 1.00f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.34f, 0.19f, 0.63f, 0.80f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.34f, 0.19f, 0.63f, 0.24f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.34f, 0.19f, 0.63f, 1.00f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.34f, 0.19f, 0.63f, 0.44f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.34f, 0.19f, 0.63f, 0.86f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.34f, 0.19f, 0.63f, 1.00f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.34f, 0.19f, 0.63f, 0.76f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.34f, 0.19f, 0.63f, 0.86f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.34f, 0.19f, 0.63f, 1.00f);
	style.Colors[ImGuiCol_Column] = ImVec4(0.87f, 0.85f, 0.92f, 0.32f);
	style.Colors[ImGuiCol_ColumnHovered] = ImVec4(0.87f, 0.85f, 0.92f, 0.78f);
	style.Colors[ImGuiCol_ColumnActive] = ImVec4(0.87f, 0.85f, 0.92f, 1.00f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.34f, 0.19f, 0.63f, 0.20f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.34f, 0.19f, 0.63f, 0.78f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.34f, 0.19f, 0.63f, 1.00f);
	style.Colors[ImGuiCol_CloseButton] = ImVec4(0.87f, 0.85f, 0.92f, 0.16f);
	style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.87f, 0.85f, 0.92f, 0.39f);
	style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.87f, 0.85f, 0.92f, 1.00f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.87f, 0.85f, 0.92f, 0.63f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.34f, 0.19f, 0.63f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.87f, 0.85f, 0.92f, 0.63f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.34f, 0.19f, 0.63f, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.34f, 0.19f, 0.63f, 0.43f);
	style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
}

void CGui::DefaultSheme1()
{
	ImGuiStyle& style = ImGui::GetStyle();

	style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.94f, 0.94f, 0.94f, 1.f);
	style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(1.00f, 1.00f, 1.00f, 1.f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.00f, 0.00f, 0.00f, 0.39f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(1.00f, 1.00f, 1.00f, 0.10f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.94f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.96f, 0.96f, 0.96f, 1.00f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 1.00f, 1.00f, 0.51f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.82f, 0.82f, 0.82f, 1.00f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.98f, 0.98f, 0.98f, 0.53f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.69f, 0.69f, 0.69f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.59f, 0.59f, 0.59f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.49f, 0.49f, 0.49f, 1.00f);
	style.Colors[ImGuiCol_ComboBg] = ImVec4(0.86f, 0.86f, 0.86f, 0.99f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.24f, 0.52f, 0.88f, 1.00f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_Column] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
	style.Colors[ImGuiCol_ColumnHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.78f);
	style.Colors[ImGuiCol_ColumnActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.50f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
	style.Colors[ImGuiCol_CloseButton] = ImVec4(0.59f, 0.59f, 0.59f, 0.50f);
	style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.98f, 0.39f, 0.36f, 1.00f);
	style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.98f, 0.39f, 0.36f, 1.00f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
	style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
}

void CGui::RedSheme()
{
	ImGuiStyle& style = ImGui::GetStyle();

	style.Colors[ImGuiCol_Text] = ImVec4(0.86f, 0.93f, 0.89f, 0.78f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.92f, 0.18f, 0.29f, 0.78f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.13f, 0.14f, 0.17f, 1.00f);
	style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.20f, 0.22f, 0.27f, 0.58f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.00f, 0.00f, 0.00f, 0.35f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.92f, 0.18f, 0.29f, 0.78f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.20f, 0.22f, 0.27f, 0.75f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.20f, 0.22f, 0.27f, 0.47f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.09f, 0.15f, 0.16f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.92f, 0.18f, 0.29f, 0.78f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_ComboBg] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.71f, 0.18f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.92f, 0.18f, 0.29f, 0.37f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.92f, 0.18f, 0.29f, 0.75f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.92f, 0.18f, 0.29f, 0.86f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.92f, 0.18f, 0.29f, 0.76f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.92f, 0.18f, 0.29f, 0.86f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_Column] = ImVec4(0.15f, 0.00f, 0.00f, 0.35f);
	style.Colors[ImGuiCol_ColumnHovered] = ImVec4(0.92f, 0.18f, 0.29f, 0.59f);
	style.Colors[ImGuiCol_ColumnActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.92f, 0.18f, 0.29f, 0.63f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.92f, 0.18f, 0.29f, 0.78f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_CloseButton] = ImVec4(1.00f, 1.00f, 1.00f, 0.51f);
	style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.67f);
	style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.78f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.86f, 0.93f, 0.89f, 0.63f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.86f, 0.93f, 0.89f, 0.63f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.92f, 0.18f, 0.29f, 0.43f);
	style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.22f, 0.27f, 0.73f);
}

void CGui::darkblue()
{
	ImGuiStyle& style = ImGui::GetStyle();

	style.Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.80f, 0.80f, 0.83f, 0.88f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style.Colors[ImGuiCol_ComboBg] = ImVec4(0.19f, 0.18f, 0.21f, 1.00f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style.Colors[ImGuiCol_Column] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style.Colors[ImGuiCol_ColumnHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_ColumnActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style.Colors[ImGuiCol_CloseButton] = ImVec4(0.40f, 0.39f, 0.38f, 0.16f);
	style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.40f, 0.39f, 0.38f, 0.39f);
	style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.40f, 0.39f, 0.38f, 1.00f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
	style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);
}

void CGui::MidNightSheme()
{
	ImGuiStyle& style = ImGui::GetStyle();

	style.Colors[ImGuiCol_Text] = ImVec4(0.85f, 0.89f, 0.92f, 1.00f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.85f, 0.89f, 0.92f, 0.58f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.12f, 0.14f, 0.16f, 1.00f);
	style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.20f, 0.30f, 0.39f, 0.00f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.05f, 0.05f, 0.10f, 0.90f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.85f, 0.89f, 0.92f, 0.30f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.30f, 0.39f, 1.00f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.32f, 0.60f, 0.92f, 0.68f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.19f, 0.43f, 0.63f, 1.00f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.77f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.77f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.00f, 0.00f, 0.00f, 0.77f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.08f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.00f, 0.47f, 1.00f, 0.31f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.10f, 0.38f, 0.62f, 0.78f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.13f, 0.44f, 0.72f, 1.00f);
	style.Colors[ImGuiCol_ComboBg] = ImVec4(0.12f, 0.45f, 0.55f, 1.00f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.00f, 0.00f, 0.00f, 0.77f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.00f, 0.00f, 0.00f, 0.77f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.00f, 0.00f, 0.00f, 0.77f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.05f, 0.27f, 0.48f, 0.59f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.00f, 0.42f, 0.44f, 1.00f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.05f, 0.27f, 0.48f, 0.59f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.00f, 0.24f, 0.44f, 1.00f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.00f, 0.42f, 0.44f, 1.00f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.00f, 0.42f, 0.44f, 1.00f);
	style.Colors[ImGuiCol_Column] = ImVec4(0.85f, 0.89f, 0.92f, 0.32f);
	style.Colors[ImGuiCol_ColumnHovered] = ImVec4(0.85f, 0.89f, 0.92f, 0.78f);
	style.Colors[ImGuiCol_ColumnActive] = ImVec4(0.85f, 0.89f, 0.92f, 1.00f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.19f, 0.43f, 0.63f, 0.20f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.19f, 0.43f, 0.63f, 0.78f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.19f, 0.43f, 0.63f, 1.00f);
	style.Colors[ImGuiCol_CloseButton] = ImVec4(0.85f, 0.89f, 0.92f, 0.16f);
	style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.85f, 0.89f, 0.92f, 0.39f);
	style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.85f, 0.89f, 0.92f, 1.00f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.85f, 0.89f, 0.92f, 0.63f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.19f, 0.43f, 0.63f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.85f, 0.89f, 0.92f, 0.63f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.19f, 0.43f, 0.63f, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.19f, 0.43f, 0.63f, 0.43f);
	style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
}

void CGui::NightSheme()
{
	ImGuiStyle& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_Text] = ImVec4(0.85f, 0.87f, 0.92f, 1.00f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.85f, 0.87f, 0.92f, 0.58f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.14f, 0.19f, 0.36f, 0.00f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.05f, 0.05f, 0.10f, 0.90f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.85f, 0.87f, 0.92f, 0.30f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.14f, 0.19f, 0.36f, 1.00f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.10f, 0.19f, 0.49f, 0.68f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.10f, 0.19f, 0.49f, 1.00f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.19f, 0.49f, 0.45f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.10f, 0.19f, 0.49f, 0.35f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.10f, 0.19f, 0.49f, 0.78f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.19f, 0.36f, 0.57f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.14f, 0.19f, 0.36f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.10f, 0.19f, 0.49f, 0.31f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.10f, 0.19f, 0.49f, 0.78f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.10f, 0.19f, 0.49f, 1.00f);
	style.Colors[ImGuiCol_ComboBg] = ImVec4(0.14f, 0.19f, 0.36f, 1.00f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.16f, 0.86f, 0.90f, 0.80f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.10f, 0.19f, 0.49f, 0.24f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.07f, 0.26f, 0.53f, 1.00f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.10f, 0.19f, 0.49f, 0.44f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.10f, 0.19f, 0.49f, 0.86f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.10f, 0.19f, 0.49f, 1.00f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.19f, 0.49f, 0.76f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.10f, 0.19f, 0.49f, 0.86f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.10f, 0.19f, 0.49f, 1.00f);
	style.Colors[ImGuiCol_Column] = ImVec4(0.85f, 0.87f, 0.92f, 0.32f);
	style.Colors[ImGuiCol_ColumnHovered] = ImVec4(0.85f, 0.87f, 0.92f, 0.78f);
	style.Colors[ImGuiCol_ColumnActive] = ImVec4(0.85f, 0.87f, 0.92f, 1.00f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.10f, 0.19f, 0.49f, 0.20f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.10f, 0.19f, 0.49f, 0.78f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.10f, 0.19f, 0.49f, 1.00f);
	style.Colors[ImGuiCol_CloseButton] = ImVec4(0.85f, 0.87f, 0.92f, 0.16f);
	style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.85f, 0.87f, 0.92f, 0.39f);
	style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.85f, 0.87f, 0.92f, 1.00f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.85f, 0.87f, 0.92f, 0.63f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.10f, 0.19f, 0.49f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.85f, 0.87f, 0.92f, 0.63f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.10f, 0.19f, 0.49f, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.10f, 0.19f, 0.49f, 0.43f);
	style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
}

void CGui::DunnoSheme()
{
	ImGuiStyle& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_Text] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.70f);
	style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.65f, 0.00f, 0.06f, 0.03f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.70f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.00f, 0.00f, 0.00f, 0.70f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.00f, 0.00f, 0.06f, 0.00f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.00f, 0.00f, 0.00f, 0.70f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.00f, 0.00f, 0.06f, 0.00f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.65f, 0.00f, 0.06f, 3.14f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.65f, 0.00f, 0.06f, 3.14f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.65f, 0.00f, 0.06f, 3.14f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.00f, 0.00f, 0.00f, 3.14f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.00f, 0.00f, 0.00f, 3.14f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.65f, 0.00f, 0.06f, 3.14f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.65f, 0.00f, 0.06f, 3.14f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.65f, 0.00f, 0.06f, 3.14f);
	style.Colors[ImGuiCol_ComboBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.90f, 0.90f, 0.90f, 0.50f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.00f, 0.00f, 0.06f, 3.14f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.65f, 0.00f, 0.06f, 3.14f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.00f, 0.00f, 0.06f, 3.14f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.00f, 0.42f, 0.44f, 1.00f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.05f, 0.27f, 0.48f, 0.59f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.00f, 0.24f, 0.44f, 1.00f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.00f, 0.42f, 0.44f, 1.00f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.00f, 0.42f, 0.44f, 1.00f);
	style.Colors[ImGuiCol_Column] = ImVec4(0.85f, 0.89f, 0.92f, 0.32f);
	style.Colors[ImGuiCol_ColumnHovered] = ImVec4(0.85f, 0.89f, 0.92f, 0.78f);
	style.Colors[ImGuiCol_ColumnActive] = ImVec4(0.85f, 0.89f, 0.92f, 1.00f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.19f, 0.43f, 0.63f, 0.20f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.19f, 0.43f, 0.63f, 0.78f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.19f, 0.43f, 0.63f, 1.00f);
	style.Colors[ImGuiCol_CloseButton] = ImVec4(0.85f, 0.89f, 0.92f, 0.16f);
	style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.85f, 0.89f, 0.92f, 0.39f);
	style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.85f, 0.89f, 0.92f, 1.00f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.85f, 0.89f, 0.92f, 0.63f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.19f, 0.43f, 0.63f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.85f, 0.89f, 0.92f, 0.63f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.19f, 0.43f, 0.63f, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.19f, 0.43f, 0.63f, 0.43f);
	style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
}

void CGui::BlueSheme()
{
	ImGuiStyle& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_Text] = ImVec4(0.86f, 0.93f, 0.89f, 0.78f); // - 
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.13f, 0.65f, 0.92f, 0.78f); // - 
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.13f, 0.14f, 0.17f, 1.00f); // - 
	style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.20f, 0.22f, 0.27f, 0.58f); // - 
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f); // - 
	style.Colors[ImGuiCol_Border] = ImVec4(0.00f, 0.00f, 0.00f, 0.35f); // - 
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f); // - 
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f); // - 
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.13f, 0.65f, 0.92f, 0.78f); // + 
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.13f, 0.65f, 0.92f, 1.00f); // + 
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f); // - 
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.20f, 0.22f, 0.27f, 0.75f); // - 
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.13f, 0.65f, 0.92f, 1.00f); // + 
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.20f, 0.22f, 0.27f, 0.47f); // - 
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f); // - 
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.09f, 0.15f, 0.16f, 1.00f); // - 
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.13f, 0.65f, 0.92f, 0.78f); // + 
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.13f, 0.65f, 0.92f, 1.00f); // + 
	style.Colors[ImGuiCol_ComboBg] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f); // - 
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.13f, 0.65f, 0.92f, 1.00f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.13f, 0.65f, 0.92f, 0.37f); // + 
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.13f, 0.65f, 0.92f, 1.00f); // + 
	style.Colors[ImGuiCol_Button] = ImVec4(0.13f, 0.65f, 0.92f, 0.75f); // + 
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.13f, 0.65f, 0.92f, 0.86f); // + 
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.13f, 0.65f, 0.92f, 1.00f); // + 
	style.Colors[ImGuiCol_Header] = ImVec4(0.13f, 0.65f, 0.92f, 0.76f); // + 
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.13f, 0.65f, 0.92f, 0.86f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.13f, 0.65f, 0.92f, 1.00f); // + 
	style.Colors[ImGuiCol_Column] = ImVec4(0.15f, 0.00f, 0.00f, 0.35f); // - 
	style.Colors[ImGuiCol_ColumnHovered] = ImVec4(0.13f, 0.65f, 0.92f, 0.59f);
	style.Colors[ImGuiCol_ColumnActive] = ImVec4(0.13f, 0.65f, 0.92f, 1.00f); // + 
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.13f, 0.65f, 0.92f, 0.63f); // + 
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.13f, 0.65f, 0.92f, 0.78f); // + 
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.13f, 0.65f, 0.92f, 1.00f); // + 
	style.Colors[ImGuiCol_CloseButton] = ImVec4(1.00f, 1.00f, 1.00f, 0.51f); // - 
	style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.67f); // - 
	style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.78f); // - 
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.86f, 0.93f, 0.89f, 0.63f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.13f, 0.65f, 0.92f, 1.00f); // + 
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.86f, 0.93f, 0.89f, 0.63f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.13f, 0.65f, 0.92f, 1.00f); // + 
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.13f, 0.65f, 0.92f, 0.43f); // + 
	style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.22f, 0.27f, 0.73f); // - 
}

void CGui::MidNight2()
{
	ImGuiStyle& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.94f, 0.94f, 0.94f, 1.f);
	style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(1.00f, 1.00f, 1.00f, 1.f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.00f, 0.00f, 0.00f, 0.39f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(1.00f, 1.00f, 1.00f, 0.10f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.94f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.96f, 0.96f, 0.96f, 1.00f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 1.00f, 1.00f, 0.51f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.82f, 0.82f, 0.82f, 1.00f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.98f, 0.98f, 0.98f, 0.53f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.69f, 0.69f, 0.69f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.59f, 0.59f, 0.59f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.49f, 0.49f, 0.49f, 1.00f);
	style.Colors[ImGuiCol_ComboBg] = ImVec4(0.86f, 0.86f, 0.86f, 0.99f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.24f, 0.52f, 0.88f, 1.00f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_Column] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
	style.Colors[ImGuiCol_ColumnHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.78f);
	style.Colors[ImGuiCol_ColumnActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.50f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
	style.Colors[ImGuiCol_CloseButton] = ImVec4(0.59f, 0.59f, 0.59f, 0.50f);
	style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.98f, 0.39f, 0.36f, 1.00f);
	style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.98f, 0.39f, 0.36f, 1.00f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
	style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
}

void CGui::BlackSheme2()
{
	ImGuiStyle& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.80f, 0.80f, 0.83f, 0.88f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style.Colors[ImGuiCol_ComboBg] = ImVec4(0.19f, 0.18f, 0.21f, 1.00f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style.Colors[ImGuiCol_Column] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style.Colors[ImGuiCol_ColumnHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_ColumnActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style.Colors[ImGuiCol_CloseButton] = ImVec4(0.40f, 0.39f, 0.38f, 0.16f);
	style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.40f, 0.39f, 0.38f, 0.39f);
	style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.40f, 0.39f, 0.38f, 1.00f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
	style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);
}

void CGui::green()
{
	ImGuiStyle& style = ImGui::GetStyle();

	style.Colors[ImGuiCol_Text] = ImVec4(0.89f, 0.92f, 0.85f, 1.00f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.89f, 0.92f, 0.85f, 0.58f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.17f, 0.20f, 0.14f, 0.55f);
	style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.37f, 0.49f, 0.20f, 0.00f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.05f, 0.05f, 0.10f, 0.90f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.89f, 0.92f, 0.85f, 0.30f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.37f, 0.49f, 0.20f, 1.00f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.53f, 0.78f, 0.17f, 0.68f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.53f, 0.78f, 0.17f, 1.00f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.53f, 0.78f, 0.17f, 0.45f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.53f, 0.78f, 0.17f, 0.35f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.53f, 0.78f, 0.17f, 0.78f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.37f, 0.49f, 0.20f, 0.57f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.37f, 0.49f, 0.20f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.53f, 0.78f, 0.17f, 0.31f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.53f, 0.78f, 0.17f, 0.78f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.53f, 0.78f, 0.17f, 1.00f);
	style.Colors[ImGuiCol_ComboBg] = ImVec4(0.37f, 0.49f, 0.20f, 1.00f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.53f, 0.78f, 0.17f, 0.80f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.53f, 0.78f, 0.17f, 0.24f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.53f, 0.78f, 0.17f, 1.00f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.53f, 0.78f, 0.17f, 0.44f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.53f, 0.78f, 0.17f, 0.86f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.53f, 0.78f, 0.17f, 1.00f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.53f, 0.78f, 0.17f, 0.76f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.53f, 0.78f, 0.17f, 0.86f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.53f, 0.78f, 0.17f, 1.00f);
	style.Colors[ImGuiCol_Column] = ImVec4(0.89f, 0.92f, 0.85f, 0.32f);
	style.Colors[ImGuiCol_ColumnHovered] = ImVec4(0.89f, 0.92f, 0.85f, 0.78f);
	style.Colors[ImGuiCol_ColumnActive] = ImVec4(0.89f, 0.92f, 0.85f, 1.00f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.53f, 0.78f, 0.17f, 0.20f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.53f, 0.78f, 0.17f, 0.78f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.53f, 0.78f, 0.17f, 1.00f);
	style.Colors[ImGuiCol_CloseButton] = ImVec4(0.89f, 0.92f, 0.85f, 0.16f);
	style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.89f, 0.92f, 0.85f, 0.39f);
	style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.89f, 0.92f, 0.85f, 1.00f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.89f, 0.92f, 0.85f, 0.63f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.53f, 0.78f, 0.17f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.89f, 0.92f, 0.85f, 0.63f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.53f, 0.78f, 0.17f, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.53f, 0.78f, 0.17f, 0.43f);
	style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);

}

void CGui::pink()
{
	ImGuiStyle& style = ImGui::GetStyle();

	style.Colors[ImGuiCol_Text] = ImVec4(0.92f, 0.92f, 0.85f, 1.00f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.92f, 0.92f, 0.85f, 0.58f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.20f, 0.19f, 0.14f, 0.55f);
	style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.49f, 0.47f, 0.20f, 0.00f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.05f, 0.05f, 0.10f, 0.90f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.92f, 0.92f, 0.85f, 0.30f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.49f, 0.47f, 0.20f, 1.00f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.78f, 0.75f, 0.17f, 0.68f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.78f, 0.75f, 0.17f, 1.00f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.78f, 0.75f, 0.17f, 0.45f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.78f, 0.75f, 0.17f, 0.35f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.78f, 0.75f, 0.17f, 0.78f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.49f, 0.47f, 0.20f, 0.57f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.49f, 0.47f, 0.20f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.78f, 0.75f, 0.17f, 0.31f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.78f, 0.75f, 0.17f, 0.78f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.78f, 0.75f, 0.17f, 1.00f);
	style.Colors[ImGuiCol_ComboBg] = ImVec4(0.49f, 0.47f, 0.20f, 1.00f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.78f, 0.75f, 0.17f, 0.80f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.78f, 0.75f, 0.17f, 0.24f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.78f, 0.75f, 0.17f, 1.00f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.78f, 0.75f, 0.17f, 0.44f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.78f, 0.75f, 0.17f, 0.86f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.78f, 0.75f, 0.17f, 1.00f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.78f, 0.75f, 0.17f, 0.76f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.78f, 0.75f, 0.17f, 0.86f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.78f, 0.75f, 0.17f, 1.00f);
	style.Colors[ImGuiCol_Column] = ImVec4(0.92f, 0.92f, 0.85f, 0.32f);
	style.Colors[ImGuiCol_ColumnHovered] = ImVec4(0.92f, 0.92f, 0.85f, 0.78f);
	style.Colors[ImGuiCol_ColumnActive] = ImVec4(0.92f, 0.92f, 0.85f, 1.00f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.78f, 0.75f, 0.17f, 0.20f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.78f, 0.75f, 0.17f, 0.78f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.78f, 0.75f, 0.17f, 1.00f);
	style.Colors[ImGuiCol_CloseButton] = ImVec4(0.92f, 0.92f, 0.85f, 0.16f);
	style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.92f, 0.92f, 0.85f, 0.39f);
	style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.92f, 0.92f, 0.85f, 1.00f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.92f, 0.92f, 0.85f, 0.63f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.78f, 0.75f, 0.17f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.92f, 0.92f, 0.85f, 0.63f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.78f, 0.75f, 0.17f, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.78f, 0.75f, 0.17f, 0.43f);
	style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
}
void CGui::blue()
{
	ImGuiStyle& style = ImGui::GetStyle();

	style.Colors[ImGuiCol_Text] = ImVec4(0.85f, 0.87f, 0.92f, 1.00f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.85f, 0.87f, 0.92f, 0.58f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.14f, 0.15f, 0.20f, 0.55f);
	style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.20f, 0.27f, 0.49f, 0.00f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.05f, 0.05f, 0.10f, 0.90f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.85f, 0.87f, 0.92f, 0.30f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.27f, 0.49f, 1.00f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.17f, 0.31f, 0.78f, 0.68f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.17f, 0.31f, 0.78f, 1.00f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.17f, 0.31f, 0.78f, 0.45f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.17f, 0.31f, 0.78f, 0.35f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.17f, 0.31f, 0.78f, 0.78f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.20f, 0.27f, 0.49f, 0.57f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.20f, 0.27f, 0.49f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.17f, 0.31f, 0.78f, 0.31f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.17f, 0.31f, 0.78f, 0.78f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.17f, 0.31f, 0.78f, 1.00f);
	style.Colors[ImGuiCol_ComboBg] = ImVec4(0.20f, 0.27f, 0.49f, 1.00f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.17f, 0.31f, 0.78f, 0.80f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.17f, 0.31f, 0.78f, 0.24f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.17f, 0.31f, 0.78f, 1.00f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.17f, 0.31f, 0.78f, 0.44f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.17f, 0.31f, 0.78f, 0.86f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.17f, 0.31f, 0.78f, 1.00f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.17f, 0.31f, 0.78f, 0.76f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.17f, 0.31f, 0.78f, 0.86f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.17f, 0.31f, 0.78f, 1.00f);
	style.Colors[ImGuiCol_Column] = ImVec4(0.85f, 0.87f, 0.92f, 0.32f);
	style.Colors[ImGuiCol_ColumnHovered] = ImVec4(0.85f, 0.87f, 0.92f, 0.78f);
	style.Colors[ImGuiCol_ColumnActive] = ImVec4(0.85f, 0.87f, 0.92f, 1.00f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.17f, 0.31f, 0.78f, 0.20f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.17f, 0.31f, 0.78f, 0.78f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.17f, 0.31f, 0.78f, 1.00f);
	style.Colors[ImGuiCol_CloseButton] = ImVec4(0.85f, 0.87f, 0.92f, 0.16f);
	style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.85f, 0.87f, 0.92f, 0.39f);
	style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.85f, 0.87f, 0.92f, 1.00f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.85f, 0.87f, 0.92f, 0.63f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.17f, 0.31f, 0.78f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.85f, 0.87f, 0.92f, 0.63f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.17f, 0.31f, 0.78f, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.17f, 0.31f, 0.78f, 0.43f);
	style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);

}

void CGui::yellow()
{
	ImGuiStyle& style = ImGui::GetStyle();

	style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.51f, 0.51f, 0.51f, 0.55f);
	style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.35f, 0.33f, 0.33f, 0.00f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.86f, 0.86f, 0.86f, 0.30f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.35f, 0.33f, 0.33f, 1.00f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.82f, 0.82f, 0.82f, 0.92f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(1.02f, 0.94f, 0.94f, 0.45f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.02f, 0.94f, 0.94f, 0.35f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(1.02f, 0.94f, 0.94f, 0.78f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.35f, 0.33f, 0.33f, 0.57f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.35f, 0.33f, 0.33f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(1.02f, 0.94f, 0.94f, 0.31f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(1.02f, 0.94f, 0.94f, 0.78f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(1.02f, 0.94f, 0.94f, 1.00f);
	style.Colors[ImGuiCol_ComboBg] = ImVec4(0.35f, 0.33f, 0.33f, 1.00f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(1.02f, 0.94f, 0.94f, 0.80f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(1.02f, 0.94f, 0.94f, 0.24f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(1.02f, 0.94f, 0.94f, 1.00f);
	style.Colors[ImGuiCol_Button] = ImVec4(1.02f, 0.94f, 0.94f, 0.44f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(1.02f, 0.94f, 0.94f, 0.86f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(1.02f, 0.94f, 0.94f, 1.00f);
	style.Colors[ImGuiCol_Header] = ImVec4(1.02f, 0.94f, 0.94f, 0.76f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(1.02f, 0.94f, 0.94f, 0.86f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(1.02f, 0.94f, 0.94f, 1.00f);
	style.Colors[ImGuiCol_Column] = ImVec4(0.92f, 0.85f, 0.85f, 0.32f);
	style.Colors[ImGuiCol_ColumnHovered] = ImVec4(0.92f, 0.85f, 0.85f, 0.78f);
	style.Colors[ImGuiCol_ColumnActive] = ImVec4(0.92f, 0.85f, 0.85f, 1.00f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(1.02f, 0.94f, 0.94f, 0.20f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.02f, 0.94f, 0.94f, 0.78f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(1.02f, 0.94f, 0.94f, 1.00f);
	style.Colors[ImGuiCol_CloseButton] = ImVec4(0.92f, 0.85f, 0.85f, 0.16f);
	style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.92f, 0.85f, 0.85f, 0.39f);
	style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.92f, 0.85f, 0.85f, 0.63f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.02f, 0.94f, 0.94f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.92f, 0.85f, 0.85f, 0.63f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.02f, 0.94f, 0.94f, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(1.02f, 0.94f, 0.94f, 0.43f);
	style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
}


void CGui::BlackSheme()
{
	ImGuiStyle& style = ImGui::GetStyle();

	style.Alpha = 1.0f;               // Global alpha applies to everything in ImGui
	style.WindowPadding = ImVec2(10, 10);     // Padding within a window
	style.WindowMinSize = ImVec2(100, 100);   // Minimum window size
	style.WindowRounding = 0.0f;               // Radius of window corners rounding. Set to 0.0f to have rectangular windows
	style.WindowTitleAlign = ImVec2(0.0f, 0.5f); // Alignment for title bar text
	style.ChildWindowRounding = 0.0f;               // Radius of child window corners rounding. Set to 0.0f to have rectangular child windows
	style.FramePadding = ImVec2(4, 3);       // Padding within a framed rectangle (used by most widgets)
	style.FrameRounding = 0.0f;               // Radius of frame corners rounding. Set to 0.0f to have rectangular frames (used by most widgets).
	style.ItemSpacing = ImVec2(5, 5);       // Horizontal and vertical spacing between widgets/lines
	style.ItemInnerSpacing = ImVec2(4, 4);       // Horizontal and vertical spacing between within elements of a composed widget (e.g. a slider and its label)
	style.TouchExtraPadding = ImVec2(0, 0);       // Expand reactive bounding box for touch-based system where touch position is not accurate enough. Unfortunately we don't sort widgets so priority on overlap will always be given to the first widget. So don't grow this too much!
	style.IndentSpacing = 21.0f;              // Horizontal spacing when e.g. entering a tree node. Generally == (FontSize + FramePadding.x*2).
	style.ColumnsMinSpacing = 6.0f;               // Minimum horizontal spacing between two columns
	style.ScrollbarSize = 16.0f;              // Width of the vertical scrollbar, Height of the horizontal scrollbar
	style.ScrollbarRounding = 9.0f;               // Radius of grab corners rounding for scrollbar
	style.GrabMinSize = 10.0f;              // Minimum width/height of a grab box for slider/scrollbar
	style.GrabRounding = 0.0f;               // Radius of grabs corners rounding. Set to 0.0f to have rectangular slider grabs.
	style.ButtonTextAlign = ImVec2(0.5f, 0.5f); // Alignment of button text when button is larger than text.
	style.DisplayWindowPadding = ImVec2(22, 22);     // Window positions are clamped to be visible within the display area by at least this amount. Only covers regular windows.
	style.DisplaySafeAreaPadding = ImVec2(4, 4);       // If you cannot see the edge of your screen (e.g. on a TV) increase the safe area padding. Covers popups/tooltips as well regular windows.
	style.AntiAliasedLines = true;               // Enable anti-aliasing on lines/borders. Disable if you are really short on CPU/GPU.
	style.AntiAliasedShapes = true;               // Enable anti-aliasing on filled shapes (rounded rectangles, circles, etc.)
	style.CurveTessellationTol = 1.25f;              // Tessellation tolerance. Decrease for highly tessellated curves (higher quality, more polygons), increase to reduce quality.
	style.Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.80f, 1.00f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 0.00f, 1.00f, 0.35f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
	style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.05f, 0.05f, 0.10f, 0.90f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.70f, 0.70f, 0.70f, 0.65f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.90f, 0.80f, 0.80f, 0.40f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.90f, 0.65f, 0.65f, 0.45f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 0.25f, 0.90f, 0.83f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.25f, 0.90f, 0.20f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.00f, 0.25f, 0.90f, 0.87f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.40f, 0.40f, 0.55f, 0.80f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.20f, 0.25f, 0.30f, 0.60f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.40f, 0.40f, 0.80f, 0.30f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40f, 0.40f, 0.80f, 0.40f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.80f, 0.50f, 0.50f, 0.40f);
	style.Colors[ImGuiCol_ComboBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.99f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.90f, 0.90f, 0.90f, 0.50f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(1.00f, 1.00f, 1.00f, 0.30f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.80f, 0.50f, 0.50f, 1.00f);
	style.Colors[ImGuiCol_Button] = style.Colors[ImGuiCol_WindowBg];
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.00f, 0.46f, 0.65f, 1.00f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.00f, 0.25f, 0.90f, 0.83f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.40f, 0.40f, 0.90f, 0.45f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.45f, 0.45f, 0.90f, 0.80f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.53f, 0.53f, 0.87f, 0.80f);
	style.Colors[ImGuiCol_Column] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	style.Colors[ImGuiCol_ColumnHovered] = ImVec4(0.70f, 0.60f, 0.60f, 1.00f);
	style.Colors[ImGuiCol_ColumnActive] = ImVec4(0.90f, 0.70f, 0.70f, 1.00f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.30f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.60f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
	style.Colors[ImGuiCol_CloseButton] = ImVec4(0.50f, 0.50f, 0.90f, 0.50f);
	style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.70f, 0.70f, 0.90f, 0.60f);
	style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
}

namespace ImGui
{
	static auto vector_getter = [](void* vec, int idx, const char** out_text)
	{
		auto& vector = *static_cast<std::vector<std::string>*>(vec);
		if (idx < 0 || idx >= static_cast<int>(vector.size())) { return false; }
		*out_text = vector.at(idx).c_str();
		return true;
	};

	IMGUI_API bool ComboBoxArray(const char* label, int* currIndex, std::vector<std::string>& values)
	{
		if (values.empty()) { return false; }
		return Combo(label, currIndex, vector_getter,
			static_cast<void*>(&values), values.size());
	}

	IMGUI_API bool TabLabels(const char **tabLabels, int tabSize, int &tabIndex, int *tabOrder)
	{
		ImGuiStyle& style = ImGui::GetStyle();

		const ImVec2 itemSpacing = style.ItemSpacing;
		const ImVec4 color = style.Colors[ImGuiCol_Button];
		const ImVec4 colorActive = style.Colors[ImGuiCol_ButtonActive];
		const ImVec4 colorHover = style.Colors[ImGuiCol_ButtonHovered];
		const ImVec4 colorText = style.Colors[ImGuiCol_Text];
		style.ItemSpacing.x = 2.5;
		style.ItemSpacing.y = 1;
		const ImVec4 colorSelectedTab = ImVec4(color.x, color.y, color.z, color.w*0.5f);
		const ImVec4 colorSelectedTabHovered = ImVec4(colorHover.x, colorHover.y, colorHover.z, colorHover.w*0.5f);
		const ImVec4 colorSelectedTabText = ImVec4(colorText.x*0.8f, colorText.y*0.8f, colorText.z*0.8f, colorText.w*0.8f);

		if (tabSize>0 && (tabIndex<0 || tabIndex >= tabSize))
		{
			if (!tabOrder)
			{
				tabIndex = 0;
			}
			else
			{
				tabIndex = -1;
			}
		}

		float windowWidth = 0.f, sumX = 0.f;
		windowWidth = ImGui::GetWindowWidth() - style.WindowPadding.x - (ImGui::GetScrollMaxY()>0 ? style.ScrollbarSize : 0.f);

		const bool isMMBreleased = ImGui::IsMouseReleased(2);
		int justClosedTabIndex = -1, newtabIndex = tabIndex;

		bool selection_changed = false; bool noButtonDrawn = true;

		for (int j = 0, i; j < tabSize; j++)
		{
			i = tabOrder ? tabOrder[j] : j;
			if (i == -1) continue;

			if (sumX > 0.f)
			{
				sumX += style.ItemSpacing.x;
				sumX += ImGui::CalcTextSize(tabLabels[i]).x + 2.f*style.FramePadding.x;

				if (sumX>windowWidth)
				{
					sumX = 0.f;
				}
				else
				{
					ImGui::SameLine();
				}
			}

			if (i != tabIndex)
			{
				// Push the style
				style.Colors[ImGuiCol_Button] = colorSelectedTab;
				style.Colors[ImGuiCol_ButtonActive] = colorSelectedTab;
				style.Colors[ImGuiCol_ButtonHovered] = colorSelectedTabHovered;
				style.Colors[ImGuiCol_Text] = colorSelectedTabText;
			}
			// Draw the button
			ImGui::PushID(i);   // otherwise two tabs with the same name would clash.
			if (ImGui::Button(tabLabels[i], ImVec2(67.f, 15.f))) { selection_changed = (tabIndex != i); newtabIndex = i; }
			ImGui::PopID();
			if (i != tabIndex)
			{
				// Reset the style
				style.Colors[ImGuiCol_Button] = color;
				style.Colors[ImGuiCol_ButtonActive] = colorActive;
				style.Colors[ImGuiCol_ButtonHovered] = colorHover;
				style.Colors[ImGuiCol_Text] = colorText;
			}
			noButtonDrawn = false;

			if (sumX == 0.f) sumX = style.WindowPadding.x + ImGui::GetItemRectSize().x; // First element of a line

		}

		tabIndex = newtabIndex;

		// Change selected tab when user closes the selected tab
		if (tabIndex == justClosedTabIndex && tabIndex >= 0)
		{
			tabIndex = -1;
			for (int j = 0, i; j < tabSize; j++)
			{
				i = tabOrder ? tabOrder[j] : j;
				if (i == -1)
				{
					continue;
				}
				tabIndex = i;
				break;
			}
		}

		// Restore the style
		style.Colors[ImGuiCol_Button] = color;
		style.Colors[ImGuiCol_ButtonActive] = colorActive;
		style.Colors[ImGuiCol_ButtonHovered] = colorHover;
		style.Colors[ImGuiCol_Text] = colorText;
		style.ItemSpacing = itemSpacing;

		return selection_changed;
	}
}
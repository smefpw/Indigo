/*
	WARNING: This cheat (like pretty much any other pastes out there) uses Virtual Method Table (VMT) hooking, which is now detected by Valve Anti-Cheat.
	We are NOT responsible for ANY bans that may occur during the process of using this cheat. This includes, but not limited to, VAC, Untrusted and
	Overwatch bans.
*/

#include "Misc.h"

using namespace Client;
//[junk_enable /]
//[enc_string_enable /]

bool miscor; //misc on render
void CMisc::OnRender() {
	if (Settings::Misc::misc_Punch) {
		CBaseEntity* lp = (CBaseEntity*)Interfaces::EntityList()->GetClientEntity(Interfaces::Engine()->GetLocalPlayer());
		try {
			if (Interfaces::Engine()->IsInGame()) {
				//check to see if player has a weapon so no crash XD
				CBaseWeapon* pWeaponEntity = lp->GetBaseWeapon();
				if (lp && pWeaponEntity && !lp->IsDead() && !lp->IsDormant() && Interfaces::Engine()->IsConnected()){
					Vector ViewAngles;
					Interfaces::Engine()->GetViewAngles(ViewAngles);
					ViewAngles += (lp->GetAimPunchAngle()) * 2.f;
					Vector fowardVec;
					AngleVectors(ViewAngles, fowardVec);
					fowardVec *= 10000;
					Vector start = lp->GetEyePosition();
					Vector end = start + fowardVec, endScreen;

					//potential crash
					if (WorldToScreen(end, endScreen) && IsLocalAlive()) {
						g_pRender->DrawFillBox(endScreen.x - 1, endScreen.y - 1, 3, 3, Color::Green());
					}
				}
			}
		}
		catch (...) {
#if ENABLE_DEBUG_FILE == 1
			if (!miscor) {
				CSX::Log::Add("[CMisc:OnRender - error!]");
				miscor = 1;
			}
#endif
		}
	}
} 

void CMisc::OnCreateMove( CUserCmd* pCmd )
{
	CMe* local = Client::g_pPlayers->GetLocal();

	if (Settings::Misc::misc_Bhop)
	{
		if (local->m_pEntity->GetMoveType() == MOVETYPE_LADDER)
			return;

		static bool bLastJumped = false;
		static bool bShouldFake = false;
	
		if (!bLastJumped && bShouldFake) 
		{
			bShouldFake = false;
			pCmd->buttons |= IN_JUMP;
		}
		else if (pCmd->buttons & IN_JUMP) 
		{
			if (local->m_pEntity->GetFlags() & FL_ONGROUND) 
			{
				bLastJumped = true;
				bShouldFake = true;
			}
			else 
			{
				pCmd->buttons &= ~IN_JUMP;
				bLastJumped = false;
			}
		}
		else 
		{
			bLastJumped = false;
			bShouldFake = false;
		}
	}

	if (Settings::Misc::misc_spamregular)
		ChatSpamRegular();

	if (Settings::Misc::misc_spamrandom)
		ChatSpamRandom();

	if (Settings::Misc::misc_Clan > 0)
		ClanTag();

	ConVar* skybox = Interfaces::GetConVar()->FindVar("sv_skyname");

	if (Settings::Misc::misc_SkyName && Settings::Untrusted) { if (skybox) skybox->SetValue(Settings::Misc::misc_SkyName); }
	if (Settings::Misc::misc_NoSky && Settings::Untrusted) { if (skybox) skybox->SetValue("sky_l4d_rural02_ldr"); }


	ConVar* PostProcess = Interfaces::GetConVar()->FindVar("mat_postprocess_enable");
	*(int*)((DWORD)&PostProcess->fnChangeCallback + 0xC) = 0;
	PostProcess->SetValue(!Settings::Misc::misc_Postprocess);

	if (Settings::Misc::misc_namespamidkmemes)
	{
		static bool steal = false;

		ConVar* Name = Interfaces::GetConVar()->FindVar("name");
		*(int*)((DWORD)&Name->fnChangeCallback + 0xC) = 0;

		if (Name)
		{
			if (!steal)
			{
				Name->SetValue(Settings::Misc::First);
				steal = true;
			}
			else
			{
				Name->SetValue(Settings::Misc::Second);
				steal = false;
			}
		}
	}

	if (Settings::Misc::misc_namespamidkmemes_static)
	{
		static bool steal = false;

		ConVar* Name = Interfaces::GetConVar()->FindVar("name");
		*(int*)((DWORD)&Name->fnChangeCallback + 0xC) = 0;

		if (Name)
		{
			if (!steal)
			{
				Name->SetValue(Settings::Misc::First_static);
				steal = true;
			}
			else
			{
				Name->SetValue(Settings::Misc::Second_static);
				steal = false;
			}
		}
	}
}

//broken
void CMisc::FrameStageNotify(ClientFrameStage_t Stage)
{
	/*if (Interfaces::Engine()->IsInGame() && Stage == ClientFrameStage_t::FRAME_RENDER_START)
	{
		CBaseEntity* localplayer = (CBaseEntity*)Interfaces::EntityList()->GetClientEntity(Interfaces::Engine()->GetLocalPlayer());
		if (!localplayer)
			return;

		static QAngle vecAngles;
		Interfaces::Engine()->GetViewAngles(vecAngles);
		if (Settings::Misc::misc_ThirdPerson && !localplayer->IsDead() && Settings::Untrusted)
		{
			if (!Interfaces::Input()->m_fCameraInThirdPerson)
				Interfaces::Input()->m_fCameraInThirdPerson = true;

			Interfaces::Input()->m_vecCameraOffset = QAngle(vecAngles.x, vecAngles.y, Settings::Misc::misc_ThirdPersonRange);

			*localplayer->GetVAngles() = Settings::Misc::qLastTickAngle;
		}
		else
		{
			if (Interfaces::Input()->m_fCameraInThirdPerson || localplayer->GetIsScoped())
			{
				Interfaces::Input()->m_fCameraInThirdPerson = false;
				Interfaces::Input()->m_vecCameraOffset = QAngle(vecAngles.x, vecAngles.y, 0);
			}
		}
	}*/
}

std::vector<const char*> smoke_materials = {
	"particle/vistasmokev1/vistasmokev1_smokegrenade",
	"particle/vistasmokev1/vistasmokev1_emods",
	"particle/vistasmokev1/vistasmokev1_emods_impactdust",
	"particle/vistasmokev1/vistasmokev1_fire",
};

void CMisc::OnDrawModelExecute()
{
	static bool NoSmoke = false;
	static bool NoFlashReset = false;
	IMaterial* flash = Interfaces::MaterialSystem()->FindMaterial("effects\\flashbang", TEXTURE_GROUP_CLIENT_EFFECTS);
	IMaterial* flashWhite = Interfaces::MaterialSystem()->FindMaterial("effects\\flashbang_white", TEXTURE_GROUP_CLIENT_EFFECTS);

	if (flash && flashWhite)
	{
		if (Settings::Misc::misc_NoFlash && !NoFlashReset)
		{
			flash->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
			flashWhite->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);

			NoFlashReset = true;
		}
		else if (!Settings::Misc::misc_NoFlash && NoFlashReset)
		{
			flash->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, false);
			flashWhite->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, false);

			NoFlashReset = false;
		}
	}

	if (Settings::Misc::misc_NoSmoke && !NoSmoke)
	{
		for (auto mat : smoke_materials)
			Interfaces::MaterialSystem()->FindMaterial(mat, TEXTURE_GROUP_CLIENT_EFFECTS)->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);

		NoSmoke = true;
	}
	else if (!Settings::Misc::misc_NoSmoke && NoSmoke)
	{
		for (auto mat : smoke_materials)
			Interfaces::MaterialSystem()->FindMaterial(mat, TEXTURE_GROUP_CLIENT_EFFECTS)->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, false);

		NoSmoke = false;
	}
}

//11th March 2020 - 19th October 2020
void CMisc::OnPlaySound(const char* pszSoundName) {
	if (Settings::Misc::misc_AutoAccept && !strcmp(pszSoundName, "UIPanorama.popup_accept_match_beep")) {
		typedef bool(__stdcall* IsReadyCallBack_t)(const char*);
		static auto IsReadyCallBackFn = (IsReadyCallBack_t)CSX::Memory::FindPatternV2(CLIENT_DLL, "55 8B EC 83 E4 F8 8B 4D 08 BA ? ? ? ? E8 ? ? ? ? 85 C0 75 12");
#if ENABLE_DEBUG_FILE == 1
		CSX::Log::Add("[FindPattern - IsReadyCallBack = %X]", IsReadyCallBackFn);
#endif
		if (IsReadyCallBackFn) {
			IsReadyCallBackFn("deferred");
		}
	}
}

void CMisc::OnOverrideView( CViewSetup * pSetup )
{
	if ( Settings::Misc::misc_FovChanger && !Interfaces::Engine()->IsTakingScreenshot() )
	{
		CBaseEntity* pPlayer = (CBaseEntity*)Interfaces::EntityList()->GetClientEntity( Interfaces::Engine()->GetLocalPlayer() );

		if (!pPlayer)
			return;

		if (pPlayer->GetIsScoped())
			return;

		if ( pPlayer->IsDead() )
		{
			if ( pPlayer->GetObserverMode() == ObserverMode_t::OBS_MODE_IN_EYE && pPlayer->GetObserverTarget() )
				pPlayer = (CBaseEntity*)Interfaces::EntityList()->GetClientEntityFromHandle( pPlayer->GetObserverTarget() );

			if ( !pPlayer )
				return;
		}

		pSetup->fov = (float)Settings::Misc::misc_FovView;
	}
}

void CMisc::OnGetViewModelFOV(float& fov)
{
	if (Settings::Misc::misc_FovChanger && !Interfaces::Engine()->IsTakingScreenshot())
	{
		CBaseEntity* pPlayer = (CBaseEntity*)Interfaces::EntityList()->GetClientEntity(Interfaces::Engine()->GetLocalPlayer());

		if (!pPlayer)
			return;

		if (pPlayer->IsDead())
		{
			if (pPlayer->GetObserverMode() == ObserverMode_t::OBS_MODE_IN_EYE && pPlayer->GetObserverTarget())
				pPlayer = (CBaseEntity*)Interfaces::EntityList()->GetClientEntityFromHandle(pPlayer->GetObserverTarget());

			if (!pPlayer)
				return;
		}

		fov = (float)Settings::Misc::misc_FovModelView;
	}
}

vector<int> CMisc::GetObservervators( int playerId )
{
	vector<int> SpectatorList;

	CBaseEntity* pPlayer = (CBaseEntity*)Interfaces::EntityList()->GetClientEntity( playerId );

	if ( !pPlayer )
		return SpectatorList;

	if ( pPlayer->IsDead() )
	{
		CBaseEntity* pObserverTarget = (CBaseEntity*)Interfaces::EntityList()->GetClientEntityFromHandle( pPlayer->GetObserverTarget() );

		if ( !pObserverTarget )
			return SpectatorList;

		pPlayer = pObserverTarget;
	}

	for ( int PlayerIndex = 0; PlayerIndex < g_pPlayers->GetSize(); PlayerIndex++ )
	{
		CBaseEntity* pCheckPlayer = (CBaseEntity*)Interfaces::EntityList()->GetClientEntity( PlayerIndex );

		if ( !pCheckPlayer )
			continue;

		if ( pCheckPlayer->IsDormant() || !pCheckPlayer->IsDead() )
			continue;

		CBaseEntity* pObserverTarget = (CBaseEntity*)Interfaces::EntityList()->GetClientEntityFromHandle( pCheckPlayer->GetObserverTarget() );

		if ( !pObserverTarget )
			continue;

		if ( pPlayer != pObserverTarget )
			continue;

		SpectatorList.push_back( PlayerIndex );
	}

	return SpectatorList;
}

void CMisc::FakeLag(bool &bSendPacket)
{
	int amount = Settings::Misc::misc_fakelag_amount;

	static int choke = -1;
	choke++;

	if (choke <= amount && choke > -1)
	{
		bSendPacket = false;
	}
	else
	{
		bSendPacket = true;
		choke = -1;
	}
}

void CMisc::OnRenderSpectatorList()
{
	if (Settings::Misc::misc_Spectators)
	{
		int DrawIndex = 1;

		for (int playerId : GetObservervators(Interfaces::Engine()->GetLocalPlayer()))
		{
			if (playerId == Interfaces::Engine()->GetLocalPlayer())
				continue;

			CBaseEntity* pPlayer = (CBaseEntity*)Interfaces::EntityList()->GetClientEntity(playerId);

			if (!pPlayer)
				continue;

			PlayerInfo Pinfo{};
			Interfaces::Engine()->GetPlayerInfo(playerId, &Pinfo);

			if (Pinfo.m_bIsFakePlayer)
				continue;

			if (g_pRender)
			{
				string Name = Pinfo.m_szPlayerName;

				if (Name != "")
				{
					Color PlayerObsColor;
					//[junk_disable /]
					switch (pPlayer->GetObserverMode())
					{
					case ObserverMode_t::OBS_MODE_IN_EYE:
						Name.append(" ");
						PlayerObsColor = Color::White();
						break;
					default:
						break;
					}
					//[junk_enable /]
					g_pRender->Text(15, 500 + (DrawIndex * 13), false, true, PlayerObsColor, "%s", Name.c_str());
					DrawIndex++;
				}
			}
		}
	}
}

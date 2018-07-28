/*
	WARNING: This cheat (like pretty much any other pastes out there) uses Virtual Method Table (VMT) hooking, which is now detected by Valve Anti-Cheat.
	We are NOT responsible for ANY bans that may occur during the process of using this cheat. This includes, but not limited to, VAC, Untrusted and
	Overwatch bans.
*/

#include "ESP.h"

using namespace Client;
SDK::PlayerInfo GetInfo(int Index)
{
	SDK::PlayerInfo info;
	Interfaces::Engine()->GetPlayerInfo(Index, &info);
	return info;
}

char* HitgroupToName(int hitgroup)
{
	switch (hitgroup)
	{
	case HITGROUP_HEAD:
		return "head";
	case HITGROUP_LEFTLEG:
		return "leg";
	case HITGROUP_RIGHTLEG:
		return "leg";
	case HITGROUP_LEFTARM:
		return "arm";
	case HITGROUP_RIGHTARM:
		return "arm";
	case HITGROUP_STOMACH:
		return "chest";
	default:
		return "chest";
	}
}

CSoundEsp::CSoundEsp()
{
	SoundColor = Color::White();
}
bool done = false;
void CSoundEsp::Update()
{
	for ( size_t i = 0; i < Sound.size(); i++ )
		if ( Sound[i].dwTime + 800 <= GetTickCount64() )
			Sound.erase( Sound.begin() + i );
}

void CSoundEsp::AddSound( Vector vOrigin )
{
	Sound_s Sound_Entry;

	Sound_Entry.dwTime = GetTickCount64();
	Sound_Entry.vOrigin = vOrigin;

	Sound.push_back( Sound_Entry );
}

void CSoundEsp::DrawSoundEsp()
{
	for ( size_t i = 0; i < Sound.size(); i++ )
	{
		Vector vScreen;

		if ( WorldToScreen( Sound[i].vOrigin , vScreen ) )
			if ( Settings::Esp::esp_Sound )
				g_pRender->DrawBox( (int)vScreen.x , (int)vScreen.y , 10 , 10 , SoundColor );
	}
}

void CEsp::Dlight(CPlayer* pPlayer)
{
	Vector getorig = pPlayer->m_pEntity->GetOrigin();
	Vector getheadorig = pPlayer->m_pEntity->GetEyePosition();
	Color EspVisibleColor = GetPlayerVisibleColor(pPlayer);

	dlight_t* pElight = Interfaces::Effects()->CL_AllocElight(pPlayer->m_pEntity->EntIndex());
	pElight->color.r = float(Settings::Esp::esp_Dlight[0] * 255.f);
	pElight->color.g = float(Settings::Esp::esp_Dlight[1] * 255.f);;
	pElight->color.b = float(Settings::Esp::esp_Dlight[2] * 255.f);;
	pElight->color.exponent = 8.f;
	pElight->m_Direction = getheadorig;
	pElight->origin = getheadorig;
	pElight->radius = 200.0f;
	pElight->die = Interfaces::GlobalVars()->curtime + 0.1f;
	pElight->decay = 25.0f;
	pElight->key = pPlayer->m_pEntity->EntIndex();

	dlight_t* pDlight = Interfaces::Effects()->CL_AllocDlight(pPlayer->m_pEntity->EntIndex());
	pDlight->color.r = float(Settings::Esp::esp_Dlight[0] * 255.f);;
	pDlight->color.g = float(Settings::Esp::esp_Dlight[1] * 255.f);;
	pDlight->color.b = float(Settings::Esp::esp_Dlight[2] * 255.f);;
	pDlight->color.exponent = 8.f;
	pDlight->m_Direction = getorig;
	pDlight->origin = getorig;
	pDlight->radius = 100.f;
	pDlight->die = Interfaces::GlobalVars()->curtime + 0.1f;
	pDlight->decay = pDlight->radius / 5.f;
	pDlight->key = pPlayer->m_pEntity->EntIndex();
}

void CEsp::NightMode()
{
	static auto sv_skyname = Interfaces::GetConVar()->FindVar("sv_skyname");
	static auto sv_skyname_backup = Interfaces::GetConVar()->FindVar("sv_skyname")->GetString();
	static auto r_DrawSpecificStaticProp = Interfaces::GetConVar()->FindVar("r_DrawSpecificStaticProp");
	static auto r_DrawSpecificStaticProp_backup = Interfaces::GetConVar()->FindVar("r_DrawSpecificStaticProp")->GetInt();


	if (Settings::Esp::esp_NightMode)
	{
		if (!done)
		{
			r_DrawSpecificStaticProp->SetValue(1);
			sv_skyname->SetValue("sky_csgo_night02");

			for (MaterialHandle_t i = Interfaces::MaterialSystem()->FirstMaterial(); i != Interfaces::MaterialSystem()->InvalidMaterial(); i = Interfaces::MaterialSystem()->NextMaterial(i))
			{
				IMaterial *pMaterial = Interfaces::MaterialSystem()->GetMaterial(i);

				if (!pMaterial)
					continue;

				const char* group = pMaterial->GetTextureGroupName();
				const char* name = pMaterial->GetName();

				if (strstr(group, "World textures"))
					pMaterial->ColorModulate(0.10, 0.10, 0.10);

				if (strstr(group, "StaticProp"))
					pMaterial->ColorModulate(0.30, 0.30, 0.30);

				if (strstr(name, "models/props/de_dust/palace_bigdome"))
					pMaterial->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);

				if (strstr(name, "models/props/de_dust/palace_pillars"))
					pMaterial->ColorModulate(0.30, 0.30, 0.30);

				if (strstr(group, "Particle textures"))
					pMaterial->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);

				done = true;
			}
		}
	}
	else
	{
		if (done)
		{
			r_DrawSpecificStaticProp->SetValue(r_DrawSpecificStaticProp_backup);
			sv_skyname->SetValue(sv_skyname_backup);
			for (MaterialHandle_t i = Interfaces::MaterialSystem()->FirstMaterial(); i != Interfaces::MaterialSystem()->InvalidMaterial(); i = Interfaces::MaterialSystem()->NextMaterial(i))
			{
				IMaterial *pMaterial = Interfaces::MaterialSystem()->GetMaterial(i);

				if (!pMaterial)
					continue;

				const char* group = pMaterial->GetTextureGroupName();
				const char* name = pMaterial->GetName();

				if (strstr(group, "World textures"))
					pMaterial->ColorModulate(1, 1, 1);

				if (strstr(group, "StaticProp"))
					pMaterial->ColorModulate(1, 1, 1);

				if (strstr(name, "models/props/de_dust/palace_bigdome"))
					pMaterial->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, false);

				if (strstr(name, "models/props/de_dust/palace_pillars"))
					pMaterial->ColorModulate(1, 1, 1);

				if (strstr(group, "Particle textures"))
					pMaterial->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, false);
			}
			done = false;
		}
	}
}

CEsp::CEsp()
{
	// Öâåòà õï áàðà

	CT_HP_ColorM.SetColor( 255 , 64 , 64 );
	TT_HP_ColorM.SetColor( 255 , 64 , 64 );

	// Öâåòà àðìîð áàðà

	CT_AR_ColorM.SetColor( 255 , 64 , 64 );
	TT_AR_ColorM.SetColor( 255 , 64 , 64 );

	visible_flat = nullptr;
	visible_tex = nullptr;
	hidden_flat = nullptr;
	hidden_tex = nullptr;

	fExplodeC4Timer = 0.f;
	fC4Timer = 0.f;
}

Color CEsp::GetPlayerColor( CPlayer* pPlayer )
{
	Color CT_Color( int( Settings::Esp::esp_Color_CT[0] * 255.f ) , int( Settings::Esp::esp_Color_CT[1] * 255.f ) , int( Settings::Esp::esp_Color_CT[2] * 255.f ) );
	Color TT_Color( int( Settings::Esp::esp_Color_TT[0] * 255.f ) , int( Settings::Esp::esp_Color_TT[1] * 255.f ) , int( Settings::Esp::esp_Color_TT[2] * 255.f ) );

	if ( pPlayer->Team == TEAM_CT )
		return CT_Color;
	else if ( pPlayer->Team == TEAM_TT )
		return TT_Color;

	return Color::White();
}

Color CEsp::GetPlayerVisibleColor( CPlayer* pPlayer )
{
	Color PlayerColor = GetPlayerColor( pPlayer );

	bool SetColor = false;

	// Ïîäñâå÷èâàòü çåë¸íûì åñëè èãðîê âèäåí:

	if ( pPlayer->bVisible )
	{
		if ( Settings::Esp::esp_Visible == 0 && pPlayer->Team != g_pPlayers->GetLocal()->Team ) // Ïðîòèâíèêîâ
			SetColor = true;
		else if ( Settings::Esp::esp_Visible == 1 && pPlayer->Team == g_pPlayers->GetLocal()->Team ) // Ñâîèõ
			SetColor = true;
		else if ( Settings::Esp::esp_Visible == 2 ) // Âñåõ
			SetColor = true;

		Color VCT_Color( int( Settings::Esp::esp_Color_VCT[0] * 255.f ) , int( Settings::Esp::esp_Color_VCT[1] * 255.f ) , int( Settings::Esp::esp_Color_VCT[2] * 255.f ) );
		Color VTT_Color( int( Settings::Esp::esp_Color_VTT[0] * 255.f ) , int( Settings::Esp::esp_Color_VTT[1] * 255.f ) , int( Settings::Esp::esp_Color_VTT[2] * 255.f ) );

		if ( SetColor )
		{
			if ( pPlayer->Team == TEAM_CT )
				PlayerColor = VCT_Color;
			else if ( pPlayer->Team == TEAM_TT )
				PlayerColor = VTT_Color;
		}
	}

	return PlayerColor;
}

bool CEsp::CheckPlayerTeam( CPlayer* pPlayer )
{
	bool CheckTeam = false;
	bool PlayerVisible = pPlayer->bVisible;

	// Ïîêàçûâàåì esp òîëüêî íà:
	if ( Settings::Esp::esp_Enemy && pPlayer->Team != g_pPlayers->GetLocal()->Team ) // Ïðîòèâíèêîâ
		CheckTeam = true;

	if ( Settings::Esp::esp_Team && pPlayer->Team == g_pPlayers->GetLocal()->Team ) // Ñâîèõ
		CheckTeam = true;

	if ( Settings::Esp::esp_Visible >= 3 && !PlayerVisible )
		CheckTeam = false;

	return CheckTeam;
}

void CEsp::Ambient()
{
	ConVar* AmbientRedCvar = Interfaces::GetConVar()->FindVar("mat_ambient_light_r");
	*(float*)((DWORD)&AmbientRedCvar->fnChangeCallback + 0xC) = NULL;
	AmbientRedCvar->SetValue(Settings::Esp::esp_Ambient[0]);

	ConVar* AmbientGreenCvar = Interfaces::GetConVar()->FindVar("mat_ambient_light_g");
	*(float*)((DWORD)&AmbientGreenCvar->fnChangeCallback + 0xC) = NULL;
	AmbientGreenCvar->SetValue(Settings::Esp::esp_Ambient[1]);

	ConVar* AmbientBlueCvar = Interfaces::GetConVar()->FindVar("mat_ambient_light_b");
	*(float*)((DWORD)&AmbientBlueCvar->fnChangeCallback + 0xC) = NULL;
	AmbientBlueCvar->SetValue(Settings::Esp::esp_Ambient[2]);
}

void CEsp::HitEvents(IGameEvent* event)
{
	if (!Settings::Esp::esp_hitevent)
		return;

	if (!strcmp(event->GetName(), "player_hurt")) 
	{
		int attackerid = event->GetInt("attacker");
		int entityid = Interfaces::Engine()->GetPlayerForUserID(attackerid);
		if (entityid == Interfaces::Engine()->GetLocalPlayer())
		{
			int nUserID = event->GetInt("attacker");
			int nDead = event->GetInt("userid");
			if (nUserID || nDead)
			{
				SDK::PlayerInfo killed_info = GetInfo(Interfaces::Engine()->GetPlayerForUserID(nDead));
				EventLog->AddToLog("Damaged %s in the %s for %i damage (%i health left)", // much skeet pasta yes?
					killed_info.m_szPlayerName, 
					HitgroupToName(event->GetInt("hitgroup")), 
					event->GetInt("dmg_health"), 
					event->GetInt("health"));
			}
		}
	}
}

void CEsp::HitmarkerEvents(IGameEvent* event)
{
	if (!Settings::Esp::esp_HitMarker)
		return;

	if (!strcmp(event->GetName(), "player_hurt")) {
		int attacker = event->GetInt("attacker");
		if (Interfaces::Engine()->GetPlayerForUserID(attacker) == Interfaces::Engine()->GetLocalPlayer()) {
			switch (Settings::Esp::esp_HitMarkerSound)
			{
				case 0: break; // do nothing
				case 1: PlaySoundA(rawData, NULL, SND_ASYNC | SND_MEMORY); break; // default / cod 
				case 2: Interfaces::Engine()->ClientCmd_Unrestricted2("play buttons\\arena_switch_press_02.wav"); // metallic
				case 3: PlaySoundA(bameware, NULL, SND_ASYNC | SND_MEMORY); break; // bameware
				case 4: PlaySoundA(penguware, NULL, SND_ASYNC | SND_MEMORY); break; // penguware
			}

			Settings::hitmarkerAlpha = 1.f;
		}
	}
}

void CEsp::DrawHitmarker()
{
	if (Settings::hitmarkerAlpha < 0.f)
		Settings::hitmarkerAlpha = 0.f;
	else if (Settings::hitmarkerAlpha > 0.f)
		Settings::hitmarkerAlpha -= 0.01f;

	int W, H;
	Interfaces::Engine()->GetScreenSize(W, H);

	float r = Settings::Esp::esp_HitMarkerColor[0] * 255.f;
	float g = Settings::Esp::esp_HitMarkerColor[1] * 255.f;
	float b = Settings::Esp::esp_HitMarkerColor[2] * 255.f;

	if (Settings::hitmarkerAlpha > 0.f)
	{
		g_pRender->DrawLine(W / 2 - 10, H / 2 - 10, W / 2 - 5, H / 2 - 5, Color(r, g, b, (Settings::hitmarkerAlpha * 255.f)));
		g_pRender->DrawLine(W / 2 - 10, H / 2 + 10, W / 2 - 5, H / 2 + 5, Color(r, g, b, (Settings::hitmarkerAlpha * 255.f)));
		g_pRender->DrawLine(W / 2 + 10, H / 2 - 10, W / 2 + 5, H / 2 - 5, Color(r, g, b, (Settings::hitmarkerAlpha * 255.f)));
		g_pRender->DrawLine(W / 2 + 10, H / 2 + 10, W / 2 + 5, H / 2 + 5, Color(r, g, b, (Settings::hitmarkerAlpha * 255.f)));
	}
}

void hitmarker::player_hurt_listener::start()
{
	if (!Interfaces::GameEvent()->AddListener(this, "player_hurt", false))
		throw exception("Failed to register the event");
}
void hitmarker::player_hurt_listener::stop()
{
	Interfaces::GameEvent()->RemoveListener(this);
}
void hitmarker::player_hurt_listener::FireGameEvent(IGameEvent *event)
{
	g_pEsp->HitmarkerEvents(event);
	g_pEsp->HitEvents(event);
}
int hitmarker::player_hurt_listener::GetEventDebugID(void)
{
	return 0x2A;
}

void CEsp::GrenadePrediction()
{
	if (Interfaces::Engine()->IsInGame() && Interfaces::Engine()->IsConnected())
	{
		CBaseEntity* pLocal = (CBaseEntity*)Interfaces::EntityList()->GetClientEntity(Interfaces::Engine()->GetLocalPlayer());

		const float TIMEALIVE = 5.f;
		const float GRENADE_COEFFICIENT_OF_RESTITUTION = 0.4f;

		float fStep = 0.1f;
		float fGravity = 800.0f / 8.f;

		Vector vPos, vThrow, vThrow2;
		Vector vStart;

		int iCollisions = 0;

		QAngle vViewAngles;
		Interfaces::Engine()->GetViewAngles(vViewAngles);

		vThrow[0] = vViewAngles[0];
		vThrow[1] = vViewAngles[1];
		vThrow[2] = vViewAngles[2];

		if (vThrow[0] < 0)
			vThrow[0] = -10 + vThrow[0] * ((90 - 10) / 90.0);
		else
			vThrow[0] = -10 + vThrow[0] * ((90 + 10) / 90.0);

		float fVel = (90 - vThrow[0]) * 4;
		if (fVel > 500)
			fVel = 500;

		AngleVectors(vThrow, vThrow2);

		Vector vEye = pLocal->GetEyePosition();
		vStart[0] = vEye[0] + vThrow2[0] * 16;
		vStart[1] = vEye[1] + vThrow2[1] * 16;
		vStart[2] = vEye[2] + vThrow2[2] * 16;

		vThrow2[0] = (vThrow2[0] * fVel) + pLocal->GetVelocity()[0];
		vThrow2[1] = (vThrow2[1] * fVel) + pLocal->GetVelocity()[1];
		vThrow2[2] = (vThrow2[2] * fVel) + pLocal->GetVelocity()[2];

#define IsGrenade g_pPlayers->GetLocal()->WeaponIndex == WEAPON_SMOKEGRENADE | g_pPlayers->GetLocal()->WeaponIndex == WEAPON_HEGRENADE | g_pPlayers->GetLocal()->WeaponIndex == WEAPON_INCGRENADE | g_pPlayers->GetLocal()->WeaponIndex == WEAPON_MOLOTOV | g_pPlayers->GetLocal()->WeaponIndex == WEAPON_DECOY | g_pPlayers->GetLocal()->WeaponIndex == WEAPON_FLASHBANG

		if (!pLocal->IsDead() && IsGrenade)
		{
			for (float fTime = 0.0f; fTime < TIMEALIVE; fTime += fStep)
			{
				vPos = vStart + vThrow2 * fStep;

				Ray_t ray;
				trace_t tr;
				CTraceFilter loc;
				loc.pSkip = pLocal;

				ray.Init(vStart, vPos);
				Interfaces::EngineTrace()->TraceRay(ray, MASK_SOLID, &loc, &tr);
				Color color = Color(0, 255, 0);
				if (tr.DidHit())
				{
					float anglez = DotProduct(Vector(0, 0, 1), tr.plane.normal);
					float invanglez = DotProduct(Vector(0, 0, -1), tr.plane.normal);
					float angley = DotProduct(Vector(0, 1, 0), tr.plane.normal);
					float invangley = DotProduct(Vector(0, -1, 0), tr.plane.normal);
					float anglex = DotProduct(Vector(1, 0, 0), tr.plane.normal);
					float invanglex = DotProduct(Vector(3 - 1, 0, 0), tr.plane.normal);
					float scale = tr.endpos.DistTo(pLocal->GetOrigin()) / 60;
					Color color = Color(0, 0, 255);
					if (anglez > 0.5)
					{
						tr.endpos.z += 1;
						Vector startPos = tr.endpos + Vector(-scale, 0, 0);
						Vector endPos = tr.endpos + Vector(scale, 0, 0);
						Vector outStart, outEnd;
						if (WorldToScreen(startPos, outStart) && WorldToScreen(endPos, outEnd))
							g_pRender->DrawLine((int)outStart.x, (int)outStart.y, (int)outEnd.x, (int)outEnd.y, Color::Green());

						startPos = tr.endpos + Vector(0, -scale, 0);
						endPos = tr.endpos + Vector(0, scale, 0);
						if (WorldToScreen(startPos, outStart) && WorldToScreen(endPos, outEnd))
							g_pRender->DrawLine((int)outStart.x, (int)outStart.y, (int)outEnd.x, (int)outEnd.y, Color::Green());
					}
					else if (invanglez > 0.5)
					{
						tr.endpos.z += 1;
						Vector startPos = tr.endpos + Vector(-scale, 0, 0);
						Vector endPos = tr.endpos + Vector(scale, 0, 0);
						Vector outStart, outEnd;
						if (WorldToScreen(startPos, outStart) && WorldToScreen(endPos, outEnd))
							g_pRender->DrawLine((int)outStart.x, (int)outStart.y, (int)outEnd.x, (int)outEnd.y, Color::Green());

						startPos = tr.endpos + Vector(0, -scale, 0);
						endPos = tr.endpos + Vector(0, scale, 0);
						if (WorldToScreen(startPos, outStart) && WorldToScreen(endPos, outEnd))
							g_pRender->DrawLine((int)outStart.x, (int)outStart.y, (int)outEnd.x, (int)outEnd.y, Color::Green());
					}
					else if (angley > 0.5)
					{
						tr.endpos.y += 1;
						Vector startPos = tr.endpos + Vector(0, 0, -scale);
						Vector endPos = tr.endpos + Vector(0, 0, scale);
						Vector outStart, outEnd;
						if (WorldToScreen(startPos, outStart) && WorldToScreen(endPos, outEnd))
							g_pRender->DrawLine((int)outStart.x, (int)outStart.y, (int)outEnd.x, (int)outEnd.y, Color::Green());

						startPos = tr.endpos + Vector(-scale, 0, 0);
						endPos = tr.endpos + Vector(scale, 0, 0);
						if (WorldToScreen(startPos, outStart) && WorldToScreen(endPos, outEnd))
							g_pRender->DrawLine((int)outStart.x, (int)outStart.y, (int)outEnd.x, (int)outEnd.y, Color::Green());
					}
					else if (invangley > 0.5)
					{
						tr.endpos.y += 1;
						Vector startPos = tr.endpos + Vector(0, 0, -scale);
						Vector endPos = tr.endpos + Vector(0, 0, scale);
						Vector outStart, outEnd;
						if (WorldToScreen(startPos, outStart) && WorldToScreen(endPos, outEnd))
							g_pRender->DrawLine((int)outStart.x, (int)outStart.y, (int)outEnd.x, (int)outEnd.y, Color::Green());

						startPos = tr.endpos + Vector(-scale, 0, 0);
						endPos = tr.endpos + Vector(scale, 0, 0);
						if (WorldToScreen(startPos, outStart) && WorldToScreen(endPos, outEnd))
							g_pRender->DrawLine((int)outStart.x, (int)outStart.y, (int)outEnd.x, (int)outEnd.y, Color::Green());
					}
					else if (anglex > 0.5)
					{
						tr.endpos.x += 1;
						Vector startPos = tr.endpos + Vector(0, -scale, 0);
						Vector endPos = tr.endpos + Vector(0, scale, 0);
						Vector outStart, outEnd;
						if (WorldToScreen(startPos, outStart) && WorldToScreen(endPos, outEnd))
							g_pRender->DrawLine((int)outStart.x, (int)outStart.y, (int)outEnd.x, (int)outEnd.y, Color::Green());

						startPos = tr.endpos + Vector(0, 0, -scale);
						endPos = tr.endpos + Vector(0, 0, scale);
						if (WorldToScreen(startPos, outStart) && WorldToScreen(endPos, outEnd))
							g_pRender->DrawLine((int)outStart.x, (int)outStart.y, (int)outEnd.x, (int)outEnd.y, Color::Green());
					}
					else if (invanglex > 0.5)
					{
						tr.endpos.x += 1;
						Vector startPos = tr.endpos + Vector(0, -scale, 0);
						Vector endPos = tr.endpos + Vector(0, scale, 0);
						Vector outStart, outEnd;
						if (WorldToScreen(startPos, outStart) && WorldToScreen(endPos, outEnd))
							g_pRender->DrawLine((int)outStart.x, (int)outStart.y, (int)outEnd.x, (int)outEnd.y, Color::Green());

						startPos = tr.endpos + Vector(0, 0, -scale);
						endPos = tr.endpos + Vector(0, 0, scale);
						if (WorldToScreen(startPos, outStart) && WorldToScreen(endPos, outEnd))
							g_pRender->DrawLine((int)outStart.x, (int)outStart.y, (int)outEnd.x, (int)outEnd.y, Color::Green());
					}

					vThrow2 = tr.plane.normal * -2.0f * DotProduct(vThrow2, tr.plane.normal) + vThrow2;
					vThrow2 *= GRENADE_COEFFICIENT_OF_RESTITUTION;

					iCollisions++;
					if (iCollisions > 2)
						break;

					vPos = vStart + vThrow2 * tr.fraction * fStep;
					fTime += (fStep * (1 - tr.fraction));
				}

				Vector vOutStart, vOutEnd;

				if (WorldToScreen(vStart, vOutStart) && WorldToScreen(vPos, vOutEnd))
					g_pRender->DrawLine((int)vOutStart.x, (int)vOutStart.y, (int)vOutEnd.x, (int)vOutEnd.y, Color::Blue());

				vStart = vPos;
				vThrow2.z -= fGravity * tr.fraction * fStep;
			}
		}
	}
}

void CEsp::OnRender()
{
	if (!g_pPlayers || !Interfaces::Engine()->IsConnected()) // should help with dlight crashes??? idk
		return;

	if ( Settings::Esp::esp_Sound )
		SoundEsp.DrawSoundEsp();

	if ( g_pAimbot )
		g_pAimbot->OnRender();

	if ( g_pTriggerbot )
		g_pTriggerbot->TriggerShowStatus();

	if (Settings::Esp::esp_GrenadePrediction)
		GrenadePrediction();

	if (Settings::Esp::esp_HitMarker)
		DrawHitmarker();

	NightMode();

	Ambient();

	if (Settings::Misc::misc_AwpAim && IsLocalAlive())
		g_pRender->DrawFillBox(iScreenWidth, iScreenHeight, 1, 1, Color::Purple());

	for ( BYTE PlayerIndex = 0; PlayerIndex < g_pPlayers->GetSize(); PlayerIndex++ )
	{
		CPlayer* pPlayer = g_pPlayers->GetPlayer( PlayerIndex );

		if ( pPlayer && pPlayer->m_pEntity && pPlayer->bUpdate )
		{
			if (CheckPlayerTeam(pPlayer))
			{
				if (g_pTriggerbot)
					g_pTriggerbot->TriggerShow(pPlayer);

				DrawPlayerEsp(pPlayer);

				if (Settings::Esp::esp_Skeleton)
					DrawPlayerSkeleton(pPlayer);
			}
			if ( Settings::Esp::esp_BulletTrace && pPlayer->Team != g_pPlayers->GetLocal()->Team)
				DrawPlayerBulletTrace( pPlayer );

			if (Settings::Esp::esp_Dlightz && pPlayer->Team != g_pPlayers->GetLocal()->Team)
				Dlight(pPlayer);
		}
	}

	if ( Settings::Esp::esp_BombTimer )
	{
		if ( bC4Timer && iC4Timer )
		{
			float fTimeStamp = Interfaces::Engine()->GetLastTimeStamp();

			if ( !fExplodeC4Timer )
				fExplodeC4Timer = fTimeStamp + (float)iC4Timer;
			else
			{
				fC4Timer = fExplodeC4Timer - fTimeStamp;

				if ( fC4Timer < 0.f )
					fC4Timer = 0.f;
			}
		}
		else
		{
			fExplodeC4Timer = 0.f;
			fC4Timer = 0.f;
		}
	}

	if ( Settings::Esp::esp_Bomb || Settings::Esp::esp_WorldWeapons || Settings::Esp::esp_WorldGrenade || Settings::Esp::esp_Chicken || Settings::Esp::esp_BoxNade )
	{
		for ( int EntIndex = 0; EntIndex < Interfaces::EntityList()->GetHighestEntityIndex(); EntIndex++ )
		{
			CBaseEntity* pEntity = (CBaseEntity*)Interfaces::EntityList()->GetClientEntity( EntIndex );

			if ( !pEntity || pEntity->IsPlayer() )
				continue;

			const model_t* pModel = pEntity->GetModel();

			if ( pModel )
			{
				const char* pModelName = Interfaces::ModelInfo()->GetModelName( pModel );
				if ( pModelName )
				{
					Vector vEntScreen;

					if ( WorldToScreen( pEntity->GetRenderOrigin() , vEntScreen ) )
					{

						if (Settings::Esp::esp_Chicken && (pEntity->GetClientClass()->m_ClassID == (int)CLIENT_CLASS_ID::CChicken))
							g_pRender->Text((int)vEntScreen.x, (int)vEntScreen.y, true, true, Color::Green(), "Chicken");

						if ( Settings::Esp::esp_Bomb && pEntity->GetClientClass()->m_ClassID == (int)CLIENT_CLASS_ID::CC4 )
							g_pRender->Text( (int)vEntScreen.x , (int)vEntScreen.y , true , true , Color::Green() , "[C4]" );

						if (Settings::Esp::esp_Bomb && pEntity->GetClientClass()->m_ClassID == (int)CLIENT_CLASS_ID::CPlantedC4)
							g_pRender->Text((int)vEntScreen.x, (int)vEntScreen.y, true, true, Color::Red(), "[C4 PLANTED]");

						if (Settings::Esp::esp_WorldWeapons && !strstr(pModelName, "models/weapons/w_eq_") && !strstr(pModelName, "models/weapons/w_ied"))
						{
							if (strstr(pModelName, "models/weapons/w_") && strstr(pModelName, "_dropped.mdl"))
							{
								string WeaponName = pModelName + 17;

								WeaponName[WeaponName.size() - 12] = '\0';

								if (strstr(pModelName, "models/weapons/w_rif") && strstr(pModelName, "_dropped.mdl"))
									WeaponName.erase(0, 4);

								else if (strstr(pModelName, "models/weapons/w_pist") && strstr(pModelName, "_dropped.mdl") && !strstr(pModelName, "models/weapons/w_pist_223"))
									WeaponName.erase(0, 5);

								else if (strstr(pModelName, "models/weapons/w_pist_223") && strstr(pModelName, "_dropped.mdl"))
									WeaponName = "usp-s";

								else if (strstr(pModelName, "models/weapons/w_smg") && strstr(pModelName, "_dropped.mdl"))
									WeaponName.erase(0, 4);

								else if (strstr(pModelName, "models/weapons/w_mach") && strstr(pModelName, "_dropped.mdl"))
									WeaponName.erase(0, 5);

								else if (strstr(pModelName, "models/weapons/w_shot") && strstr(pModelName, "_dropped.mdl"))
									WeaponName.erase(0, 5);

								else if (strstr(pModelName, "models/weapons/w_snip") && strstr(pModelName, "_dropped.mdl"))
									WeaponName.erase(0, 5);

								g_pRender->Text((int)vEntScreen.x, (int)vEntScreen.y, true, true, Color::White(), WeaponName.c_str());
							}
						}

						if (Settings::Esp::esp_BoxNade && (strstr(pModelName, "models/weapons/w_eq_")) || strstr(pModelName, "models/Weapons/w_eq_"))
						{
							if (strstr(pModelName, "_dropped.mdl"))
							{
								if (strstr(pModelName, "fraggrenade"))
									if (Settings::Esp::esp_BoxNade)
										g_pRender->DrawOutlineBox((int)vEntScreen.x - 10, (int)vEntScreen.y - 10, 20, 20, Color::Red());

								else if (strstr(pModelName, "molotov"))
									if (Settings::Esp::esp_BoxNade)
										g_pRender->DrawOutlineBox((int)vEntScreen.x - 10, (int)vEntScreen.y - 10, 20, 20, Color::OrangeRed());

								else if (strstr(pModelName, "incendiarygrenade"))
									if (Settings::Esp::esp_BoxNade)
										g_pRender->DrawOutlineBox((int)vEntScreen.x - 10, (int)vEntScreen.y - 10, 20, 20, Color::OrangeRed());

								else if (strstr(pModelName, "flashbang"))
									if (Settings::Esp::esp_BoxNade)
										g_pRender->DrawOutlineBox((int)vEntScreen.x - 10, (int)vEntScreen.y - 10, 20, 20, Color::Yellow());
							}
							else if (strstr(pModelName, "smokegrenade_thrown.mdl"))
								if (Settings::Esp::esp_BoxNade)
									g_pRender->DrawOutlineBox((int)vEntScreen.x - 10, (int)vEntScreen.y - 10, 20, 20, Color::Gray());
						}
						
						if ( Settings::Esp::esp_WorldGrenade && ( strstr( pModelName , "models/weapons/w_eq_" ) || strstr( pModelName , "models/Weapons/w_eq_" ) ) )
						{
							if ( strstr( pModelName , "_dropped.mdl" ) )
							{
								string WeaponName = pModelName + 20;

								WeaponName[WeaponName.size() - 12] = '\0';

								Color GrenadeColor = Color::White();

								if ( strstr( pModelName , "fraggrenade" ) )
								{
									WeaponName = "Grenade";
									GrenadeColor = Color::Red();
								}
								else if (strstr(pModelName, "molotov"))
								{
									WeaponName = "Molotov";
									GrenadeColor = Color::OrangeRed();
								}
								else if (strstr(pModelName, "incendiarygrenade"))
								{
									WeaponName = "Incendiary";
									GrenadeColor = Color::OrangeRed();
								}
								else if ( strstr( pModelName , "flashbang" ) )
								{
									WeaponName = "Flashbang";
									GrenadeColor = Color::Yellow();
								}

								g_pRender->Text( (int)vEntScreen.x , (int)vEntScreen.y , true , true , GrenadeColor , WeaponName.c_str() );
							}
							else if ( strstr( pModelName , "smokegrenade_thrown.mdl" ) )
							{
								string WeaponName = "Smoke";
								g_pRender->Text( (int)vEntScreen.x , (int)vEntScreen.y , true , true , Color::Gray() , WeaponName.c_str() );
							}
						}
					}
				}
			}
		}
	}

	// Draw Ticks

	if (Settings::Aimbot::aim_Backtrack && Settings::Aimbot::aim_DrawBacktrack) // Use Esp Visible Combo to change from visible only and not visible.
	{
		for (int i = 0; i < Interfaces::EntityList()->GetHighestEntityIndex(); i++)
		{
			CBaseEntity* local = (CBaseEntity*)Interfaces::EntityList()->GetClientEntity(Interfaces::Engine()->GetLocalPlayer());
			CBaseEntity *entity = (CBaseEntity*)Interfaces::EntityList()->GetClientEntity(i);
			CPlayer* pPlayer = g_pPlayers->GetPlayer(i);
			PlayerInfo pinfo;
			if (entity == nullptr || entity == local || entity->IsDormant() || entity->GetTeam() == local->GetTeam())
				continue;
			if (Interfaces::Engine()->GetPlayerInfo(i, &pinfo) && !entity->IsDead())
			{
				if (Settings::Esp::esp_Visible >= 3)
				{
					if (!local->IsDead() && pPlayer->bVisible)
					{
						for (int t = 0; t < BacktrackTicks(); ++t)
						{
							Vector screenbacktrack[64][25];

							if (headPositions[i][t].simtime && headPositions[i][t].simtime + 1 > local->GetSimTime())
							{
								if (WorldToScreen(headPositions[i][t].hitboxPos, screenbacktrack[i][t]))
								{
									g_pRender->DrawLine(screenbacktrack[i][t].x - 3.5, screenbacktrack[i][t].y, screenbacktrack[i][t].x + 3.5, screenbacktrack[i][t].y, Color(255, 0, 0, 75));
									g_pRender->DrawLine(screenbacktrack[i][t].x, screenbacktrack[i][t].y - 3.5, screenbacktrack[i][t].x, screenbacktrack[i][t].y + 3.5, Color(255, 0, 0, 75));
								}
							}
						}
					}
					else
						memset(&headPositions[0][0], 0, sizeof(headPositions));
				}
				else
				{
					if (!local->IsDead())
					{
						for (int t = 0; t < BacktrackTicks(); ++t)
						{
							Vector screenbacktrack[64][25];

							if (headPositions[i][t].simtime && headPositions[i][t].simtime + 1 > local->GetSimTime())
							{
								if (WorldToScreen(headPositions[i][t].hitboxPos, screenbacktrack[i][t]))
								{
									g_pRender->DrawLine(screenbacktrack[i][t].x - 3.5, screenbacktrack[i][t].y, screenbacktrack[i][t].x + 3.5, screenbacktrack[i][t].y, Color(255, 0, 0, 75));
									g_pRender->DrawLine(screenbacktrack[i][t].x, screenbacktrack[i][t].y - 3.5, screenbacktrack[i][t].x, screenbacktrack[i][t].y + 3.5, Color(255, 0, 0, 75));
								}
							}
						}
					}
					else
						memset(&headPositions[0][0], 0, sizeof(headPositions));
				}
			}
		}
	}
}

void MsgFunc_ServerRankRevealAll()
{
	using tServerRankRevealAllFn = bool( __cdecl* )( int* );
	static tServerRankRevealAllFn ServerRankRevealAll = 0;

	if ( !ServerRankRevealAll )
	{
		ServerRankRevealAll = (tServerRankRevealAllFn)(
			CSX::Memory::FindPattern( CLIENT_DLL , "55 8B EC 8B 0D ? ? ? ? 68" , 0 ) );
	}

	if ( ServerRankRevealAll )
	{
		int fArray[3] = { 0,0,0 };
		ServerRankRevealAll( fArray );
	}
}

void CEsp::OnCreateMove(CUserCmd* pCmd)
{
	if (Settings::Esp::esp_Rank && pCmd->buttons & IN_SCORE)
		MsgFunc_ServerRankRevealAll();

	g_pEsp->SoundEsp.Update();
}

void CEsp::OnReset()
{
	g_pEsp->SoundEsp.Sound.clear();

	if ( Settings::Esp::esp_BombTimer )
	{
		if ( Settings::Esp::esp_BombTimer > 60 )
			Settings::Esp::esp_BombTimer = 60;

		bC4Timer = false;
		iC4Timer = Settings::Esp::esp_BombTimer;
	}
}

void CEsp::OnEvents( IGameEvent* pEvent )
{
	if ( g_pEsp && Settings::Esp::esp_BombTimer )
	{
		if ( !strcmp( pEvent->GetName() , "bomb_defused" ) || !strcmp( pEvent->GetName() , "bomb_exploded" ) )
			bC4Timer = false;
		else if ( !strcmp( pEvent->GetName() , "bomb_planted" ) )
			bC4Timer = true;
	}
}

void CEsp::OnDrawModelExecute( IMatRenderContext* ctx , const DrawModelState_t &state , const ModelRenderInfo_t &pInfo , matrix3x4_t *pCustomBoneToWorld )
{
	if ( !g_pPlayers || Interfaces::Engine()->IsTakingScreenshot() || !Interfaces::Engine()->IsConnected() || !pInfo.pModel )
		return;

	static bool InitalizeMaterial = false;

	if ( !InitalizeMaterial )
	{
		visible_flat = CreateMaterial( true , false );
		visible_tex = CreateMaterial( false , false );
		hidden_flat = CreateMaterial( true , true );
		hidden_tex = CreateMaterial( false , true );

		InitalizeMaterial = true;

		return;
	}

	string strModelName = Interfaces::ModelInfo()->GetModelName( pInfo.pModel );

	if ( strModelName.size() <= 1 )
		return;

	if (Settings::Misc::misc_ChamsMaterials)
	{
		if (strModelName.find("models/player") != std::string::npos)
		{
			IClientEntity* pBaseEntity = Interfaces::EntityList()->GetClientEntity(pInfo.entity_index);

			if (pBaseEntity && pBaseEntity->GetClientClass()->m_ClassID == (int)CLIENT_CLASS_ID::CCSPlayer)
			{
				IMaterial *material;
				switch (Settings::Misc::misc_ChamsMaterialsList)
				{
					case 0: material = Interfaces::MaterialSystem()->FindMaterial("models/inventory_items/cologne_prediction/cologne_prediction_glass", TEXTURE_GROUP_OTHER); break; // Glass
					case 1:	material = Interfaces::MaterialSystem()->FindMaterial("models/inventory_items/trophy_majors/crystal_clear", TEXTURE_GROUP_OTHER); break; // Crystal
					case 2:	material = Interfaces::MaterialSystem()->FindMaterial("models/inventory_items/trophy_majors/gold", TEXTURE_GROUP_OTHER); break; // Gold
					case 3:	material = Interfaces::MaterialSystem()->FindMaterial("models/gibs/glass/glass", TEXTURE_GROUP_OTHER); break; // Dark Chrome
					case 4: material = Interfaces::MaterialSystem()->FindMaterial("models/inventory_items/trophy_majors/gloss", TEXTURE_GROUP_OTHER); break; // Plastic Glass
					case 5:	material = Interfaces::MaterialSystem()->FindMaterial("models/inventory_items/trophy_majors/velvet", TEXTURE_GROUP_OTHER); break; // Velvet
					case 6: material = Interfaces::MaterialSystem()->FindMaterial("models/inventory_items/trophy_majors/crystal_blue", TEXTURE_GROUP_OTHER); break; // Crystal Blue
					case 7: material = Interfaces::MaterialSystem()->FindMaterial("models/inventory_items/wildfire_gold/wildfire_gold_detail", TEXTURE_GROUP_OTHER); break; // Detailed Gold
				}
				Color color = Color(255, 255, 255, 255);
				if (Settings::Esp::esp_ChamsVisible <= 2)
				{
					ForceMaterial(color, material);
					material->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);
				}
				else
				{
					ForceMaterial(color, material);
					material->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
				}
				Interfaces::ModelRender()->DrawModelExecute(ctx, state, pInfo, pCustomBoneToWorld);
			}
		}
	}

	if (Settings::Misc::misc_ArmMaterials)
	{
		switch (Settings::Misc::misc_ArmMaterialsType)
		{
			case 0: if (strModelName.find("arms") != std::string::npos)
			{
				IMaterial *material;
				switch (Settings::Misc::misc_ArmMaterialsList)
				{
					case 0: material = Interfaces::MaterialSystem()->FindMaterial("models/inventory_items/cologne_prediction/cologne_prediction_glass", TEXTURE_GROUP_OTHER); break; // Glass
					case 1:	material = Interfaces::MaterialSystem()->FindMaterial("models/inventory_items/trophy_majors/crystal_clear", TEXTURE_GROUP_OTHER); break; // Crystal
					case 2:	material = Interfaces::MaterialSystem()->FindMaterial("models/inventory_items/trophy_majors/gold", TEXTURE_GROUP_OTHER); break; // Gold
					case 3:	material = Interfaces::MaterialSystem()->FindMaterial("models/gibs/glass/glass", TEXTURE_GROUP_OTHER); break; // Dark Chrome
					case 4: material = Interfaces::MaterialSystem()->FindMaterial("models/inventory_items/trophy_majors/gloss", TEXTURE_GROUP_OTHER); break; // Plastic Glass
					case 5:	material = Interfaces::MaterialSystem()->FindMaterial("models/inventory_items/trophy_majors/velvet", TEXTURE_GROUP_OTHER); break; // Velvet
					case 6: material = Interfaces::MaterialSystem()->FindMaterial("models/inventory_items/trophy_majors/crystal_blue", TEXTURE_GROUP_OTHER); break; // Crystal Blue
					case 7: material = Interfaces::MaterialSystem()->FindMaterial("models/inventory_items/wildfire_gold/wildfire_gold_detail", TEXTURE_GROUP_OTHER); break; // Detailed Gold
				}
				Color color = Color(255, 255, 255, 255);
				ForceMaterial(color, material);
				Interfaces::ModelRender()->DrawModelExecute(ctx, state, pInfo, pCustomBoneToWorld);
			} break;
			case 1: if (strModelName.find("weapons/v") != std::string::npos)
			{
				IMaterial *material;
				switch (Settings::Misc::misc_ArmMaterialsList)
				{
					case 0: material = Interfaces::MaterialSystem()->FindMaterial("models/inventory_items/cologne_prediction/cologne_prediction_glass", TEXTURE_GROUP_OTHER); break; // Glass
					case 1:	material = Interfaces::MaterialSystem()->FindMaterial("models/inventory_items/trophy_majors/crystal_clear", TEXTURE_GROUP_OTHER); break; // Crystal
					case 2:	material = Interfaces::MaterialSystem()->FindMaterial("models/inventory_items/trophy_majors/gold", TEXTURE_GROUP_OTHER); break; // Gold
					case 3:	material = Interfaces::MaterialSystem()->FindMaterial("models/gibs/glass/glass", TEXTURE_GROUP_OTHER); break; // Dark Chrome
					case 4: material = Interfaces::MaterialSystem()->FindMaterial("models/inventory_items/trophy_majors/gloss", TEXTURE_GROUP_OTHER); break; // Plastic Glass
					case 5:	material = Interfaces::MaterialSystem()->FindMaterial("models/inventory_items/trophy_majors/velvet", TEXTURE_GROUP_OTHER); break; // Velvet
					case 6: material = Interfaces::MaterialSystem()->FindMaterial("models/inventory_items/trophy_majors/crystal_blue", TEXTURE_GROUP_OTHER); break; // Crystal Blue
					case 7: material = Interfaces::MaterialSystem()->FindMaterial("models/inventory_items/wildfire_gold/wildfire_gold_detail", TEXTURE_GROUP_OTHER); break; // Detailed Gold
				}
				Color color = Color(255, 255, 255, 255);
				ForceMaterial(color, material);
				Interfaces::ModelRender()->DrawModelExecute(ctx, state, pInfo, pCustomBoneToWorld);
			} break;
		}
	}

	if (Settings::Misc::misc_NoHands)
	{
		if (strModelName.find("arms") != string::npos && Settings::Misc::misc_NoHands)
		{
			IMaterial* Hands = Interfaces::MaterialSystem()->FindMaterial(strModelName.c_str(), TEXTURE_GROUP_MODEL);
			Hands->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
			Interfaces::ModelRender()->ForcedMaterialOverride(Hands);
		}
	}
	else if (strModelName.find("arms") != string::npos)
	{
		IMaterial* Hands = Interfaces::MaterialSystem()->FindMaterial(strModelName.c_str(), TEXTURE_GROUP_MODEL);
		Hands->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, false);
	}

	if (Settings::Misc::misc_WireHands) //Wireframe Hands
	{
		if (strModelName.find("arms") != string::npos)
		{
			IMaterial* WireHands = Interfaces::MaterialSystem()->FindMaterial(strModelName.c_str(), TEXTURE_GROUP_MODEL);
			WireHands->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, true);
			Interfaces::ModelRender()->ForcedMaterialOverride(WireHands);
		}
	}
	else if (strModelName.find("arms") != string::npos)
	{
		IMaterial* WireHands = Interfaces::MaterialSystem()->FindMaterial(strModelName.c_str(), TEXTURE_GROUP_MODEL);
		WireHands->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, false);
	}

	if ( Settings::Esp::esp_Chams && Client::g_pPlayers && Client::g_pPlayers->GetLocal() && strModelName.find( "models/player" ) != string::npos )
	{
		IClientEntity* pBaseEntity = Interfaces::EntityList()->GetClientEntity( pInfo.entity_index );

		if ( pBaseEntity && pBaseEntity->GetClientClass()->m_ClassID == (int)CLIENT_CLASS_ID::CCSPlayer )
		{
			CPlayer* pPlayer = g_pPlayers->GetPlayer( pInfo.entity_index );

			if ( pPlayer && pPlayer->bUpdate )
			{
				bool CheckTeam = false;

				Color TeamHideColor;
				Color TeamVisibleColor;

				if ( Settings::Esp::esp_Enemy && pPlayer->Team != g_pPlayers->GetLocal()->Team ) // Ïðîòèâíèêîâ
					CheckTeam = true;

				if ( Settings::Esp::esp_Team && pPlayer->Team == g_pPlayers->GetLocal()->Team ) // Ñâîèõ
					CheckTeam = true;

				if ( pPlayer->Team == TEAM_CT )
				{
					TeamHideColor = Color( int( Settings::Esp::chams_Color_CT[0] * 255.f ) ,
										   int( Settings::Esp::chams_Color_CT[1] * 255.f ) ,
										   int( Settings::Esp::chams_Color_CT[2] * 255.f ) );
				}
				else if ( pPlayer->Team == TEAM_TT )
				{
					TeamHideColor = Color( int( Settings::Esp::chams_Color_TT[0] * 255.f ) ,
										   int( Settings::Esp::chams_Color_TT[1] * 255.f ) ,
										   int( Settings::Esp::chams_Color_TT[2] * 255.f ) );
				}

				bool SetColor = false;

				if ( Settings::Esp::esp_ChamsVisible == 0 && pPlayer->Team != g_pPlayers->GetLocal()->Team ) // Ïðîòèâíèêîâ
					SetColor = true;
				else if ( Settings::Esp::esp_ChamsVisible == 1 && pPlayer->Team == g_pPlayers->GetLocal()->Team ) // Ñâîèõ
					SetColor = true;
				else if ( Settings::Esp::esp_ChamsVisible == 2 ) // Âñåõ
					SetColor = true;

				if ( SetColor )
				{
					if ( pPlayer->Team == TEAM_CT )
					{
						TeamVisibleColor = Color( int( Settings::Esp::chams_Color_VCT[0] * 255.f ) ,
												  int( Settings::Esp::chams_Color_VCT[1] * 255.f ) ,
												  int( Settings::Esp::chams_Color_VCT[2] * 255.f ) );
					}
					else if ( pPlayer->Team == TEAM_TT )
					{
						TeamVisibleColor = Color( int( Settings::Esp::chams_Color_VTT[0] * 255.f ) ,
												  int( Settings::Esp::chams_Color_VTT[1] * 255.f ) ,
												  int( Settings::Esp::chams_Color_VTT[2] * 255.f ) );
					}
				}
				else
					TeamVisibleColor = TeamHideColor;

				if (CheckTeam && !Settings::Misc::misc_ChamsMaterials)
				{
					if (Settings::Esp::esp_ChamsVisible <= 2)
					{
						if (Settings::Esp::esp_Chams == 1 && Settings::Esp::esp_XQZ == true)
						{
							ForceMaterial(TeamHideColor, hidden_flat);
							hidden_flat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);
						}
						else if (Settings::Esp::esp_Chams == 2 && Settings::Esp::esp_XQZ == true)
						{
							ForceMaterial(TeamHideColor, hidden_tex);
							hidden_tex->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);
						}
						else if (Settings::Esp::esp_Chams == 1 && Settings::Esp::esp_XQZ == false)
						{
							ForceMaterial(TeamHideColor, hidden_flat);
							hidden_flat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
						}
						else if (Settings::Esp::esp_Chams == 2 && Settings::Esp::esp_XQZ == false)
						{
							ForceMaterial(TeamHideColor, hidden_tex);
							hidden_tex->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
						}
					}

					Interfaces::ModelRender()->DrawModelExecute(ctx, state, pInfo, pCustomBoneToWorld);

					if (Settings::Esp::esp_Chams == 1)
					{
						ForceMaterial(TeamVisibleColor, visible_flat);
						visible_flat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
					}
					else if (Settings::Esp::esp_Chams >= 2)
					{
						ForceMaterial(TeamVisibleColor, visible_tex);
						visible_tex->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
					}
				}
			}
		}
	}
}

void CEsp::DrawPlayerEsp( CPlayer* pPlayer )
{
	bool bOriginScreen = ( pPlayer->vOriginScreen.x > 0 && pPlayer->vOriginScreen.y > 0 );
	bool bHitBoxScreen = ( pPlayer->vHitboxHeadScreen.x > 0 && pPlayer->vHitboxHeadScreen.y > 0 );

	if ( !bOriginScreen && !bHitBoxScreen )
		return;

	Vector vLineOrigin;

	if ( Settings::Esp::esp_Size < 0 )
		Settings::Esp::esp_Size = 1;
	else if ( Settings::Esp::esp_Size > 10 )
		Settings::Esp::esp_Size = 10;

	int Height = (int)pPlayer->vOriginScreen.y - (int)pPlayer->vHitboxHeadScreen.y;

	if ( Height < 18 )
		Height = 18;

	int Width = Height / 2;

	int x = (int)pPlayer->vHitboxHeadScreen.x - Width / 2;
	int y = (int)pPlayer->vHitboxHeadScreen.y;

	vLineOrigin = pPlayer->vHitboxHeadScreen;
	vLineOrigin.y += Height;

	Color EspPlayerColor = GetPlayerColor( pPlayer );
	Color EspVisibleColor = GetPlayerVisibleColor( pPlayer );

	if (Settings::Esp::esp_Size)
	{
		if (Settings::Esp::esp_Style == 1)
		{
			if (!Settings::Esp::esp_Outline)
				g_pRender->DrawBox(x, y, Width, Height, EspVisibleColor);
			else if (Settings::Esp::esp_Outline)
				g_pRender->DrawOutlineBox(x, y, Width, Height, EspVisibleColor);
		}
		else if (Settings::Esp::esp_Style >= 2)
		{
			if (!Settings::Esp::esp_Outline)
				g_pRender->DrawCoalBox(x, y, Width, Height, EspVisibleColor);
			else if (Settings::Esp::esp_Outline)
				g_pRender->DrawOutlineCoalBox(x, y, Width, Height, EspVisibleColor);
		}
	}

	if ( Settings::Esp::esp_Line )
		g_pRender->DrawLine( (int)vLineOrigin.x , (int)vLineOrigin.y , iScreenWidth / 2 , iScreenHeight , EspVisibleColor );

	if ( Settings::Esp::esp_Name )
		g_pRender->Text( (int)vLineOrigin.x , (int)pPlayer->vHitboxHeadScreen.y - 13 , true , true , EspPlayerColor , pPlayer->Name.c_str() );

	int iHpAmY = 1;

	if ( Settings::Esp::esp_Health >= 1 )
	{
		Color Minus = Color::Red();

		if ( pPlayer->Team == TEAM_CT )
			Minus = CT_HP_ColorM;
		else if ( pPlayer->Team == TEAM_TT )
			Minus = TT_HP_ColorM;

		int iHealth = pPlayer->iHealth;

		if ( iHealth )
		{
			if ( Settings::Esp::esp_Health == 1 )
			{
				g_pRender->Text( (int)vLineOrigin.x , (int)vLineOrigin.y + iHpAmY , true , true , EspPlayerColor , to_string( iHealth ).c_str() );
				iHpAmY += 10;
			}
			else if ( Settings::Esp::esp_Health == 2 )
			{
				g_pRender->DrawHorBar( x , (int)vLineOrigin.y + iHpAmY , Width , 5 , iHealth , Color::LawnGreen() , Minus );
				iHpAmY += 6;
			}
			else if ( Settings::Esp::esp_Health >= 3 )
			{
				g_pRender->DrawVerBar( x - 6 , (int)pPlayer->vHitboxHeadScreen.y , 5 , Height , iHealth , Color::LawnGreen() , Minus );
			}
		}
	}

	if ( Settings::Esp::esp_Armor >= 1 )
	{
		Color Minus = Color::Red();

		if ( pPlayer->Team == TEAM_CT )
			Minus = CT_AR_ColorM;
		else if ( pPlayer->Team == TEAM_TT )
			Minus = TT_AR_ColorM;

		int iArmor = pPlayer->iArmor;

		if ( iArmor )
		{
			if ( Settings::Esp::esp_Armor == 1 )
			{
				g_pRender->Text( (int)vLineOrigin.x , (int)vLineOrigin.y + iHpAmY , true , true ,
								 EspPlayerColor ,
								 to_string( iArmor ).c_str() );
				iHpAmY += 10;
			}
			if ( Settings::Esp::esp_Armor == 2 )
			{
				g_pRender->DrawHorBar( x , (int)vLineOrigin.y + iHpAmY , Width , 5 , iArmor ,
									   Color::White() , Minus );
				iHpAmY += 6;
			}
			else if ( Settings::Esp::esp_Armor >= 3 )
			{
				g_pRender->DrawVerBar( x + Width + 1 , (int)pPlayer->vHitboxHeadScreen.y , 5 , Height , iArmor ,
									   Color::White() , Minus );
			}
		}
	}

	if ( Settings::Esp::esp_Weapon && !pPlayer->WeaponName.empty() )
	{
		string WeaponStr = pPlayer->WeaponName; WeaponStr.erase(0, 6);

		if ( Settings::Esp::esp_Ammo && pPlayer->iWAmmo )
		{
			WeaponStr += " (";
			WeaponStr += to_string( pPlayer->iWAmmo );
			WeaponStr += ")";
		}

		g_pRender->Text( (int)vLineOrigin.x , (int)vLineOrigin.y + iHpAmY , true , true , EspPlayerColor , WeaponStr.c_str() );
		iHpAmY += 10;
	}

	if ( Settings::Esp::esp_Distance && g_pPlayers->GetLocal()->bAlive )
	{
		int Distance = pPlayer->iDistance;
		g_pRender->Text( (int)vLineOrigin.x , (int)vLineOrigin.y + iHpAmY , true , true , EspPlayerColor , to_string( Distance ).c_str() );
	}

	if (Settings::Esp::esp_Infoz)
	{
		if (pPlayer->m_pEntity->IsDefusing())
		{
			int iHpAmY = 10;
			g_pRender->Text((int)vLineOrigin.x, (int)vLineOrigin.y + iHpAmY, true, true, Color::GreenYellow(), ("Defusing"));
		}
		if (pPlayer->m_pEntity->GetIsScoped())
		{
			int iHpAmY = 10;
			g_pRender->Text((int)vLineOrigin.x, (int)vLineOrigin.y + iHpAmY, true, true, Color::GreenYellow(), ("Scoped"));

		}
		if (pPlayer->m_pEntity->IsFlashed())
		{
			int iHpAmY = 10;
			g_pRender->Text((int)vLineOrigin.x, (int)vLineOrigin.y + iHpAmY, true, true, Color::GreenYellow(), ("Flashed"));
		}/* fix it if you want 
 		if (pPlayer->m_pEntity->IsReloading())
		{
			int iHpAmY = 10;
			g_pRender->Text((int)vLineOrigin.x, (int)vLineOrigin.y + iHpAmY, true, true, Color::GreenYellow(), ("Reloading"));
		}*/
	}

}



void CEsp::DrawPlayerSkeleton( CPlayer* pPlayer )
{
	Color SkeletonColor = GetPlayerColor( pPlayer );

	for ( BYTE IndexArray = 0; IndexArray < 18; IndexArray++ )
		DrawHitBoxLine( pPlayer->vHitboxSkeletonArray[IndexArray] , SkeletonColor );
}

void CEsp::DrawPlayerBulletTrace( CPlayer* pPlayer )
{
	Color EspColor = GetPlayerColor( pPlayer );

	if ( !pPlayer->vBulletTraceArray[0].IsZero() && !pPlayer->vBulletTraceArray[1].IsZero() )
	{
		g_pRender->DrawLine(
			(int)pPlayer->vBulletTraceArray[0].x , (int)pPlayer->vBulletTraceArray[0].y ,
			(int)pPlayer->vBulletTraceArray[1].x , (int)pPlayer->vBulletTraceArray[1].y , EspColor );

		g_pRender->DrawFillBox( (int)pPlayer->vBulletTraceArray[1].x - 2 , (int)pPlayer->vBulletTraceArray[1].y - 2 , 4 , 4 , EspColor );
	}
}

void CEsp::DrawHitBoxLine( Vector* vHitBoxArray , Color color )
{
	Vector vHitBoxOneScreen;
	Vector vHitBoxTwoScreen;

	if ( vHitBoxArray[0].IsZero() || !vHitBoxArray[0].IsValid() || vHitBoxArray[1].IsZero() || !vHitBoxArray[1].IsValid() )
		return;

	if ( WorldToScreen( vHitBoxArray[0] , vHitBoxOneScreen ) && WorldToScreen( vHitBoxArray[1] , vHitBoxTwoScreen ) )
	{
		g_pRender->DrawLine( (int)vHitBoxOneScreen.x , (int)vHitBoxOneScreen.y ,
			(int)vHitBoxTwoScreen.x , (int)vHitBoxTwoScreen.y , color );
	}
}

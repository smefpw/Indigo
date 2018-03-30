#include "Triggerbot.h"

using namespace Client;

#pragma warning(disable:4244)
//[junk_enable /]
CTriggerbot::CTriggerbot()
{
	m_pLocal = nullptr;
	m_pCmd = nullptr;

	iPreFov = 0;
	iTriggerShotsFired = 0;
	bTrigger = false;
	bAssist = false;
	bFovTime = false;
	bIsZoomSniper = false;
	bTriggerAttack = false;
	dwTriggerStatusTime = 0;

	m_pTriggerDelayBefore = new CTimer();
	m_pTriggerDelayAfter = new CTimer();
}

void CTriggerbot::LeftClick()
{
	INPUT    Input;
	memset( &Input , 0 , sizeof( INPUT ) );
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	SendInput( 1 , &Input , sizeof( INPUT ) );
	memset( &Input , 0 , sizeof( INPUT ) );
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
	SendInput( 1 , &Input , sizeof( INPUT ) );
}

void CTriggerbot::RightClick()
{
	INPUT    Input;
	memset( &Input , 0 , sizeof( INPUT ) );
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
	SendInput( 1 , &Input , sizeof( INPUT ) );
	memset( &Input , 0 , sizeof( INPUT ) );
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
	SendInput( 1 , &Input , sizeof( INPUT ) );
}

bool CTriggerbot::IsEnable()
{
	if ( !m_pLocal || !m_pCmd )
		return false;

	if ( Settings::Triggerbot::trigger_Enable == false )
		return false;

	if ( !m_pLocal->WeaponAmmo || m_pLocal->bInReload )
		return false;

	if ( m_pLocal->WeaponType > WEAPON_TYPE_SNIPER )
		return false;

	return true;
}

void CTriggerbot::TriggerShow( CPlayer * pPlayer )
{
	if ( !IsEnable() )
		return;

	if ( Settings::Triggerbot::trigger_Enable )
	{
		if ( m_pLocal->WeaponType < WEAPON_TYPE_GRENADE && TriggerCheckTeam( pPlayer ) )
		{
			if ( Settings::Triggerbot::trigger_DrawFov && Settings::Triggerbot::trigger_Enable <= 1 )
				TriggerShowFov( pPlayer );

			if ( Settings::Triggerbot::weapon_trigger_settings[iWeaponID].trigger_Assist && Settings::Triggerbot::trigger_DrawFovAssist )
				TriggerShowAssistFov( pPlayer );

			if ( Settings::Triggerbot::trigger_DrawSpot && Settings::Triggerbot::trigger_Enable <= 1 )
			{
				if ( Settings::Triggerbot::weapon_trigger_settings[iWeaponID].trigger_HeadOnly )
				{
					g_pRender->DrawBox( pPlayer->vTriggerHeadPos.x , pPlayer->vTriggerHeadPos.y , 2 , 2 , Color::White() );
				}
				else
				{
					g_pRender->DrawBox( pPlayer->vTriggerHeadPos.x , pPlayer->vTriggerHeadPos.y , 2 , 2 , Color::White() );
					g_pRender->DrawBox( pPlayer->vTriggerChestPos.x , pPlayer->vTriggerChestPos.y , 2 , 2 , Color::White() );
				}
			}
			else if ( Settings::Triggerbot::trigger_DrawSpot && Settings::Triggerbot::trigger_Enable >= 2 )
			{
				g_pRender->DrawBox( pPlayer->vTriggerHeadPos.x , pPlayer->vTriggerHeadPos.y , 2 , 2 , Color::White() );

				if ( Settings::Triggerbot::weapon_trigger_settings[iWeaponID].trigger_HitGroup <= 1 )
					g_pRender->DrawBox( pPlayer->vTriggerChestPos.x , pPlayer->vTriggerChestPos.y , 2 , 2 , Color::White() );
			}
		}
	}
}

void CTriggerbot::TriggerShowFov( CPlayer * pPlayer )
{
	if ( pPlayer->vTriggerHeadPos.x > 0.f )
	{
		int iHeadX = (int)pPlayer->vTriggerHeadPos.x;
		int iHeadY = (int)pPlayer->vTriggerHeadPos.y;

		int iHeadFov = GetTriggerHeadFov( pPlayer );

		if ( Settings::Triggerbot::weapon_trigger_settings[iWeaponID].trigger_HeadOnly )
		{
			g_pRender->DrawBox( iHeadX - iHeadFov , iHeadY - iHeadFov , iHeadFov * 2 , iHeadFov * 2 , Color( 30 , 255 , 30 ) );
		}
		else if ( Settings::Triggerbot::weapon_trigger_settings[iWeaponID].trigger_HeadOnly == false && pPlayer->vTriggerChestPos.x > 0.f )
		{
			int iChestX = (int)pPlayer->vTriggerChestPos.x;
			int iChestY = (int)pPlayer->vTriggerChestPos.y;

			int iChestFovX = GetTriggerChestFovX( pPlayer );
			int iChestFovY = GetTriggerChestFovY( pPlayer );

			g_pRender->DrawBox( iHeadX - iHeadFov , iHeadY - iHeadFov , iHeadFov * 2 , iHeadFov * 2 , Color( 255 , 30 , 255 ) );
			g_pRender->DrawBox( iChestX - iChestFovX , iChestY - iChestFovY , iChestFovX * 2 , iChestFovY * 2 , Color( 255 , 30 , 255 ) );
		}
	}
}

void CTriggerbot::TriggerShowAssistFov( CPlayer * pPlayer )
{
	if ( pPlayer->vTriggerHeadPos.x > 0.f )
	{
		int iHeadX = (int)pPlayer->vTriggerHeadPos.x;
		int iHeadY = (int)pPlayer->vTriggerHeadPos.y;

		int iHeadFov = GetTriggerAssistHeadFov( pPlayer );

		if ( Settings::Triggerbot::weapon_trigger_settings[iWeaponID].trigger_AssistFovType <= 0 )
		{
			if ( Settings::Triggerbot::weapon_trigger_settings[iWeaponID].trigger_HeadOnly )
			{
				g_pRender->DrawBox( iHeadX - iHeadFov , iHeadY - iHeadFov , iHeadFov * 2 , iHeadFov * 2 , Color( 255 , 230 , 0 ) );
			}
			else if ( Settings::Triggerbot::weapon_trigger_settings[iWeaponID].trigger_HeadOnly == false && pPlayer->vTriggerChestPos.x > 0.f )
			{
				int iChestX = (int)pPlayer->vTriggerChestPos.x;
				int iChestY = (int)pPlayer->vTriggerChestPos.y;

				int iChestFovX = GetTriggerAssistChestFovX( pPlayer );
				int iChestFovY = GetTriggerAssistChestFovY( pPlayer );

				g_pRender->DrawBox( iHeadX - iHeadFov , iHeadY - iHeadFov , iHeadFov * 2 , iHeadFov * 2 , Color( 255 , 230 , 0 ) );
				g_pRender->DrawBox( iChestX - iChestFovX , iChestY - iChestFovY , iChestFovX * 2 , iChestFovY * 2 , Color( 255 , 230 , 0 ) );
			}
		}
		else if ( Settings::Triggerbot::weapon_trigger_settings[iWeaponID].trigger_AssistFovType >= 1 )
		{
			g_pRender->DrawBox( g_vCenterScreen.x - iHeadFov , g_vCenterScreen.y - iHeadFov , iHeadFov * 2 , iHeadFov * 2 , Color( 255 , 230 , 0 ) );
		}
	}
}

int CTriggerbot::GetTriggerHeadFov( CPlayer * pPlayer )
{
	int iFov = 0;

	iFov = (int)( ( pow( Settings::Triggerbot::weapon_trigger_settings[iWeaponID].trigger_Fov + FIX_MIN_FOV_HEAD , 2 ) * 90 ) / ( pPlayer->iDistance * m_pLocal->iFov ) );

	return iFov;
}

int CTriggerbot::GetTriggerChestFovX( CPlayer * pPlayer )
{
	return (int)( ( pow( Settings::Triggerbot::weapon_trigger_settings[iWeaponID].trigger_Fov + FIX_MIN_FOV_CHEST , 2 ) * 90 ) / ( pPlayer->iDistance * m_pLocal->iFov ) );
}

int CTriggerbot::GetTriggerChestFovY( CPlayer * pPlayer )
{
	return (int)( ( pow( ( Settings::Triggerbot::weapon_trigger_settings[iWeaponID].trigger_Fov + FIX_MIN_FOV_CHEST ) * 1.25 , 2 ) * 90 ) / ( pPlayer->iDistance * m_pLocal->iFov ) );
}

int CTriggerbot::GetTriggerAssistHeadFov( CPlayer * pPlayer )
{
	int iFov = 0;

	if ( Settings::Triggerbot::trigger_Enable <= 2 )
	{
		if ( Settings::Triggerbot::weapon_trigger_settings[iWeaponID].trigger_AssistFovType <= 0 )
		{
			iFov = ( ( pow( Settings::Triggerbot::weapon_trigger_settings[iWeaponID].trigger_Fov + Settings::Triggerbot::weapon_trigger_settings[iWeaponID].trigger_AssistFov + FIX_MIN_FOV_HEAD , 2 ) * 90 ) / ( pPlayer->iDistance * m_pLocal->iFov ) );
		}
		else if ( Settings::Triggerbot::weapon_trigger_settings[iWeaponID].trigger_AssistFovType >= 1 )
		{
			iFov = ( ( pow( Settings::Triggerbot::weapon_trigger_settings[iWeaponID].trigger_AssistFov + FIX_MIN_FOV_HEAD , 2 ) * 90 ) / ( FOV_BASE_DISTANCE * m_pLocal->iFov ) );
		}
	}
	else
	{
		if ( Settings::Triggerbot::weapon_trigger_settings[iWeaponID].trigger_AssistFovType <= 0 )
		{
			iFov = ( ( pow( Settings::Triggerbot::weapon_trigger_settings[iWeaponID].trigger_AssistFov + FIX_MIN_FOV_HEAD , 2 ) * 90 ) / ( pPlayer->iDistance * m_pLocal->iFov ) );
		}
		else if ( Settings::Triggerbot::weapon_trigger_settings[iWeaponID].trigger_AssistFovType >= 1 )
		{
			iFov = ( ( pow( Settings::Triggerbot::weapon_trigger_settings[iWeaponID].trigger_AssistFov + FIX_MIN_FOV_HEAD , 2 ) * 90 ) / ( FOV_BASE_DISTANCE * m_pLocal->iFov ) );
		}
	}

	return iFov;
}

int CTriggerbot::GetTriggerAssistChestFovX( CPlayer * pPlayer )
{
	int iFov = 0;

	if ( Settings::Triggerbot::trigger_Enable <= 2 )
	{
		iFov = ( ( pow( Settings::Triggerbot::weapon_trigger_settings[iWeaponID].trigger_Fov + Settings::Triggerbot::weapon_trigger_settings[iWeaponID].trigger_AssistFov + FIX_MIN_FOV_CHEST , 2 ) * 90 ) / ( pPlayer->iDistance * m_pLocal->iFov ) );
	}
	else
	{
		iFov = ( ( pow( Settings::Triggerbot::weapon_trigger_settings[iWeaponID].trigger_AssistFov + FIX_MIN_FOV_CHEST , 2 ) * 90 ) / ( pPlayer->iDistance * m_pLocal->iFov ) );
	}
	
	return iFov;
}

int CTriggerbot::GetTriggerAssistChestFovY( CPlayer * pPlayer )
{
	int iFov = 0;

	if ( Settings::Triggerbot::trigger_Enable <= 2 )
	{
		iFov = ( ( pow( ( Settings::Triggerbot::weapon_trigger_settings[iWeaponID].trigger_Fov + Settings::Triggerbot::weapon_trigger_settings[iWeaponID].trigger_AssistFov + FIX_MIN_FOV_CHEST ) * 1.25 , 2 ) * 90 ) / ( pPlayer->iDistance * m_pLocal->iFov ) );
	}
	else
	{
		iFov = ( ( pow( ( Settings::Triggerbot::weapon_trigger_settings[iWeaponID].trigger_AssistFov + FIX_MIN_FOV_CHEST ) * 1.25 , 2 ) * 90 ) / ( pPlayer->iDistance * m_pLocal->iFov ) );
	}
	
	return iFov;
}

void CTriggerbot::TriggerShowStatus()
{
	if ( Settings::Triggerbot::trigger_Enable )
	{
		if ( Settings::Triggerbot::trigger_KeyMode >= 1 )
		{
			if ( GetTickCount64() - dwTriggerStatusTime < 1000 )
			{
				con_nprint_t print;
				print.index = 1;
				print.fixed_width_font = false;
				print.time_to_live = 1;

				if ( bTrigger )
				{
					print.color[0] = 0.f;
					print.color[1] = 1.f;
					print.color[2] = 0.f;

					Interfaces::Engine()->Con_NXPrintf( &print , "%s" , CVAR_TRIGGER_ENABLED );
				}
				else
				{
					print.color[0] = 1.f;
					print.color[1] = 0.f;
					print.color[2] = 0.f;

					Interfaces::Engine()->Con_NXPrintf( &print , "%s" , CVAR_TRIGGER_DISABLED );
				}
			}
		}
		else if ( bTrigger )
		{
			g_pRender->Text( iScreenWidth / 2 , 65 , true , true , Color( 0 , 255 , 0 ) , "%s" , CVAR_TRIGGER_ENABLED );
		}
	}
}

bool CTriggerbot::TriggerCheckTeam( CPlayer * pPlayer )
{
	bool CheckTeam = false;
	bool CheckVis = false;

	// Работа триггера только на:
	if ( Settings::Triggerbot::trigger_Deathmatch == false && m_pLocal->Team != pPlayer->Team ) // Противников
	{
		CheckTeam = true;
	}
	else if ( Settings::Triggerbot::trigger_Deathmatch ) // На всех
	{
		CheckTeam = true;
	}
	
	if ( Settings::Triggerbot::trigger_WallAttack )
	{
		CheckVis = true;
	}
	else if ( !Settings::Triggerbot::trigger_WallAttack && pPlayer->bVisible )
	{
		CheckVis = true;
	}
	
	return ( CheckTeam && CheckVis );
}

bool CTriggerbot::CheckPlayerTriggerLine( CPlayer * pPlayer )
{
	trace_t tr;
	Ray_t ray;
	Vector vAngle;
	CTraceFilter filter;

	if ( Settings::Triggerbot::weapon_trigger_settings[iWeaponID].trigger_Assist )
	{
		vAngle.x = m_pCmd->viewangles.x;
		vAngle.y = m_pCmd->viewangles.y;
	}
	else
	{
		vAngle.x = m_pCmd->viewangles.x + ( m_pLocal->vAimPunch.x * 2 );
		vAngle.y = m_pCmd->viewangles.y + ( m_pLocal->vAimPunch.y * 2 );
	}

	Vector	vecStart , vecStop , vecFor;

	AngleVectors( vAngle , vecFor );

	CWeaponInfo* Weapon = m_pLocal->m_pWeaponEntity->GetWeaponInfo();

	if ( Weapon )
	{
		vecFor *= Weapon->m_flRange;
	}
	else
	{
		vecFor *= 8192.0f;
	}

	filter.pSkip = m_pLocal->m_pEntity;

	vecStart = m_pLocal->vEyeOrigin;
	vecStop = vecStart + vecFor;

	ray.Init( vecStart , vecStop );

	Interfaces::EngineTrace()->TraceRay( ray , PlayerVisibleMask , &filter , &tr );

	bool didHit = false;

	if ( Settings::Triggerbot::trigger_Enable == 1 )
	{
		didHit = true;
	}
	else if ( Settings::Triggerbot::trigger_Enable >= 2 )
	{
		int hitgroup = tr.hitgroup;

		if ( ( hitgroup == HITGROUP_CHEST || hitgroup == HITGROUP_HEAD || hitgroup == HITGROUP_STOMACH ) && Settings::Triggerbot::weapon_trigger_settings[iWeaponID].trigger_HitGroup == 1 )
		{
			didHit = true;
		}
		else if ( hitgroup == HITGROUP_HEAD && Settings::Triggerbot::weapon_trigger_settings[iWeaponID].trigger_HitGroup >= 2 )
		{
			didHit = true;
		}
		else if ( Settings::Triggerbot::weapon_trigger_settings[iWeaponID].trigger_HitGroup <= 0 && hitgroup >= 1 && hitgroup <= 7 )
		{
			didHit = true;
		}
	}

	if ( tr.m_pEnt == pPlayer->m_pEntity && didHit )
		return true;

	return false;
}

byte CTriggerbot::GetCountPlayerAssistFov()
{
	byte cnt = 0;

	for ( BYTE PlayerIndex = 0; PlayerIndex < g_pPlayers->GetSize(); PlayerIndex++ )
	{
		CPlayer* pPlayer = g_pPlayers->GetPlayer( PlayerIndex );

		if ( pPlayer && pPlayer->bUpdate && pPlayer->bAssistFov )
			cnt++;
	}

	return cnt;
}

bool CTriggerbot::CheckPlayerTriggerPos()
{
	for ( BYTE PlayerIndex = 0; PlayerIndex < g_pPlayers->GetSize(); PlayerIndex++ )
	{
		CPlayer* pPlayer = g_pPlayers->GetPlayer( PlayerIndex );

		if ( pPlayer && pPlayer->bUpdate && pPlayer->bTriggerFov )
			return true;
	}

	return false;
}

bool CTriggerbot::CheckPlayerAssistTriggerPos()
{
	for ( BYTE PlayerIndex = 0; PlayerIndex < g_pPlayers->GetSize(); PlayerIndex++ )
	{
		CPlayer* pPlayer = g_pPlayers->GetPlayer( PlayerIndex );

		if ( pPlayer && pPlayer->bUpdate && pPlayer->bAssistFov )
			return true;
	}

	return false;
}

void CTriggerbot::TriggerAssistSet( Vector vAimAssist )
{
	Vector vAimAngle , vAimAssistAngle;
	Vector vSmoothAimAssistAngles;

	vAimAngle = vAimAssist - m_pLocal->vEyeOrigin;

	if ( vAimAngle.IsValid() )
	{
		VectorAngles( vAimAngle , vAimAssistAngle );

		if ( Settings::Triggerbot::weapon_trigger_settings[iWeaponID].trigger_AssistRcs )
		{
			vAimAssistAngle.x -= m_pLocal->vAimPunch.x * ( Settings::Triggerbot::weapon_trigger_settings[iWeaponID].trigger_AssistRcs * 0.02 );
			vAimAssistAngle.y -= m_pLocal->vAimPunch.y * ( Settings::Triggerbot::weapon_trigger_settings[iWeaponID].trigger_AssistRcs * 0.02 );
		}

		float fSmooth = 10.f;

		fSmooth = Settings::Triggerbot::weapon_trigger_settings[iWeaponID].trigger_AssistSmooth;

		if ( fSmooth <= 10.f )
		{
			fSmooth = 10.f;
		}
		else
		{
			fSmooth = fSmooth / 10.f;
		}

		AngleNormalize( vAimAssistAngle );

		SmoothAngles( m_pCmd->viewangles , vAimAssistAngle , vSmoothAimAssistAngles , fSmooth );

		m_pCmd->viewangles = vSmoothAimAssistAngles;
	}
}

void CTriggerbot::TriggerAssistUpd( Vector vAimAssist )
{
	if ( ( m_pLocal->WeaponType == WEAPON_TYPE_PISTOL || m_pLocal->WeaponType == WEAPON_TYPE_SHOTGUN ) )
	{
		TriggerAssistSet( vAimAssist );
	}
	else if ( m_pLocal->WeaponType == WEAPON_TYPE_SNIPER )
	{
		if ( Settings::Triggerbot::trigger_FastZoom )
		{
			TriggerAssistSet( vAimAssist );
		}
		else if ( !Settings::Triggerbot::trigger_FastZoom && bIsZoomSniper )
		{
			TriggerAssistSet( vAimAssist );
		}
		else if ( !Settings::Triggerbot::trigger_FastZoom && !bIsZoomSniper )
		{
			return;
		}
	}
}

void CTriggerbot::TriggerAssist( CPlayer * pPlayer )
{
	if ( Settings::Triggerbot::weapon_trigger_settings[iWeaponID].trigger_Assist )
	{
		// если режим 1 и мы не стреляем и не в вофе асиста вообще и асист выключен то включаем его
		if ( Settings::Triggerbot::weapon_trigger_settings[iWeaponID].trigger_Assist == 1 && !CheckPlayerAssistTriggerPos() && !( m_pCmd->buttons & IN_ATTACK ) && iTriggerShotsFired == 0 )
		{
			if ( !bAssist )
				bAssist = true;
		}

		float fScreenDist = 1000.f;

		byte bCountPlayerAssistFov = GetCountPlayerAssistFov();

		if ( pPlayer->bAssistFov )
		{
			if ( Settings::Triggerbot::weapon_trigger_settings[iWeaponID].trigger_Assist == 1 && ( m_pCmd->buttons & IN_ATTACK ) )
			{
				bAssist = false;
			}
			else if ( Settings::Triggerbot::weapon_trigger_settings[iWeaponID].trigger_Assist >= 2 )
			{
				bAssist = true;
			}
			
			if ( pPlayer->vAssistTriggerHead.IsValid() && pPlayer->vAssistTriggerChest.IsValid() && bAssist )
			{
				if ( Settings::Triggerbot::weapon_trigger_settings[iWeaponID].trigger_HeadOnly )
				{
					if ( bCountPlayerAssistFov >= 2 )
					{
						byte bTargetAssist = 0;

						for ( byte p = 0; p < ABSOLUTE_PLAYER_LIMIT; p++ )
						{
							if ( g_pPlayers->GetPlayer( p )->bAssistFov )
							{
								if ( g_pPlayers->GetPlayer( p )->fDistanceHead < fScreenDist )
								{
									fScreenDist = g_pPlayers->GetPlayer( p )->fDistanceHead;
									bTargetAssist = p;
								}
							}
						}

						TriggerAssistUpd( g_pPlayers->GetPlayer( bTargetAssist )->vAssistTriggerHead );
					}
					else
					{
						TriggerAssistUpd( pPlayer->vAssistTriggerHead );
					}
				}
				else
				{
					if ( bCountPlayerAssistFov >= 2 )
					{
						byte bTargetAssist = 0;
						byte bTargetAssistType = 0;

						Vector vAimAssist = Vector( 0 , 0 , 0 );

						for ( BYTE PlayerIndex = 0; PlayerIndex < g_pPlayers->GetSize(); PlayerIndex++ )
						{
							CPlayer* pPlayer = g_pPlayers->GetPlayer( PlayerIndex );

							if ( pPlayer && pPlayer->bUpdate && pPlayer->bAssistFov )
							{
								if ( g_pPlayers->GetPlayer( PlayerIndex )->fDistanceHead < fScreenDist && g_pPlayers->GetPlayer( PlayerIndex )->fDistanceHead < g_pPlayers->GetPlayer( PlayerIndex )->fDistanceChest )
								{
									fScreenDist = g_pPlayers->GetPlayer( PlayerIndex )->fDistanceHead;
									bTargetAssist = PlayerIndex;
									bTargetAssistType = 1;
								}
								else if ( g_pPlayers->GetPlayer( PlayerIndex )->fDistanceChest < fScreenDist && g_pPlayers->GetPlayer( PlayerIndex )->fDistanceChest < g_pPlayers->GetPlayer( PlayerIndex )->fDistanceHead )
								{
									fScreenDist = g_pPlayers->GetPlayer( PlayerIndex )->fDistanceChest;
									bTargetAssist = PlayerIndex;
									bTargetAssistType = 2;
								}
							}
						}

						if ( bTargetAssistType == 1 )
						{
							vAimAssist = g_pPlayers->GetPlayer( bTargetAssist )->vAssistTriggerHead;
						}
						else if ( bTargetAssistType == 2 )
						{
							vAimAssist = g_pPlayers->GetPlayer( bTargetAssist )->vAssistTriggerChest;
						}

						TriggerAssistUpd( vAimAssist );
					}
					else
					{
						Vector vAimAssist = Vector( 0 , 0 , 0 );

						if ( pPlayer->fDistanceHead < pPlayer->fDistanceChest )
						{
							vAimAssist = pPlayer->vAssistTriggerHead;
						}
						else if ( pPlayer->fDistanceChest < pPlayer->fDistanceHead )
						{
							vAimAssist = pPlayer->vAssistTriggerChest;
						}

						TriggerAssistUpd( vAimAssist );
					}
				}

				// режим 1, пока не стрельним 1 раз в тригере или просто выстрел то наводка работает иначе отключаем асист
				// во 2 режиме всегда будет наводить 
				if ( Settings::Triggerbot::weapon_trigger_settings[iWeaponID].trigger_Assist == 1 && pPlayer->bTriggerFov && iTriggerShotsFired )
					bAssist = false;
			}
		}
	}
}

void CTriggerbot::TriggerAttack()
{
	bool bShotTrigger = false;

	if ( iTriggerShotsFired <= 0 )
	{
		if ( m_pTriggerDelayBefore->delay( Settings::Triggerbot::weapon_trigger_settings[iWeaponID].trigger_DelayBefore ) )
		{
			bShotTrigger = true;
		}
	}
	else
	{
		if ( m_pTriggerDelayAfter->delay( Settings::Triggerbot::weapon_trigger_settings[iWeaponID].trigger_DelayAfter ) )
		{
			bShotTrigger = true;
			m_pTriggerDelayAfter->reset();
		}
	}

	if ( bShotTrigger )
	{
		bTriggerAttack = true;
		LeftClick();
		iTriggerShotsFired++;
	}
}

void CTriggerbot::TriggerBot()
{
	bTriggerAttack = false;

	for ( BYTE PlayerIndex = 0; PlayerIndex < g_pPlayers->GetSize(); PlayerIndex++ )
	{
		CPlayer* pPlayer = g_pPlayers->GetPlayer( PlayerIndex );

		if ( !pPlayer->bUpdate || !bTrigger )
		{
			pPlayer->bAssistFov = false;
			pPlayer->bTriggerFov = false;
			continue;
		}

		bool bCheckTeam = TriggerCheckTeam( pPlayer );
		bool bSniperNoZoom = true;
		bool bSniperZoomAttack = true;

		float fPlayerDistance = (float)pPlayer->iDistance;

		if ( bCheckTeam && fPlayerDistance >= Settings::Triggerbot::weapon_trigger_settings[iWeaponID].trigger_DistanceMin && fPlayerDistance <= Settings::Triggerbot::weapon_trigger_settings[iWeaponID].trigger_DistanceMax )
		{
			int iHeadFov = GetTriggerHeadFov( pPlayer );
			int iAssistHeadFov = 0;

			if ( Settings::Triggerbot::weapon_trigger_settings[iWeaponID].trigger_Assist )
				iAssistHeadFov = GetTriggerAssistHeadFov( pPlayer );

			if ( Settings::Triggerbot::trigger_Enable == 1 ) // режим 1 только боксы
			{
				if ( Settings::Triggerbot::weapon_trigger_settings[iWeaponID].trigger_HeadOnly )
				{
					if ( CheckPlayerFov( pPlayer->vTriggerHeadPos , iHeadFov ) )
					{
						pPlayer->bTriggerFov = true;
					}
					else
					{
						pPlayer->bTriggerFov = false;
					}

					if ( Settings::Triggerbot::weapon_trigger_settings[iWeaponID].trigger_Assist )
					{
						if ( CheckPlayerFov( pPlayer->vTriggerHeadPos , iAssistHeadFov ) )
						{
							pPlayer->bAssistFov = true;
						}
						else
						{
							pPlayer->bAssistFov = false;
						}
					}
				}
				else if ( Settings::Triggerbot::weapon_trigger_settings[iWeaponID].trigger_HeadOnly == false )
				{
					int iChestFovX = 0;
					int iChestFovY = 0;

					int iAssistChestFovX = 0;
					int iAssistChestFovY = 0;

					iChestFovX = GetTriggerChestFovX( pPlayer );
					iChestFovY = GetTriggerChestFovY( pPlayer );

					if ( Settings::Triggerbot::weapon_trigger_settings[iWeaponID].trigger_Assist )
					{
						if ( Settings::Triggerbot::weapon_trigger_settings[iWeaponID].trigger_AssistFovType <= 0 )
						{
							iAssistChestFovX = GetTriggerAssistChestFovX( pPlayer );
							iAssistChestFovY = GetTriggerAssistChestFovY( pPlayer );
						}
						else if ( Settings::Triggerbot::weapon_trigger_settings[iWeaponID].trigger_AssistFovType >= 1 )
						{
							iAssistChestFovX = GetTriggerAssistHeadFov( pPlayer );
							iAssistChestFovY = GetTriggerAssistHeadFov( pPlayer );
						}
					}

					if ( CheckPlayerFov( pPlayer->vTriggerHeadPos , iHeadFov ) ||
						 CheckPlayerFov( pPlayer->vTriggerChestPos , iChestFovX , iChestFovY ) )
					{
						pPlayer->bTriggerFov = true;
					}
					else
					{
						pPlayer->bTriggerFov = false;
					}

					if ( Settings::Triggerbot::weapon_trigger_settings[iWeaponID].trigger_Assist )
					{
						if ( CheckPlayerFov( pPlayer->vTriggerHeadPos , iAssistHeadFov ) ||
							 CheckPlayerFov( pPlayer->vTriggerChestPos , iAssistChestFovX , iAssistChestFovY ) )
							 {
							pPlayer->bAssistFov = true;
							 }
						else
						{
							pPlayer->bAssistFov = false;
						}
					}
				}
			}
			else if ( Settings::Triggerbot::trigger_Enable >= 2 ) // режим 3 только трейс лайн без бокса
			{
				if ( CheckPlayerTriggerLine( pPlayer ) )
				{
					pPlayer->bTriggerFov = true;
				}
				else
				{
					pPlayer->bTriggerFov = false;
				}

				int iAssistChestFovX = 0;
				int iAssistChestFovY = 0;

				if ( Settings::Triggerbot::weapon_trigger_settings[iWeaponID].trigger_Assist )
				{
					if ( Settings::Triggerbot::weapon_trigger_settings[iWeaponID].trigger_AssistFovType <= 0 )
					{
						iAssistChestFovX = GetTriggerAssistChestFovX( pPlayer );
						iAssistChestFovY = GetTriggerAssistChestFovY( pPlayer );
					}
					else if ( Settings::Triggerbot::weapon_trigger_settings[iWeaponID].trigger_AssistFovType >= 1 )
					{
						iAssistChestFovX = GetTriggerAssistHeadFov( pPlayer );
						iAssistChestFovY = GetTriggerAssistHeadFov( pPlayer );
					}

					if ( CheckPlayerFov( pPlayer->vTriggerHeadPos , iAssistHeadFov ) ||
						 CheckPlayerFov( pPlayer->vTriggerChestPos , iAssistChestFovX , iAssistChestFovY ) )
						 {
						pPlayer->bAssistFov = true;
						 }
					else
					{
						pPlayer->bAssistFov = false;
					}
				}
			}

			if ( m_pLocal->WeaponType == WEAPON_TYPE_SNIPER )
			{
				if ( Settings::Triggerbot::trigger_FastZoom && m_pLocal->iFov == 90 && pPlayer->bTriggerFov )
				{
					m_pCmd->buttons |= IN_ATTACK2;
				}

				if ( m_pLocal->iFov != iPreFov && iPreFov && !bFovTime )
				{
					bFovTime = true;
				}

				iPreFov = m_pLocal->iFov;

				static CTimer TriggerDelayZoom;

				if ( bFovTime )
				{
					if ( TriggerDelayZoom.delay( 50 ) )
					{
						TriggerDelayZoom.reset();
						bFovTime = false;

						bSniperZoomAttack = true;
					}
					else
					{
						bSniperZoomAttack = false;
					}
				}
			}

			// Стрелять в снайперах только в зуме
			if ( m_pLocal->WeaponType != WEAPON_TYPE_SNIPER )
			{
				bSniperNoZoom = true;
			}
			else if ( m_pLocal->iFov == 90 && m_pLocal->WeaponType == WEAPON_TYPE_SNIPER )
			{
				bSniperNoZoom = false;
			}

			if ( m_pLocal->iFov != 90 && m_pLocal->WeaponType == WEAPON_TYPE_SNIPER )
			{
				bIsZoomSniper = true;
			}
			else
			{
				bIsZoomSniper = false;
			}

			if ( m_pLocal->WeaponType == WEAPON_TYPE_PISTOL || m_pLocal->WeaponType == WEAPON_TYPE_SHOTGUN )
			{
				if ( !CheckPlayerTriggerPos() && !pPlayer->bTriggerFov )
				{
					iTriggerShotsFired = 0;
					m_pTriggerDelayBefore->reset();
					m_pTriggerDelayAfter->reset();
				}
			}
			else if ( m_pLocal->WeaponType == WEAPON_TYPE_SNIPER )
			{
				if ( !CheckPlayerTriggerPos() && !pPlayer->bTriggerFov )
				{
					iTriggerShotsFired = 0;
					m_pTriggerDelayBefore->reset();
					m_pTriggerDelayAfter->reset();
				}
			}

			if ( Settings::Triggerbot::trigger_SmokCheck )
			{
				if ( LineGoesThroughSmoke( m_pLocal->vEyeOrigin , pPlayer->vAssistTriggerHead ) ||
					 LineGoesThroughSmoke( m_pLocal->vEyeOrigin , pPlayer->vAssistTriggerChest ) )
					 {
					continue;
					 }
			}

			if ( !( m_pCmd->buttons & IN_ATTACK ) )
			{
				TriggerAssist( pPlayer );

				if ( bSniperNoZoom && bSniperZoomAttack && pPlayer->bTriggerFov )
				{
					TriggerAttack();
				}
			}
		}
	}
}

void CTriggerbot::OnCreateMove( CUserCmd * pCmd , CMe * pLocal )
{
	m_pLocal = pLocal;
	m_pCmd = pCmd;

	int KEY_CODE = 0;

	if ( Settings::Triggerbot::trigger_Key <= 0 )
	{
		KEY_CODE = VK_MBUTTON;
	}
	else if ( Settings::Triggerbot::trigger_Key == 1 )
	{
		KEY_CODE = VK_XBUTTON1;
	}
	else if ( Settings::Triggerbot::trigger_Key >= 2 )
	{
		KEY_CODE = VK_XBUTTON2;
	}

	if ( Settings::Triggerbot::trigger_KeyMode >= 1 )
	{
		if ( KEY_TRIGGER_DOWN( KEY_CODE ) )
		{
			bTrigger = !bTrigger;
			dwTriggerStatusTime = GetTickCount64();
		}
	}
	else
	{
		if ( GetAsyncKeyState( KEY_CODE ) )
		{
			bTrigger = true;
		}
		else
		{
			bTrigger = false;
		}
	}

	if ( !IsEnable() )
		return;

	TriggerBot();
}
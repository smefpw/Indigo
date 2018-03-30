#include "Aimbot.h"

using namespace Client;

#pragma warning(disable:4244)
//[junk_enable /]
//[enc_string_enable /]
byte tt_ct_best_hit_1[6] =
{
	HITBOX_HEAD,
	HITBOX_NECK,
	HITBOX_LOWER_NECK,
	HITBOX_BODY,
	HITBOX_THORAX,
	HITBOX_CHEST,
};

#define TT_CT_BEST_HIT_SIZE_1 ( sizeof( tt_ct_best_hit_1 ) / sizeof( *tt_ct_best_hit_1 ) )

CAimbot::CAimbot()
{
	m_pLocal = nullptr;

	m_iBestPreTarget = -1;

	m_iBestTarget = -1;
	m_iBestHitbox = -1;

	m_bClamp = false;
	m_bAttack = false;
	m_bAimShot = false;
	m_bTargetFov = false;
	m_bTargetChange = false;
	m_bAutoPistolEn = false;

	m_pShotDelay = new CTimer();

	m_vAimBestHitboxScreen.Zero();
}

bool CAimbot::IsEnable()
{
	if ( !m_pLocal || !m_pCmd )
		return false;

	if ( !Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_Active )
		return false;

	if ( !m_pLocal->WeaponAmmo || m_pLocal->bInReload )
		return false;

	if ( m_pLocal->WeaponType > WEAPON_TYPE_SNIPER )
		return false;

	if ( g_pTriggerbot && g_pTriggerbot->bTriggerAttack )
		return false;

	return true;
}

int CAimbot::GetPlayerFov( CPlayer * pPlayer )
{
	int iFov = 0;
	int iFovVal = 0;

	if ( m_pLocal->iShotsFired > 1 && m_pLocal->WeaponType == WEAPON_TYPE_SHOTGUN )
	{
		iFovVal = Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_RcsFov;
	}
	else
	{
		iFovVal = Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_Fov;
	}

	int base_fov = pow( iFovVal + FIX_MIN_FOV_HEAD , 2 ) * 90;

	if ( Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_FovType <= 0 )
	{
		iFov = (int)( base_fov / ( pPlayer->iDistance * m_pLocal->iFov ) );
	}
	else
	{
		iFov = (int)( base_fov / ( FOV_BASE_DISTANCE * m_pLocal->iFov ) );
	}

	return iFov;
}

int CAimbot::GetBestTarget()
{
	float ScreenDistanceBase = 1000.f;

	int m_lBestTarget = -1;

	for ( BYTE PlayerIndex = 0; PlayerIndex < g_pPlayers->GetSize() /*&& g_pPlayers->bActive*/; PlayerIndex++ )
	{
		CPlayer* pPlayer = g_pPlayers->GetPlayer( PlayerIndex );

		if ( pPlayer && pPlayer->m_pEntity && pPlayer->bUpdate )
		{
			if ( !Settings::Aimbot::aim_Deathmatch && pPlayer->Team == m_pLocal->Team )
				continue;

			if ( !Settings::Aimbot::aim_WallAttack && !pPlayer->bVisible )
				continue;

			Vector vHitbox;
			Vector vHitboxScreen;

			if ( Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_BestHit >= 1 )
			{
				vHitbox = pPlayer->m_pEntity->GetHitboxPosition( HITBOX_BODY );
			}
			else
			{
				vHitbox = pPlayer->m_pEntity->GetHitboxPosition( tt_ct_best_hit_1[Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_Spot] );
			}

			if ( WorldToScreen( vHitbox , vHitboxScreen ) )
			{
				Vector2D vPlayerScr = Vector2D( vHitboxScreen.x , vHitboxScreen.y );

				float fDistanceScreen = DistanceScreen( g_vCenterScreen , vPlayerScr );

				if ( fDistanceScreen < ScreenDistanceBase )
				{
					ScreenDistanceBase = fDistanceScreen;
					m_lBestTarget = PlayerIndex;
				}
			}
		}
	}

	return m_lBestTarget;
}

int CAimbot::GetBestHitBox()
{
	float ScreenDistanceBase = 1000.f;

	int m_lBestHitbox = -1;

	if ( m_pLocal->WeaponType == WEAPON_TYPE_SHOTGUN )
	{
		if ( m_iBestHitbox && m_bAttack && !m_bTargetChange )
			m_lBestHitbox = m_iBestHitbox;
	}

	CPlayer* pPlayer = g_pPlayers->GetPlayer( m_iBestTarget );

	if ( pPlayer && pPlayer->m_pEntity && pPlayer->bUpdate )
	{
		if ( m_lBestHitbox == -1 )
		{
			if ( Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_BestHit >= 1 )
			{
				for ( byte bHitbox = 0; bHitbox < TT_CT_BEST_HIT_SIZE_1; bHitbox++ )
				{
					Vector vHitBox = pPlayer->m_pEntity->GetHitboxPosition( tt_ct_best_hit_1[bHitbox] );
					Vector vHitBoxScreen;

					if ( !WorldToScreen( vHitBox , vHitBoxScreen ) )
						continue;

					Vector2D vHitboxSrc = Vector2D( vHitBoxScreen.x , vHitBoxScreen.y );

					float fDistanceScreen = DistanceScreen( g_vCenterScreen , vHitboxSrc );

					if ( fDistanceScreen < ScreenDistanceBase )
					{
						ScreenDistanceBase = fDistanceScreen;
						m_lBestHitbox = tt_ct_best_hit_1[bHitbox];
					}
				}
			}
			else
			{
				m_lBestHitbox = tt_ct_best_hit_1[Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_Spot];
			}
		}

		Vector vHitBox = pPlayer->m_pEntity->GetHitboxPosition( m_lBestHitbox );

		if ( vHitBox.IsValid() && WorldToScreen( vHitBox , m_vAimBestHitboxScreen ) )
		{
			m_vAimBestHitbox = vHitBox;
			return m_lBestHitbox;
		}
		else
		{
			m_vAimBestHitbox.Zero();
			m_vAimBestHitboxScreen.Zero();
			return -1;
		}
	}

	return m_lBestHitbox;
}

void CAimbot::OnRender()
{	
	if ( !IsEnable() || m_iBestTarget == -1 || m_iBestHitbox == -1 )
		return;

	if ( m_vAimBestHitboxScreen.x <= 0.f || m_vAimBestHitboxScreen.y <= 0.f )
		return;

	CPlayer* pTargetPlayer = g_pPlayers->GetPlayer( m_iBestTarget );

	if ( Settings::Aimbot::aim_DrawSpot )
	{
		g_pRender->DrawFillBox( m_vAimBestHitboxScreen.x , m_vAimBestHitboxScreen.y ,
								2 , 2 , g_pEsp->GetPlayerColor( pTargetPlayer ) );
	}

	if ( Settings::Aimbot::aim_DrawFov )
	{
		int iPlayerFov = GetPlayerFov( pTargetPlayer );

		Color TargetFovColor = g_pEsp->GetPlayerColor( pTargetPlayer );

		if ( Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_FovType <= 0 )
		{
			g_pRender->DrawBox( m_vAimBestHitboxScreen.x - iPlayerFov , m_vAimBestHitboxScreen.y - iPlayerFov ,
								iPlayerFov * 2 , iPlayerFov * 2 , TargetFovColor );
		}
		else
		{
			g_pRender->DrawBox( g_vCenterScreen.x - iPlayerFov , g_vCenterScreen.y - iPlayerFov , iPlayerFov * 2 , iPlayerFov * 2 , TargetFovColor );
		}
	}
}

void CAimbot::Aimbot()
{
	m_bAimShot = false;
	m_bTargetFov = false;

	if ( !IsEnable() )
		return;

	m_bAttack = ( m_pCmd->buttons & IN_ATTACK );

	if ( !g_pPlayers->GetPlayer( m_iBestTarget )->bUpdate )
	{
		m_iBestPreTarget = -1;
		m_iBestTarget = -1;
		m_iBestHitbox = -1;
	}

	m_iBestPreTarget = m_iBestTarget;
	m_iBestTarget = GetBestTarget();

	if ( m_bAttack )
	{
		if ( Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_AutoPistol && m_pLocal->WeaponType == WEAPON_TYPE_PISTOL && !m_bAutoPistolEn )
		{
			AutoPistol();
		}
	}

	if ( m_iBestTarget == -1 )
		return;

	if ( m_iBestPreTarget != m_iBestTarget && m_bAttack )
	{
		m_bTargetChange = true;
	}
	else
	{
		m_bTargetChange = false;
	}

	m_iBestHitbox = GetBestHitBox();

	if ( m_iBestHitbox == -1 )
		return;

	CPlayer* pPreTargetPlayer = g_pPlayers->GetPlayer( m_iBestPreTarget );
	CPlayer* pTargetPlayer = g_pPlayers->GetPlayer( m_iBestTarget );

	int iPlayerFov = GetPlayerFov( pTargetPlayer );

	if ( CheckPlayerFov( m_vAimBestHitboxScreen , iPlayerFov ) )
		m_bTargetFov = true;

	static bool bLockDelay = false;
	static bool bLockAttack = false;

	if ( !m_bAttack )
	{
		m_bClamp = true;
		bLockDelay = false;
		bLockAttack = false;
		m_pShotDelay->reset();
	}

	if ( Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_AutoPistol && m_pLocal->WeaponType == WEAPON_TYPE_PISTOL )
	{
		if ( m_bTargetFov && !m_bAttack )
		{
			m_bAutoPistolEn = true;
		}
		else if ( !m_bTargetFov && !m_bAttack )
		{
			m_bAutoPistolEn = false;
		}
		else if ( !m_bTargetFov && m_bAttack )
		{
			m_bAutoPistolEn = false;
		}
	}

	if ( !Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_AutoPistol && m_pLocal->WeaponType == WEAPON_TYPE_PISTOL )
	{
		if ( m_pLocal->iShotsFired == 1 && bLockDelay )
			return;
	}

	AIM_ATTACK_TYPE AttackType = AA_SHOTGUN;

	if ( m_pLocal->WeaponType == WEAPON_TYPE_SNIPER || m_pLocal->WeaponType == WEAPON_TYPE_PISTOL )
		AttackType = AA_SNIPER_PISTOL;

	static bool bAimAttack = false;
	static bool bAimCheckAttack = false;

	if ( AttackType == AA_SHOTGUN )
	{
		if ( m_bAttack && m_bTargetFov )
		{
			bAimCheckAttack = true;
		}
		else
		{
			bAimCheckAttack = false;
		}
	}
	else if ( AttackType == AA_SNIPER_PISTOL )
	{
		if ( !Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_AutoPistol )
		{
			if ( m_bTargetFov && !m_bAttack )
			{
				bAimAttack = true;
			}
			else if ( !m_bTargetFov && m_bAttack )
			{
				bAimAttack = false;
			}
			else if ( !m_bTargetFov && !m_bAttack )
			{
				bAimAttack = false;
			}
			
			if ( m_bAttack && bAimAttack && m_bTargetFov )
			{
				bAimCheckAttack = true;
			}
			else
			{
				bAimCheckAttack = false;
			}
		}
		else
		{
			if ( m_bAttack && m_bTargetFov )
			{
				bAimCheckAttack = true;
			}
			else
			{
				bAimCheckAttack = false;
				m_bClamp = true;
			}
		}
	}

	if ( m_pLocal->WeaponType == WEAPON_TYPE_SNIPER || m_pLocal->WeaponType == WEAPON_TYPE_PISTOL )
		Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_RcsClampType = 2;

	if ( Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_RcsClampType >= 1 && m_pLocal->WeaponType == WEAPON_TYPE_SHOTGUN )
	{
		if ( !bLockAttack )
		{
			if ( m_bTargetFov && !m_bAttack )
			{
				bAimAttack = true;
			}
			else if ( !m_bTargetFov && m_bAttack )
			{
				bAimAttack = false;
			}
			else if ( !m_bTargetFov && !m_bAttack )
			{
				bAimAttack = false;
			}
		}

		if ( m_bAttack && bAimAttack && m_bTargetFov && !bLockAttack )
		{
			bAimCheckAttack = true;
			bLockAttack = true;
		}
		else if ( m_bAttack && m_bTargetFov && bLockAttack )
		{
			bAimCheckAttack = true;
			bLockAttack = true;
		}
		else
			bAimCheckAttack = false;
	}

	if ( Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_RcsClampType >= 2 )
	{
		if ( m_bClamp && m_bTargetChange )
			m_bClamp = false;
	}

	if ( !( pTargetPlayer->m_pEntity->GetFlags() & FL_ONGROUND ) && Settings::Aimbot::aim_AntiJump )
		return;

	if ( bAimCheckAttack )
	{
		int AimDelay = Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_Delay;

		if ( AimDelay )
		{
			float fDistanceAimScreen = DistanceScreen( g_vCenterScreen , Vector2D( m_vAimBestHitboxScreen.x , m_vAimBestHitboxScreen.y ) );

			if ( m_pShotDelay->delay( AimDelay ) )
				m_bAimShot = true;

			if ( fDistanceAimScreen <= 3.5f && !bLockDelay )
			{
				m_bAimShot = true;
				bLockDelay = true;
			}
			else if ( bLockDelay )
			{
				m_bAimShot = true;
			}
		}
		else
		{
			m_bAimShot = true;
		}

		if ( m_bClamp )
		{
			if ( Settings::Aimbot::aim_CheckSmoke )
			{
				if ( LineGoesThroughSmoke( m_pLocal->vEyeOrigin , m_vAimBestHitbox ) )
					return;
			}

			AimbotSet();
		}

		if ( m_pLocal->WeaponType == WEAPON_TYPE_SHOTGUN || !Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_AutoPistol )
		{
			if ( m_bAimShot )
			{
				m_pCmd->buttons |= IN_ATTACK;
			}
			else
			{
				m_pCmd->buttons &= ~IN_ATTACK;
			}
		}

		if ( m_pLocal->WeaponType == WEAPON_TYPE_PISTOL && Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_AutoPistol && m_bAutoPistolEn )
		{
			if ( m_bAimShot )
			{
				m_pCmd->buttons |= IN_ATTACK;
				m_bAutoPistolEn = false;
			}
			else
				m_pCmd->buttons &= ~IN_ATTACK;
		}
	}
}

void CAimbot::AimbotSet()
{
	if ( !m_vAimBestHitbox.IsValid() )
		return;

	Vector vAimAngle , vSmoothAimAngle;

	VectorAngles( m_vAimBestHitbox - m_pLocal->vEyeOrigin , vAimAngle );

	bool bEnableRcs = false;

	if ( Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_Rcs )
	{
		if ( m_pLocal->WeaponType == WEAPON_TYPE_SHOTGUN && m_pLocal->iShotsFired > 1 )
			bEnableRcs = true;

		if ( m_pLocal->WeaponType == WEAPON_TYPE_PISTOL && m_pLocal->iShotsFired <= 1 )
			bEnableRcs = true;
	}

	if ( bEnableRcs )
	{
		float MulVar = Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_Rcs * 0.02f;

		if ( Settings::Aimbot::aim_RcsType <= 0 )
		{
			vAimAngle -= m_pLocal->vAimPunch * MulVar;
		}
		else if ( Settings::Aimbot::aim_RcsType == 1 )
		{
			vAimAngle -= ( m_pLocal->vPunch + m_pLocal->vAimPunch ) * MulVar;
		}
		else if ( Settings::Aimbot::aim_RcsType >= 2 )
		{
			vAimAngle -= ( m_pLocal->vPunch + ( m_pLocal->vAimPunch * 2 * 0.5f ) ) * MulVar;
		}
	}

	float fSmooth = 10.f;

	if ( bEnableRcs && m_pLocal->WeaponType == WEAPON_TYPE_SHOTGUN )
	{
		fSmooth = Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_RcsSmooth;
	}
	else
	{
		fSmooth = Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_Smooth;
	}

	AngleNormalize( vAimAngle );

	if ( fSmooth < 10.f )
		fSmooth = 10.f;

	fSmooth = fSmooth / 10.f;

	SmoothAngles( m_pCmd->viewangles , vAimAngle , vSmoothAimAngle , fSmooth );

	m_pCmd->viewangles = vSmoothAimAngle;
}

void CAimbot::AutoPistol()
{
	float NextAttack = m_pLocal->m_pWeaponEntity->GetNextPrimaryAttack();
	float Tick = m_pLocal->m_pEntity->GetTickBase() * Interfaces::GlobalVars()->interval_per_tick;

	if ( NextAttack < Tick )
		return;

	if ( *m_pLocal->m_pWeaponEntity->GeteAttributableItem()->GetItemDefinitionIndex() == WEAPON_REVOLVER )
	{
		m_pCmd->buttons &= ~IN_ATTACK2;
	}
	else
	{
		m_pCmd->buttons &= ~IN_ATTACK;
	}
}

void CAimbot::OnCreateMove( CUserCmd * pCmd , CMe * pLocal )
{
	m_pLocal = pLocal;
	m_pCmd = pCmd;

	Aimbot();
}

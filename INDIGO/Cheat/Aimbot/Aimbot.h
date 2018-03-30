#pragma once

#include "../../Engine/Engine.h"

#define MAXBACKTRACKTICKS ((int)Settings::Aimbot::aim_Backtracktickrate)
#define TICK_INTERVAL			(Interfaces::GlobalVars()->interval_per_tick)
#define TIME_TO_TICKS( dt )		( (int)( 0.5f + (float)(dt) / TICK_INTERVAL ) )

class CTimer;

namespace Engine
{
	class CMe;
	class CPlayer;
}

using namespace Engine;

enum AIM_ATTACK_TYPE
{
	AA_SHOTGUN ,
	AA_SNIPER_PISTOL
};

class CAimbot
{
public:

	CAimbot();
//[swap_lines]
	bool	IsEnable();

	int		GetPlayerFov( CPlayer* pPlayer );

	int		GetBestTarget();
	int		GetBestHitBox();

	void	OnRender();

	void	Aimbot();
	void	AimbotSet();

	void	AutoPistol();
	void	OnCreateMove( CUserCmd* pCmd , CMe* pLocal );
//[/swap_lines]
private:

	CMe*		m_pLocal;
	CUserCmd*	m_pCmd;

	int		m_iBestPreTarget;

	int		m_iBestTarget;
	int		m_iBestHitbox;

	bool	m_bClamp;
	bool	m_bAttack;
	bool	m_bAimShot;
	bool	m_bTargetFov;
	bool	m_bTargetChange;
	bool	m_bAutoPistolEn;

	CTimer*	m_pShotDelay;

	Vector	m_vAimBestHitbox;
	Vector  m_vAimBestHitboxScreen;
};
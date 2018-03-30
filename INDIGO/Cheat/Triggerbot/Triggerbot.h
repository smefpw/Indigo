#pragma once

#include "../../Engine/Engine.h"

#define KEY_TRIGGER_DOWN(KeyNum) ( GetAsyncKeyState(KeyNum) & 0x1 )

class CTimer;

namespace Engine
{
	class CMe;
	class CPlayer;
}

using namespace Engine;

class CTriggerbot
{
public:

	CTriggerbot();
//[swap_lines]
	void LeftClick();
	void RightClick();

	bool IsEnable();

	void TriggerShow( CPlayer* pPlayer );
	void TriggerShowFov( CPlayer* pPlayer );
	void TriggerShowAssistFov( CPlayer* pPlayer );

	int GetTriggerHeadFov( CPlayer* pPlayer );
	int GetTriggerChestFovX( CPlayer* pPlayer );
	int GetTriggerChestFovY( CPlayer* pPlayer );

	int GetTriggerAssistHeadFov( CPlayer* pPlayer );
	int GetTriggerAssistChestFovX( CPlayer* pPlayer );
	int GetTriggerAssistChestFovY( CPlayer* pPlayer );

	void TriggerShowStatus();
	bool TriggerCheckTeam( CPlayer* pPlayer );

	bool CheckPlayerTriggerLine( CPlayer* pPlayer );
	byte GetCountPlayerAssistFov();
	bool CheckPlayerTriggerPos();
	bool CheckPlayerAssistTriggerPos();

	void TriggerAssistSet( Vector vAimAssist );
	void TriggerAssistUpd( Vector vAimAssist );
	void TriggerAssist( CPlayer* pPlayer );

	void TriggerAttack();
	void TriggerBot();

	void OnCreateMove( CUserCmd* pCmd , CMe* pLocal );
//[/swap_lines]
private:

	CMe*		m_pLocal;
	CUserCmd*	m_pCmd;

	int			iPreFov;
	int			iTriggerShotsFired;

public:

	bool		bTrigger;
	bool		bAssist;
	bool		bFovTime;
	bool		bIsZoomSniper;
	bool		bTriggerAttack;

private:

	ULONGLONG	dwTriggerStatusTime;

	CTimer*		m_pTriggerDelayBefore;
	CTimer*		m_pTriggerDelayAfter;
};
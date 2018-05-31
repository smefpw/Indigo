#include "AntiAim.h"

void AntiAim::LegitAA(CUserCmd* cmd, bool& bSendPacket) 
{
	CMe* local = Client::g_pPlayers->GetLocal();
	if (!cmd || !local || !local->bAlive || (cmd->buttons & IN_USE) || (cmd->buttons & IN_ATTACK) || (cmd->buttons & IN_ATTACK2) || local->m_pEntity->GetMoveType() == MOVETYPE_LADDER || (local->WeaponType > WEAPON_TYPE_SNIPER))
		return;

	static int ChokedTicks = 0;
	static bool Direction = false;
	if (GetAsyncKeyState(VK_LEFT)) Direction = true;
	if (GetAsyncKeyState(VK_RIGHT)) Direction = false;

	if (ChokedTicks < 1) {
		bSendPacket = false;
		cmd->viewangles.y += Direction ? 90 : -90;

		ChokedTicks++;
	}
	else {
		bSendPacket = true;
		ChokedTicks = 0;
	}
}

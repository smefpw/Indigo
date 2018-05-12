#include "LagComp.h"
using namespace SDK;

int BacktrackTicks() 
{
	int ret = ((float)Settings::Aimbot::aim_Backtracktime / 1000) / Interfaces::GlobalVars()->interval_per_tick; // pretty b1g yes?
	return (ret < 1 ? 1 : ret);
}

void BackTrack::legitBackTrack(CUserCmd* cmd)
{
	int bestTargetIndex = -1;
	float bestFov = FLT_MAX;
	CBaseEntity* pLocal = (CBaseEntity*)Interfaces::EntityList()->GetClientEntity(Interfaces::Engine()->GetLocalPlayer());
	PlayerInfo info;
	if (pLocal->IsDead())
		return;

	for (int i = 0; i < Interfaces::Engine()->GetMaxClients(); i++)
	{
		auto entity = (CBaseEntity*)Interfaces::EntityList()->GetClientEntity(i);

		if (!entity || !pLocal || (entity == pLocal) || (!Interfaces::Engine()->GetPlayerInfo(i, &info)) || (entity->IsDormant()) || (entity->GetTeam() == pLocal->GetTeam()))		
			continue;

		if (!entity->IsDead())
		{
			float simtime = entity->GetSimTime();
			Vector hitboxPos = entity->GetHitboxPosition(0);

			headPositions[i][cmd->command_number % (BacktrackTicks() + 1)] = backtrackData{ simtime, hitboxPos };
			Vector ViewDir = angle_vector(cmd->viewangles + (pLocal->GetAimPunchAngle() * 2.f));
			float FOVDistance = distance_point_to_line(hitboxPos, pLocal->GetEyePosition(), ViewDir);

			if (bestFov > FOVDistance)
			{
				bestFov = FOVDistance;
				bestTargetIndex = i;
			}
		}
	}

	float bestTargetSimTime;
	if (bestTargetIndex != -1)
	{
		float tempFloat = FLT_MAX;
		Vector ViewDir = angle_vector(cmd->viewangles + (pLocal->GetAimPunchAngle() * 2.f));
		for (int t = 0; t < BacktrackTicks(); ++t)
		{
			float tempFOVDistance = distance_point_to_line(headPositions[bestTargetIndex][t].hitboxPos, pLocal->GetEyePosition(), ViewDir);
			if (tempFloat > tempFOVDistance && headPositions[bestTargetIndex][t].simtime > pLocal->GetSimTime() - 1)
			{
				tempFloat = tempFOVDistance;
				bestTargetSimTime = headPositions[bestTargetIndex][t].simtime;
			}
		}

		cmd->tick_count = TIME_TO_TICKS(bestTargetSimTime);
	}
}

BackTrack* backtracking = new BackTrack();
backtrackData headPositions[64][25]; //support for 128tick servers

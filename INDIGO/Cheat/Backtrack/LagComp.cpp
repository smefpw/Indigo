#include "LagComp.h"
using namespace SDK;
void BackTrack::legitBackTrack(CUserCmd* cmd)
{
	if (Settings::Aimbot::aim_Backtrack)
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

			if (!entity || !pLocal || entity == pLocal || entity->IsDormant() || entity->GetTeam() == pLocal->GetTeam() || !Interfaces::Engine()->GetPlayerInfo(i, &info))
				continue;

			if (!entity->IsDead())
			{
				float simtime = entity->GetSimTime();
				Vector hitboxPos = entity->GetHitboxPosition(0);

				headPositions[i][cmd->command_number % (TIME_TO_TICKS(Settings::Aimbot::aim_Backtracktime / 1000) + 1)] = backtrackData{ simtime, hitboxPos };
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
			for (int t = 0; t < TIME_TO_TICKS(Settings::Aimbot::aim_Backtracktime / 1000); ++t)
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
}

BackTrack* backtracking = new BackTrack();
backtrackData headPositions[64][25]; //support for 128tick servers

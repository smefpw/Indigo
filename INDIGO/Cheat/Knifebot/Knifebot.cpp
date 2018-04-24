#include "Knifebot.h"

using namespace Client;
//[junk_enable /]
//[enc_string_enable /]
void CKnifebot::OnCreateMove(CUserCmd * pCmd)
{
	if (Settings::Knifebot::knf_Active && g_pPlayers->GetLocal()->WeaponName.find("Knife") != string::npos)
	{
		for (BYTE PlayerIndex = 0; PlayerIndex < g_pPlayers->GetSize(); PlayerIndex++)
		{
			CPlayer* pPlayer = g_pPlayers->GetPlayer(PlayerIndex);

			if (pPlayer && pPlayer->m_pEntity && pPlayer->bUpdate)
			{
				if (!Settings::Knifebot::knf_Team && g_pPlayers->GetLocal()->Team == pPlayer->Team)
					continue;

				bool bOriginScreen = (pPlayer->vOriginScreen.x > 0 && pPlayer->vOriginScreen.y > 0);
				bool bHitBoxScreen = (pPlayer->vHitboxHeadScreen.x > 0 && pPlayer->vHitboxHeadScreen.y > 0);

				if (bOriginScreen || bHitBoxScreen)
				{
					int MinDistance = 0;
					//[junk_disable /]
					switch (Settings::Knifebot::knf_Attack)
					{
					case 0:
						MinDistance = Settings::Knifebot::knf_DistAttack;
						break;
					case 1:
						MinDistance = Settings::Knifebot::knf_DistAttack2;
						break;
					case 2:
						MinDistance = Settings::Knifebot::knf_DistAttack;
						break;
					}
					//[junk_enable /]
					int DistanceToPlayer = pPlayer->iDistance;

					if (DistanceToPlayer > MinDistance)
						continue;
					//[junk_disable /]
					switch (Settings::Knifebot::knf_Attack)
					{
					case 0:
						pCmd->buttons |= IN_ATTACK;
						break;
						return;
					case 1:
						pCmd->buttons |= IN_ATTACK2;
						break;
						return;
					case 2:
						if (DistanceToPlayer > Settings::Knifebot::knf_DistAttack2)
							pCmd->buttons |= IN_ATTACK;
						else
							pCmd->buttons |= IN_ATTACK2;
						break;
						return;
					}
					//[junk_enable /]
				}
			}
		}
	}
}


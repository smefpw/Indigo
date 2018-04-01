#include "InventoryChanger.h"
#include <vector>

void CInventoryChanger::PostRetrieveMessage(uint32_t* punMsgType, void* pubDest, uint32_t cubDest, uint32_t* pcubMsgSize)
{
	uint32_t MessageType = *punMsgType & 0x7FFFFFFF;
	if (MessageType == k_EMsgGCCStrike15_v2_MatchmakingGC2ClientHello && Settings::ProfileChanger::enabled) {
		CMsgGCCStrike15_v2_MatchmakingGC2ClientHello Message;
		try
		{
			if (!Message.ParsePartialFromArray((void*)((DWORD)pubDest + 8), *pcubMsgSize - 8))
				return;
		}
		catch (...)
		{
			return;
		}
		if (Settings::ProfileChanger::level != 0) {
			Message.set_player_level(Settings::ProfileChanger::level);
		}
		if (Settings::ProfileChanger::xp != 0) {
			Message.set_player_cur_xp(Settings::ProfileChanger::xp);
		}
		PlayerRankingInfo* Ranking = Message.mutable_ranking();
		Ranking->set_account_id(Interfaces::SteamUser()->GetSteamID().GetAccountID());
		if (Settings::ProfileChanger::rank_id != 0) {
			Ranking->set_rank_id(Settings::ProfileChanger::rank_id);
		}
		if (Settings::ProfileChanger::wins != 0) {
			Ranking->set_wins(Settings::ProfileChanger::wins);
		}
		PlayerCommendationInfo* Commendation = Message.mutable_commendation();
		if (Settings::ProfileChanger::cmd_friendly != 0) {
			Commendation->set_cmd_friendly(Settings::ProfileChanger::cmd_friendly);
		}
		if (Settings::ProfileChanger::cmd_leader != 0) {
			Commendation->set_cmd_leader(Settings::ProfileChanger::cmd_leader);
		}
		if (Settings::ProfileChanger::cmd_teaching != 0) {
			Commendation->set_cmd_teaching(Settings::ProfileChanger::cmd_teaching);
		}
		if ((uint32_t)Message.ByteSize() <= cubDest - 8)
		{
			Message.SerializeToArray((void*)((DWORD)pubDest + 8), Message.ByteSize());
			*pcubMsgSize = Message.ByteSize() + 8;
		}
	}
	if (MessageType == k_EMsgGCClientWelcome) {
		CMsgClientWelcome Message;
		try
		{
			if (!Message.ParsePartialFromArray((void*)((DWORD)pubDest + 8), *pcubMsgSize - 8))
				return;
		}
		catch (...)
		{
			return;
		}
		if (Message.outofdate_subscribed_caches_size() <= 0)
			return;
		CMsgSOCacheSubscribed* Cache = Message.mutable_outofdate_subscribed_caches(0);
		for (int i = 0; i < Cache->objects_size(); i++)
		{
			CMsgSOCacheSubscribed::SubscribedType* Object = Cache->mutable_objects(i);
			if (!Object->has_type_id())
				continue;
			if (Object->type_id() == 1) {
				for (int j = 0; j < Object->object_data_size(); j++)
				{
					std::string* ObjectData = Object->mutable_object_data(j);
					CSOEconItem Item;
					if (!Item.ParseFromArray((void*)const_cast<char*>(ObjectData->data()), ObjectData->size()))
						continue;
					if (Item.equipped_state_size() <= 0)
						continue;
					if (Settings::MedalChanger::equipped_medal_override)
					{
						CSOEconItemEquipped* EquippedState = Item.mutable_equipped_state(0);
						if (EquippedState->new_class() == 0 && EquippedState->new_slot() == 55)
						{
							Item.clear_equipped_state();
							ObjectData->resize(Item.ByteSize());
							Item.SerializeToArray((void*)const_cast<char*>(ObjectData->data()), Item.ByteSize());
						}
					}
				}
				ApplyMedals(Object);
				ApplySkins(Object);
			}
		}
		if ((uint32_t)Message.ByteSize() <= cubDest - 8)
		{
			Message.SerializeToArray((void*)((DWORD)pubDest + 8), Message.ByteSize());
			*pcubMsgSize = Message.ByteSize() + 8;
		}
	}
}
bool CInventoryChanger::PreSendMessage(uint32_t &unMsgType, void* pubData, uint32_t &cubData)
{
	if (!Settings::InventoryChanger::enabled)
		return true;
	uint32_t MessageType = unMsgType & 0x7FFFFFFF;
	if (MessageType == k_EMsgGCAdjustItemEquippedState) {
		CMsgAdjustItemEquippedState Message;
		try
		{
			if (!Message.ParsePartialFromArray((void*)((DWORD)pubData + 8), cubData - 8))
				return true;
		}
		catch (...)
		{
			return true;
		}
		if (!Message.has_item_id() || !Message.has_new_class() || !Message.has_new_slot())
			return true;
		uint32_t item_id = (uint32_t)Message.item_id() - 20000;
		if (item_id < 1 || item_id > Settings::InventoryChanger::weapons.size()) {
			return true;
		}
		auto weapon = Settings::InventoryChanger::weapons[item_id - 1];
		g_SkinChangerCfg[weapon.itemDefinitionIndex].flFallbackWear = weapon.wear;
		g_SkinChangerCfg[weapon.itemDefinitionIndex].iItemDefinitionIndex = weapon.itemDefinitionIndex;
		g_SkinChangerCfg[weapon.itemDefinitionIndex].nFallbackPaintKit = weapon.paintKit;
		ForceFullUpdate();
		return false;
	}
	return true;
}
void CInventoryChanger::ApplySkins(CMsgSOCacheSubscribed::SubscribedType* pInventoryCacheObject)
{
	if (!Settings::InventoryChanger::enabled) {
		return;
	}
	int c = 20001;
	for (auto weapon : Settings::InventoryChanger::weapons) {
		CSOEconItem Skin;
		Skin.set_id(c);
		Skin.set_account_id(Interfaces::SteamUser()->GetSteamID().GetAccountID());
		Skin.set_def_index(weapon.itemDefinitionIndex);
		Skin.set_inventory(c);
		Skin.set_origin(24);
		Skin.set_quantity(1);
		Skin.set_level(1);
		Skin.set_style(0);
		Skin.set_flags(0);
		Skin.set_in_use(false);
		Skin.set_original_id(0);
		Skin.set_rarity(weapon.rarity);
		Skin.set_quality(0);
		auto PaintKitAttribute = Skin.add_attribute();
		float PaintKitAttributeValue = (float)weapon.paintKit;
		PaintKitAttribute->set_def_index(6);
		PaintKitAttribute->set_value_bytes(&PaintKitAttributeValue, 4);
		auto SeedAttribute = Skin.add_attribute();
		float SeedAttributeValue = (float)weapon.seed;
		SeedAttribute->set_def_index(7);
		SeedAttribute->set_value_bytes(&SeedAttributeValue, 4);
		auto WearAttribute = Skin.add_attribute();
		float WearAttributeValue = weapon.wear;
		WearAttribute->set_def_index(8);
		WearAttribute->set_value_bytes(&WearAttributeValue, 4);
		pInventoryCacheObject->add_object_data(Skin.SerializeAsString());
		c++;
	}
}
void CInventoryChanger::ApplyMedals(CMsgSOCacheSubscribed::SubscribedType* pInventoryCacheObject)
{
	if (!Settings::MedalChanger::enabled) {
		return;
	}
	int c = 10001;
	for (uint32_t MedalIndex : Settings::MedalChanger::medals)
	{
		CSOEconItem Medal;
		Medal.set_account_id(Interfaces::SteamUser()->GetSteamID().GetAccountID());
		Medal.set_origin(9);
		Medal.set_rarity(6);
		Medal.set_quantity(1);
		Medal.set_quality(4);
		Medal.set_level(1);
		CSOEconItemAttribute* TimeAcquiredAttribute = Medal.add_attribute();
		uint32_t TimeAcquiredAttributeValue = 0;
		TimeAcquiredAttribute->set_def_index(222);
		TimeAcquiredAttribute->set_value_bytes(&TimeAcquiredAttributeValue, 4);
		Medal.set_def_index(MedalIndex);
		Medal.set_inventory(c);
		Medal.set_id(c);
		if (Settings::MedalChanger::equipped_medal_override && Settings::MedalChanger::equipped_medal == MedalIndex)
		{
			CSOEconItemEquipped* EquippedState = Medal.add_equipped_state();
			EquippedState->set_new_class(0);
			EquippedState->set_new_slot(55);
		}
		pInventoryCacheObject->add_object_data(Medal.SerializeAsString());
		c++;
	}
}
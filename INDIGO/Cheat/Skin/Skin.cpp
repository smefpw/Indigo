/*
	WARNING: This cheat (like pretty much any other pastes out there) uses Virtual Method Table (VMT) hooking, which is now detected by Valve Anti-Cheat.
	We are NOT responsible for ANY bans that may occur during the process of using this cheat. This includes, but not limited to, VAC, Untrusted and
	Overwatch bans.
*/

#include "Skin.h"
#include <algorithm>
#include <functional>
unordered_map<int, EconomyItemCfg> g_SkinChangerCfg;
unordered_map<int, const char*> g_ViewModelCfg;
unordered_map<const char*, const char*> g_KillIconCfg;

char* pWeaponData[33] =
{
	// ��������� - 0 - 9
	WEAPON_DEAGLE_STR,WEAPON_ELITE_STR,WEAPON_FIVESEVEN_STR,
	WEAPON_GLOCK_STR,WEAPON_HKP2000_STR,WEAPON_P250_STR,
	WEAPON_USP_S_STR,WEAPON_CZ75A_STR,WEAPON_REVOLVER_STR,
	WEAPON_TEC9_STR,
	// �������� - 10 - 30
	WEAPON_AK47_STR,WEAPON_AUG_STR,WEAPON_FAMAS_STR,WEAPON_GALILAR_STR,
	WEAPON_M249_STR,WEAPON_M4A1_STR,WEAPON_M4A1_S_STR,WEAPON_MAC10_STR,
	WEAPON_P90_STR,WEAPON_UMP45_STR,WEAPON_XM1014_STR,WEAPON_BIZON_STR,
	WEAPON_MAG7_STR,WEAPON_NEGEV_STR,WEAPON_SAWEDOFF_STR,
	WEAPON_MP7_STR,WEAPON_MP9_STR,WEAPON_NOVA_STR,WEAPON_SG553_STR,
	WEAPON_SCAR20_STR,WEAPON_G3SG1_STR,
	// ���������  - 31 - 32
	WEAPON_AWP_STR,WEAPON_SSG08_STR
};

int pWeaponItemIndexData[33] =
{
	// ��������� - 0 - 9
	WEAPON_DEAGLE,WEAPON_ELITE,WEAPON_FIVESEVEN,
	WEAPON_GLOCK,WEAPON_HKP2000,WEAPON_P250,
	WEAPON_USP_SILENCER,WEAPON_CZ75A,WEAPON_REVOLVER,
	WEAPON_TEC9,
	// �������� - 10 - 30
	WEAPON_AK47,WEAPON_AUG,WEAPON_FAMAS,WEAPON_GALILAR,
	WEAPON_M249,WEAPON_M4A1,WEAPON_M4A1_SILENCER,WEAPON_MAC10,
	WEAPON_P90,WEAPON_UMP45,WEAPON_XM1014,WEAPON_BIZON,
	WEAPON_MAG7,WEAPON_NEGEV,WEAPON_SAWEDOFF,
	WEAPON_MP7,WEAPON_MP9,WEAPON_NOVA,WEAPON_SG553,
	WEAPON_SCAR20,WEAPON_G3SG1,
	// ���������  - 31 - 32
	WEAPON_AWP,WEAPON_SSG08
};

char* pKnifeData[10] =
{
	"m9_bayonet","knife_flip","knife_gut","knife_karambit" ,"knife_m9_bayonet",
	"knife_tactical","knife_falchion","knife_survival_bowie","knife_butterfly","knife_push"
};

//[enc_string_enable /]
const char* bloodhound = "models/weapons/v_models/arms/glove_bloodhound/v_glove_bloodhound.mdl";
const char* handwrap = "models/weapons/v_models/arms/glove_handwrap_leathery/v_glove_handwrap_leathery.mdl";
const char* slick = "models/weapons/v_models/arms/glove_slick/v_glove_slick.mdl";
const char* sporty = "models/weapons/v_models/arms/glove_sporty/v_glove_sporty.mdl";
const char* motorcycle = "models/weapons/v_models/arms/glove_motorcycle/v_glove_motorcycle.mdl";
const char* specialist = "models/weapons/v_models/arms/glove_specialist/v_glove_specialist.mdl";
const char* hydra = "models / weapons / v_models / arms / glove_bloodhound / v_glove_bloodhound_hydra.mdl";
//[enc_string_disable /]

WeaponSkins_s WeaponSkins[33];
KnifeSkins_s KnifeSkins[10];
Gloves_s GloveSkin[10];
//[junk_enable /]
//[enc_string_enable /]
RecvVarProxyFn fnSequenceProxyFn = NULL;

using namespace Client;

int GetWeaponSkinIndexFromPaintKit(int iPaintKit)
{
	for (size_t iSkinID = 0; iSkinID < WeaponSkins[iWeaponID].SkinPaintKit.size(); iSkinID++)
	{
		if (WeaponSkins[iWeaponID].SkinPaintKit[iSkinID] == iPaintKit)
			return iSkinID;
	}

	return 0;
}

int GetKnifeSkinIndexFromPaintKit(int iPaintKit, bool tt)
{
	int iKnifeModelID = (tt ? Settings::Skin::knf_tt_model - 1 : Settings::Skin::knf_ct_model - 1);

	for (size_t iSkinID = 0; iSkinID < KnifeSkins[iKnifeModelID].SkinPaintKit.size(); iSkinID++)
	{
		if (KnifeSkins[iKnifeModelID].SkinPaintKit[iSkinID] == iPaintKit)
			return iSkinID;
	}

	return 0;
}
void CSkin::OnEvents(IGameEvent* pEvent)
{
	const char* szEventName = pEvent->GetName();

	if (!strcmp(szEventName, "player_death"))
		ApplyCustomKillIcon(pEvent);

	if (!strcmp(szEventName, "game_newmap"))
	{
		if (g_ViewModelCfg.size() >= 1)
			g_ViewModelCfg.clear();

		SetSkinConfig();
		SetModelConfig();
		SetKillIconCfg();
	}
}

void CSkin::SetSkinConfig()
{
	int KnifeModelsType[10] =
	{
		WEAPON_KNIFE_BAYONET,WEAPON_KNIFE_FLIP,WEAPON_KNIFE_GUT,WEAPON_KNIFE_KARAMBIT,
		WEAPON_KNIFE_M9_BAYONET,WEAPON_KNIFE_TACTICAL,WEAPON_KNIFE_FALCHION,
		WEAPON_KNIFE_SURVIVAL_BOWIE,WEAPON_KNIFE_BUTTERFLY,WEAPON_KNIFE_PUSH
	};

	if (Settings::Skin::knf_ct_model >= 1 && Settings::Skin::knf_ct_model <= 10)
	{
		g_SkinChangerCfg[WEAPON_KNIFE].iItemDefinitionIndex = KnifeModelsType[Settings::Skin::knf_ct_model - 1];
	}
	else
	{
		g_SkinChangerCfg[WEAPON_KNIFE].iItemDefinitionIndex = WEAPON_KNIFE;
	}

	if (Settings::Skin::knf_ct_skin)
	{
		g_SkinChangerCfg[WEAPON_KNIFE].nFallbackPaintKit = Settings::Skin::knf_ct_skin;
	}
	else
	{
		g_SkinChangerCfg[WEAPON_KNIFE].nFallbackPaintKit = 0;
	}

	if (Settings::Skin::knf_tt_model >= 1 && Settings::Skin::knf_tt_model <= 10)
	{
		g_SkinChangerCfg[WEAPON_KNIFE_T].iItemDefinitionIndex = KnifeModelsType[Settings::Skin::knf_tt_model - 1];
	}
	else
	{
		g_SkinChangerCfg[WEAPON_KNIFE_T].iItemDefinitionIndex = WEAPON_KNIFE_T;
	}

	if (Settings::Skin::knf_tt_skin)
	{
		g_SkinChangerCfg[WEAPON_KNIFE_T].nFallbackPaintKit = Settings::Skin::knf_tt_skin;
	}
	else
	{
		g_SkinChangerCfg[WEAPON_KNIFE_T].nFallbackPaintKit = 0;
	}
}

void CSkin::SetModelConfig()
{
	char* pszDefaultCtModel = "models/weapons/v_knife_default_ct.mdl";
	char* pszDefaultTtModel = "models/weapons/v_knife_default_t.mdl";

	char* pszKnifeBayonet = "models/weapons/v_knife_bayonet.mdl";
	char* pszKnifeFlip = "models/weapons/v_knife_flip.mdl";
	char* pszKnifeGut = "models/weapons/v_knife_gut.mdl";
	char* pszKnifeKarambit = "models/weapons/v_knife_karam.mdl";
	char* pszKnifeM9Bay = "models/weapons/v_knife_m9_bay.mdl";
	char* pszKnifeHuntsman = "models/weapons/v_knife_tactical.mdl";
	char* pszKnifeFalchion = "models/weapons/v_knife_falchion_advanced.mdl";
	char* pszKnifeBowie = "models/weapons/v_knife_survival_bowie.mdl";
	char* pszKnifeButterfly = "models/weapons/v_knife_butterfly.mdl";
	char* pszKnifeShadow = "models/weapons/v_knife_push.mdl";

	char* pszKnifeModels[10] =
	{
		pszKnifeBayonet,pszKnifeFlip,pszKnifeGut,pszKnifeKarambit,
		pszKnifeM9Bay,pszKnifeHuntsman,pszKnifeFalchion,pszKnifeBowie,
		pszKnifeButterfly,pszKnifeShadow
	};

	int nOriginalKnifeCT = Interfaces::ModelInfo()->GetModelIndex(pszDefaultCtModel);
	int nOriginalKnifeT = Interfaces::ModelInfo()->GetModelIndex(pszDefaultTtModel);

	if (Settings::Skin::knf_ct_model >= 1 && Settings::Skin::knf_ct_model <= 10)
	{
		char* mdl_ct = pszKnifeModels[Settings::Skin::knf_ct_model - 1];
		g_ViewModelCfg[nOriginalKnifeCT] = mdl_ct;
	}
	else
	{
		g_ViewModelCfg[nOriginalKnifeCT] = pszDefaultCtModel;
	}

	if (Settings::Skin::knf_tt_model >= 1 && Settings::Skin::knf_tt_model <= 10)
	{
		char* mdl_tt = pszKnifeModels[Settings::Skin::knf_tt_model - 1];
		g_ViewModelCfg[nOriginalKnifeT] = mdl_tt;
	}
	else
	{
		g_ViewModelCfg[nOriginalKnifeT] = pszDefaultTtModel;
	}
}

void CSkin::SetKillIconCfg()
{
	//[enc_string_disable /]
	char* pszKnifeModelsIcon[10] =
	{
		"bayonet","knife_flip","knife_gut","knife_karambit",
		"knife_m9_bayonet","knife_tactical","knife_falchion",
		"knife_survival_bowie","knife_butterfly","knife_push"
	};
	//[enc_string_enable /]
	if (Settings::Skin::knf_ct_model >= 1 && Settings::Skin::knf_ct_model <= 10)
	{
		g_KillIconCfg["knife_default_ct"] = pszKnifeModelsIcon[Settings::Skin::knf_ct_model - 1];
	}
	else
	{
		g_KillIconCfg["knife_default_ct"] = "knife_default_ct";
	}
	if (Settings::Skin::knf_tt_model >= 1 && Settings::Skin::knf_tt_model <= 10)
	{
		g_KillIconCfg["knife_t"] = pszKnifeModelsIcon[Settings::Skin::knf_tt_model - 1];
	}
	else
	{
		g_KillIconCfg["knife_t"] = "knife_t";
	}
}

void CSkin::InitalizeSkins()
{
	InitializeKits();

	SetSkinConfig();
	SetKillIconCfg();

	for (ClientClass* pClass = Interfaces::Client()->GetAllClasses(); pClass; pClass = pClass->m_pNext)
	{
		if (!strcmp(pClass->m_pNetworkName, "CBaseViewModel"))
		{
			RecvTable* pClassTable = pClass->m_pRecvTable;

			for (int nIndex = 0; nIndex < pClassTable->m_nProps; nIndex++)
			{
				RecvProp* pProp = &pClassTable->m_pProps[nIndex];

				if (!pProp || strcmp(pProp->m_pVarName, "m_nSequence"))
					continue;

				fnSequenceProxyFn = pProp->m_ProxyFn;
				pProp->m_ProxyFn = (RecvVarProxyFn)Hook_SetViewModelSequence;

				break;
			}

			break;
		}
	}
}

bool CSkin::ApplyCustomSkin(CBaseAttributableItem* pWeapon, int nWeaponIndex)
{
	if (g_SkinChangerCfg.find(nWeaponIndex) == g_SkinChangerCfg.end())
		return false;

	*pWeapon->GetFallbackPaintKit() = g_SkinChangerCfg[nWeaponIndex].nFallbackPaintKit;
	*pWeapon->GetEntityQuality() = g_SkinChangerCfg[nWeaponIndex].iEntityQuality;
	*pWeapon->GetFallbackStatTrak() = g_SkinChangerCfg[nWeaponIndex].nFallbackStatTrak;
	*pWeapon->GetFallbackWear() = g_SkinChangerCfg[nWeaponIndex].flFallbackWear;

	if (g_SkinChangerCfg[nWeaponIndex].iItemDefinitionIndex)
	{
		*pWeapon->GetItemDefinitionIndex() = g_SkinChangerCfg[nWeaponIndex].iItemDefinitionIndex;
	}

	*pWeapon->GetItemIDHigh() = -1;

	return true;
}

bool CSkin::ApplyCustomModel(CBaseEntity* pLocal, CBaseAttributableItem* pWeapon)
{
	CBaseViewModel* pViewModel = pLocal->GetViewModel();

	if (!pViewModel)
		return false;

	DWORD hViewModelWeapon = pViewModel->GetWeapon();

	CBaseAttributableItem* pViewModelWeapon = (CBaseAttributableItem*)Interfaces::EntityList()->GetClientEntityFromHandle((PVOID)hViewModelWeapon);

	if (pViewModelWeapon != pWeapon)
		return false;

	int nViewModelIndex = pViewModel->GetModelIndex();

	if (!nViewModelIndex)
		return false;

	if (g_ViewModelCfg.find(nViewModelIndex) == g_ViewModelCfg.end())
		return false;

	pViewModel->SetModelIndex(Interfaces::ModelInfo()->GetModelIndex(g_ViewModelCfg[nViewModelIndex]));

	return true;
}

bool CSkin::ApplyCustomKillIcon(IGameEvent* pEvent)
{
	int nUserID = pEvent->GetInt("attacker");

	if (!nUserID)
		return false;

	if (Interfaces::Engine()->GetPlayerForUserID(nUserID) != Interfaces::Engine()->GetLocalPlayer())
		return false;

	const char* szWeapon = pEvent->GetString("weapon");

	for (auto ReplacementIcon : g_KillIconCfg)
	{
		if (!strcmp(szWeapon, ReplacementIcon.first))
		{
			pEvent->SetString("weapon", ReplacementIcon.second);
			break;
		}
	}

	return true;
}

bool IsCodePtr(void* ptr)
{
	constexpr const DWORD protect_flags = PAGE_EXECUTE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY;

	MEMORY_BASIC_INFORMATION out;
	VirtualQuery(ptr, &out, sizeof out);

	return out.Type
		&& !(out.Protect & (PAGE_GUARD | PAGE_NOACCESS))
		&& out.Protect & protect_flags;
}

enum class EStickerAttributeType
{
	Index,
	Wear,
	Scale,
	Rotation
};

DWORD dwEconItemInterfaceWrapper = 0x2DB0 + 0xC;

typedef float(__thiscall* GetStickerAttributeBySlotIndexFloatFn)(void*, int, EStickerAttributeType, float);
GetStickerAttributeBySlotIndexFloatFn oGetStickerAttributeBySlotIndexFloat;
// Below is reserved for future use, if we want to add stickers.
/*float __fastcall Hooked_GetStickerAttributeBySlotIndexFloat(void* thisptr, void* edx, int iSlot, EStickerAttributeType iAttribute, float flUnknown)
{
auto pItem = reinterpret_cast<CBaseAttributableItem*>(uintptr_t(thisptr) - dwEconItemInterfaceWrapper);
if (!pItem)
return oGetStickerAttributeBySlotIndexFloat(thisptr, iSlot, iAttribute, flUnknown);

int iID = *pItem->GetItemDefinitionIndex();
if (!Settings::Aimbot::weapon_aim_settings[iWeaponID].StickersEnabled)
return oGetStickerAttributeBySlotIndexFloat(thisptr, iSlot, iAttribute, flUnknown);

switch (iAttribute)
{
case EStickerAttributeType::Wear:
return min(1.f, Settings::Aimbot::weapon_aim_settings[iWeaponID].Stickers[iSlot].flWear + 0.0000000001f);
case EStickerAttributeType::Scale:
return 1.f;
case EStickerAttributeType::Rotation:
return Settings::Aimbot::weapon_aim_settings[iWeaponID].Stickers[iSlot].iRotation;
default:
break;
}
return oGetStickerAttributeBySlotIndexFloat(thisptr, iSlot, iAttribute, flUnknown);
}

typedef UINT(__thiscall* GetStickerAttributeBySlotIndexIntFn)(void*, int, EStickerAttributeType, float);
GetStickerAttributeBySlotIndexIntFn oGetStickerAttributeBySlotIndexInt;

UINT __fastcall Hooked_GetStickerAttributeBySlotIndexInt(void* thisptr, void* edx, int iSlot, EStickerAttributeType iAttribute, UINT iUnknown)
{
auto pItem = reinterpret_cast<CBaseAttributableItem*>(uintptr_t(thisptr) - dwEconItemInterfaceWrapper);
if (!pItem)
return oGetStickerAttributeBySlotIndexInt(thisptr, iSlot, iAttribute, iUnknown);

int iID = *pItem->GetItemDefinitionIndex();
if (!Settings::Aimbot::weapon_aim_settings[iWeaponID].StickersEnabled)
return oGetStickerAttributeBySlotIndexInt(thisptr, iSlot, iAttribute, iUnknown);

if (iAttribute == EStickerAttributeType::Index)
return k_stickers.at(Settings::Aimbot::weapon_aim_settings[iWeaponID].Stickers[iSlot].iID).id;

return oGetStickerAttributeBySlotIndexInt(thisptr, iSlot, iAttribute, iUnknown);
}
void ApplyStickers(CBaseAttributableItem* pItem)
{
void**& vmt = *reinterpret_cast<void***>(uintptr_t(pItem) + dwEconItemInterfaceWrapper);

static void** hooked_vmt = nullptr;
if (!hooked_vmt)
{
size_t size = 0;

while (IsCodePtr(vmt[size]))
++size;

hooked_vmt = new void*[size];
memcpy(hooked_vmt, vmt, size * sizeof(void*));

oGetStickerAttributeBySlotIndexFloat = (GetStickerAttributeBySlotIndexFloatFn)hooked_vmt[4];
hooked_vmt[4] = reinterpret_cast<void*>(&Hooked_GetStickerAttributeBySlotIndexFloat);

oGetStickerAttributeBySlotIndexInt = (GetStickerAttributeBySlotIndexIntFn)hooked_vmt[5];
hooked_vmt[5] = reinterpret_cast<void*>(&Hooked_GetStickerAttributeBySlotIndexInt);
}
vmt = hooked_vmt;
}*/

void Skin_OnFrameStageNotify(ClientFrameStage_t Stage)
{
	if (Stage == ClientFrameStage_t::FRAME_NET_UPDATE_POSTDATAUPDATE_START)
	{
		int nLocalPlayerID = Interfaces::Engine()->GetLocalPlayer();

		CBaseEntity* pLocal = (CBaseEntity*)Interfaces::EntityList()->GetClientEntity(nLocalPlayerID);

		if (!pLocal || pLocal->IsDead())
			return;

		UINT* hWeapons = pLocal->GetWeapons();

		if (!hWeapons)
			return;

		PlayerInfo LocalPlayerInfo;
		Interfaces::Engine()->GetPlayerInfo(nLocalPlayerID, &LocalPlayerInfo);

		for (int nIndex = 0; hWeapons[nIndex]; nIndex++)
		{
			CBaseAttributableItem* pWeapon = (CBaseAttributableItem*)Interfaces::EntityList()->GetClientEntityFromHandle(
				(PVOID)hWeapons[nIndex]);

			CBaseViewModel* pView = pWeapon->GetViewModel();

			if (!pWeapon)
				continue;

			int nWeaponIndex = *pWeapon->GetItemDefinitionIndex();

			if (g_ViewModelCfg.find(pView->GetModelIndex()) != g_ViewModelCfg.end())
				pView->SetModelIndex(Interfaces::ModelInfo()->GetModelIndex(g_ViewModelCfg[pView->GetModelIndex()]));

			if (Client::g_pSkin)
				Client::g_pSkin->ApplyCustomModel(pLocal, pWeapon);

			if (LocalPlayerInfo.m_nXuidLow != *pWeapon->GetOriginalOwnerXuidLow())
				continue;

			if (LocalPlayerInfo.m_nXuidHigh != *pWeapon->GetOriginalOwnerXuidHigh())
				continue;

			if (Client::g_pSkin)
				Client::g_pSkin->ApplyCustomSkin(pWeapon, nWeaponIndex);

			*pWeapon->GetAccountID() = LocalPlayerInfo.m_nXuidLow;
		}
	}
}

void Gloves_OnFrameStageNotify(ClientFrameStage_t Stage)
{
	if (!Settings::Skin::gloves_skin)
		return;

	if (Stage != ClientFrameStage_t::FRAME_NET_UPDATE_POSTDATAUPDATE_START)
		return;

	if (!IsLocalAlive())
		return;

	CBaseEntity* pLocal = g_pPlayers->GetLocal()->m_pEntity;

	UINT* pWeapons = pLocal->GetWeapons();

	if (!pWeapons)
		return;

	UINT* pWareables = pLocal->GetWearables();

	if (!Interfaces::EntityList()->GetClientEntityFromHandle((PVOID)pWareables[0]))
	{
		for (ClientClass* pClass = Interfaces::Client()->GetAllClasses(); pClass; pClass = pClass->m_pNext)
		{
			if (pClass->m_ClassID == (int)CLIENT_CLASS_ID::CEconWearable)
			{
				int iEntry = Interfaces::EntityList()->GetHighestEntityIndex() + 1;
				int iSerial = CSX::Utils::RandomIntRange(0x0, 0xFFF);

				if (pClass->m_pCreateFn(iEntry, iSerial))
				{
					pWareables[0] = iEntry | (iSerial << 16);
					break;
				}
			}
		}

		if (!pWareables[0])
			return;

		CBaseAttributableItem* pGlove = (CBaseAttributableItem*)Interfaces::EntityList()->GetClientEntityFromHandle((PVOID)pWareables[0]);

		if (!pGlove)
			return;

		PlayerInfo LocalPlayerInfo;

		if (!Interfaces::Engine()->GetPlayerInfo(Interfaces::Engine()->GetLocalPlayer(), &LocalPlayerInfo))
			return;

		*pGlove->GetItemDefinitionIndex() = k_glove_names.at(Settings::Skin::gloves_model).GloveType;
		*pGlove->GetItemIDHigh() = -1;
		*pGlove->GetEntityQuality() = 4;
		*pGlove->GetAccountID() = LocalPlayerInfo.m_nXuidLow;
		*pGlove->GetFallbackWear() = 0.00000001f;
		*pGlove->GetFallbackSeed() = 0;
		*pGlove->GetFallbackStatTrak() = -1;
		*pGlove->GetFallbackPaintKit() = GloveSkin[Settings::Skin::gloves_model].PaintKit[Settings::Skin::gloves_skin];

		int ModelIndex = Interfaces::ModelInfo()->GetModelIndex(k_glove_names.at(Settings::Skin::gloves_model).szModel);


		if (!ModelIndex)
			return;

		if (pGlove->GetViewModel())
			pGlove->GetViewModel()->SetModelIndex(ModelIndex);

		if (pGlove->GetClientNetworkable())
			pGlove->GetClientNetworkable()->PreDataUpdate(DATA_UPDATE_CREATED);
	}
}

void Hook_SetViewModelSequence(const CRecvProxyData *pDataConst, void *pStruct, void *pOut)
{
	CRecvProxyData* pData = const_cast<CRecvProxyData*>(pDataConst);
	CBaseViewModel* pViewModel = (CBaseViewModel*)pStruct;

	if (pViewModel)
	{
		IClientEntity* pOwner = Interfaces::EntityList()->GetClientEntityFromHandle((PVOID)pViewModel->GetOwner());

		if (pOwner && pOwner->EntIndex() == Interfaces::Engine()->GetLocalPlayer())
		{
			const model_t* pModel = Interfaces::ModelInfo()->GetModel(pViewModel->GetModelIndex());
			const char* szModel = Interfaces::ModelInfo()->GetModelName(pModel);

			int m_nSequence = pData->m_Value.m_Int;
			//[junk_disable /]
			if (!strcmp(szModel, "models/weapons/v_knife_butterfly.mdl"))
			{
				switch (m_nSequence)
				{
				case SEQUENCE_DEFAULT_DRAW:
					m_nSequence = CSX::Utils::RandomIntRange(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2); break;
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence = CSX::Utils::RandomIntRange(SEQUENCE_BUTTERFLY_LOOKAT01, SEQUENCE_BUTTERFLY_LOOKAT03); break;
				default:
					m_nSequence++;
				}
			}
			else if (!strcmp(szModel, "models/weapons/v_knife_falchion_advanced.mdl"))
			{
				// Fix animations for the Falchion Knife.
				switch (m_nSequence)
				{
				case SEQUENCE_DEFAULT_IDLE2:
					m_nSequence = SEQUENCE_FALCHION_IDLE1; break;
				case SEQUENCE_DEFAULT_HEAVY_MISS1:
					m_nSequence = CSX::Utils::RandomIntRange(SEQUENCE_FALCHION_HEAVY_MISS1, SEQUENCE_FALCHION_HEAVY_MISS1_NOFLIP); break;
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence = CSX::Utils::RandomIntRange(SEQUENCE_FALCHION_LOOKAT01, SEQUENCE_FALCHION_LOOKAT02); break;
				case SEQUENCE_DEFAULT_DRAW:
				case SEQUENCE_DEFAULT_IDLE1:
					break;
				default:
					m_nSequence--;
				}
			}
			else if (!strcmp(szModel, "models/weapons/v_knife_push.mdl"))
			{
				// Fix animations for the Shadow Daggers.
				switch (m_nSequence)
				{
				case SEQUENCE_DEFAULT_IDLE2:
					m_nSequence = SEQUENCE_DAGGERS_IDLE1; break;
				case SEQUENCE_DEFAULT_LIGHT_MISS1:
				case SEQUENCE_DEFAULT_LIGHT_MISS2:
					m_nSequence = CSX::Utils::RandomIntRange(SEQUENCE_DAGGERS_LIGHT_MISS1, SEQUENCE_DAGGERS_LIGHT_MISS5); break;
				case SEQUENCE_DEFAULT_HEAVY_MISS1:
					m_nSequence = CSX::Utils::RandomIntRange(SEQUENCE_DAGGERS_HEAVY_MISS2, SEQUENCE_DAGGERS_HEAVY_MISS1); break;
				case SEQUENCE_DEFAULT_HEAVY_HIT1:
				case SEQUENCE_DEFAULT_HEAVY_BACKSTAB:
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence += 3; break;
				case SEQUENCE_DEFAULT_DRAW:
				case SEQUENCE_DEFAULT_IDLE1:
					break;
				default:
					m_nSequence += 2;
				}
			}
			else if (!strcmp(szModel, "models/weapons/v_knife_survival_bowie.mdl"))
			{
				switch (m_nSequence)
				{
				case SEQUENCE_DEFAULT_DRAW:
				case SEQUENCE_DEFAULT_IDLE1:
					break;
				case SEQUENCE_DEFAULT_IDLE2:
					m_nSequence = SEQUENCE_BOWIE_IDLE1; break;
				default:
					m_nSequence--;
				}
			}
			//[junk_enable /]
			pData->m_Value.m_Int = m_nSequence;
		}
	}

	fnSequenceProxyFn(pData, pStruct, pOut);
}

std::deque<Kit_t> k_skins;
std::deque<Kit_t> k_gloves;
std::deque<Kit_t> k_stickers;

class CCStrike15ItemSchema;
class CCStrike15ItemSystem;

template <typename Key, typename Value>
struct Node_t
{
	int previous_id;		//0x0000
	int next_id;			//0x0004
	void* _unknown_ptr;		//0x0008
	int _unknown;			//0x000C
	Key key;				//0x0010
	Value value;			//0x0014
};

template <typename Key, typename Value>
struct Head_t
{
	Node_t<Key, Value>* memory;		//0x0000
	int allocation_count;			//0x0004
	int grow_size;					//0x0008
	int start_element;				//0x000C
	int next_available;				//0x0010
	int _unknown;					//0x0014
	int last_element;				//0x0018
}; //Size=0x001C

   // could use CUtlString but this is just easier and CUtlString isn't needed anywhere else
struct String_t
{
	char* buffer;	//0x0000
	int capacity;	//0x0004
	int grow_size;	//0x0008
	int length;		//0x000C
}; //Size=0x0010

struct CPaintKit
{
	int id;						//0x0000

	String_t name;				//0x0004
	String_t description;		//0x0014
	String_t item_name;			//0x0024
	String_t material_name;		//0x0034
	String_t image_inventory;	//0x0044

	char pad_0x0054[0x8C];		//0x0054
}; //Size=0x00E0

struct CStickerKit
{
	int id;

	int item_rarity;

	String_t name;
	String_t description;
	String_t item_name;
	String_t material_name;
	String_t image_inventory;

	int tournament_event_id;
	int tournament_team_id;
	int tournament_player_id;
	bool is_custom_sticker_material;

	float rotate_end;
	float rotate_start;

	float scale_min;
	float scale_max;

	float wear_min;
	float wear_max;

	String_t image_inventory2;
	String_t image_inventory_large;

	uint32_t pad0[4];
};
// TODO: Sort out the paint kits
void InitializeKits()
{
	static std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

	// Search the relative calls

	// call    ItemSystem
	// push    dword ptr [esi+0Ch]
	// lea     ecx, [eax+4]
	// call    CEconItemSchema::GetPaintKitDefinition

	auto sig_address = CSX::Memory::FindPBYTEPattern("client_panorama.dll", (PBYTE)"\xE8\x00\x00\x00\x00\xFF\x76\x0C\x8D\x48\x04\xE8", "x????xxxxxxx", NULL, NULL);

	// Skip the opcode, read rel32 address
	auto item_system_offset = *reinterpret_cast<int32_t*>(sig_address + 1);

	// Add the offset to the end of the instruction
	auto item_system_fn = reinterpret_cast<CCStrike15ItemSystem* (*)()>(sig_address + 5 + item_system_offset);

	// Skip VTable, first member variable of ItemSystem is ItemSchema
	auto item_schema = reinterpret_cast<CCStrike15ItemSchema*>(uintptr_t(item_system_fn()) + sizeof(void*));

	// Dump paint kits
	{
		// Skip the instructions between, skip the opcode, read rel32 address
		auto get_paint_kit_definition_offset = *reinterpret_cast<int32_t*>(sig_address + 11 + 1);

		// Add the offset to the end of the instruction
		auto get_paint_kit_definition_fn = reinterpret_cast<CPaintKit* (__thiscall *)(CCStrike15ItemSchema*, int)>(sig_address + 11 + 5 + get_paint_kit_definition_offset);

		// The last offset is head_element, we need that

		// push    ebp
		// mov     ebp, esp
		// sub     esp, 0Ch
		// mov     eax, [ecx+298h]

		// Skip instructions, skip opcode, read offset
		auto start_element_offset = *reinterpret_cast<intptr_t*>(uintptr_t(get_paint_kit_definition_fn) + 8 + 2);

		// Calculate head base from start_element's offset
		auto head_offset = start_element_offset - 12;

		auto map_head = reinterpret_cast<Head_t<int, CPaintKit*>*>(uintptr_t(item_schema) + head_offset);

		for (int i = 0; i <= map_head->last_element; ++i)
		{
			auto paint_kit = map_head->memory[i].value;

			if (paint_kit->id == 9001)
				continue;

			const wchar_t* wide_name = Interfaces::GetLocalize()->Find(paint_kit->item_name.buffer + 1);
			auto name = converter.to_bytes(wide_name);

			if (paint_kit->id < 10000) {
				k_skins.push_back({ paint_kit->id, name });
				for (int i = 0; i < KNIFE_DATA_SIZE; i++) {
					KnifeSkins[i].SkinNames.push_back(name);
					KnifeSkins[i].SkinPaintKit.push_back(paint_kit->id);
				}
				for (int i = 0; i < WEAPON_DATA_SIZE; i++) {
					WeaponSkins[i].SkinNames.push_back(name);
					WeaponSkins[i].SkinPaintKit.push_back(paint_kit->id);
				}
			}
			else {
				k_gloves.push_back({ paint_kit->id, name });
				for (int i = 0; i < 10; i++) {
					GloveSkin[i].Names.push_back(name);
					GloveSkin[i].PaintKit.push_back(paint_kit->id);
				}

			}
		}
		std::sort(k_skins.begin(), k_skins.end());
		std::sort(k_gloves.begin(), k_gloves.end());

	}

	// Dump sticker kits
	{
		auto sticker_sig = CSX::Memory::FindPBYTEPattern("client_panorama.dll", (PBYTE)"\x53\x8D\x48\x04\xE8\x00\x00\x00\x00\x8B\x4D\x10", "xxxxx????xxx", NULL, NULL) + 4;

		// Skip the opcode, read rel32 address
		auto get_sticker_kit_definition_offset = *reinterpret_cast<intptr_t*>(sticker_sig + 1);

		// Add the offset to the end of the instruction
		auto get_sticker_kit_definition_fn = reinterpret_cast<CPaintKit* (__thiscall *)(CCStrike15ItemSchema*, int)>(sticker_sig + 5 + get_sticker_kit_definition_offset);

		// The last offset is head_element, we need that

		//	push    ebp
		//	mov     ebp, esp
		//	push    ebx
		//	push    esi
		//	push    edi
		//	mov     edi, ecx
		//	mov     eax, [edi + 2BCh]

		// Skip instructions, skip opcode, read offset
		auto start_element_offset = *reinterpret_cast<intptr_t*>(uintptr_t(get_sticker_kit_definition_fn) + 8 + 2);

		// Calculate head base from start_element's offset
		auto head_offset = start_element_offset - 12;

		auto map_head = reinterpret_cast<Head_t<int, CStickerKit*>*>(uintptr_t(item_schema) + head_offset);

		for (int i = 0; i <= map_head->last_element; ++i)
		{
			auto sticker_kit = map_head->memory[i].value;

			char sticker_name_if_valve_fucked_up_their_translations[64];

			auto sticker_name_ptr = sticker_kit->item_name.buffer + 1;

			if (strstr(sticker_name_ptr, "StickerKit_dhw2014_dignitas"))
			{
				strcpy_s(sticker_name_if_valve_fucked_up_their_translations, "StickerKit_dhw2014_teamdignitas");
				strcat_s(sticker_name_if_valve_fucked_up_their_translations, sticker_name_ptr + 27);
				sticker_name_ptr = sticker_name_if_valve_fucked_up_their_translations;
			}

			const wchar_t* wide_name = Interfaces::GetLocalize()->Find(sticker_name_ptr);
			auto name = converter.to_bytes(wide_name);

			k_stickers.push_back({ sticker_kit->id, name });
		}

		std::sort(k_stickers.begin(), k_stickers.end());

		k_stickers.insert(k_stickers.begin(), { 0, "None" });
	}
}

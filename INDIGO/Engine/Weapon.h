#pragma once

#include "Engine.h"

namespace Engine
{
	class CBaseAttributableItem;
	class CBaseViewModel;

	class CBaseWeapon : public IClientEntity
	{
	public:
//[swap_lines]
		int				GetWeaponId();
		int				GetWeaponType();
		int				GetWeaponAmmo();
		int				GetZoomLevel();
		bool			GetWeaponReload();
		float			GetNextPrimaryAttack();

		CWeaponInfo*			GetWeaponInfo();
		CBaseAttributableItem*	GeteAttributableItem();

		const char*		GetName();
		const char*		GetPrintName();
//[/swap_lines]
	};

	class CBaseAttributableItem : public IClientEntity
	{
	public:
//[swap_lines]
		int*			GetItemDefinitionIndex();
		int*			GetItemIDHigh();
		int*			GetAccountID();
		int*			GetEntityQuality();
		int*			GetOriginalOwnerXuidLow();
		int*			GetOriginalOwnerXuidHigh();
		int*			GetFallbackPaintKit();
		float*			GetFallbackWear();
		int*			GetFallbackSeed();
		int*			GetFallbackStatTrak();
		CBaseViewModel*	GetViewModel();
//[/swap_lines]
	};

	enum WEAPON_TYPE
	{
		WEAPON_TYPE_PISTOL ,
		WEAPON_TYPE_SHOTGUN ,
		WEAPON_TYPE_SNIPER ,
		WEAPON_TYPE_GRENADE ,
		WEAPON_TYPE_KNIFE ,
		WEAPON_TYPE_C4 ,
		WEAPON_TYPE_UNKNOWN ,
	};

	enum WEAPON_ID
	{
		WEAPON_NONE = 0 ,
		WEAPON_DEAGLE = 1 ,
		WEAPON_ELITE = 2 ,
		WEAPON_FIVESEVEN = 3 ,
		WEAPON_GLOCK = 4 ,
		WEAPON_AK47 = 7 ,
		WEAPON_AUG = 8 ,
		WEAPON_AWP = 9 ,
		WEAPON_FAMAS = 10 ,
		WEAPON_G3SG1 = 11 ,
		WEAPON_GALILAR = 13 ,
		WEAPON_M249 = 14 ,
		WEAPON_M4A1 = 16 ,
		WEAPON_MAC10 = 17 ,
		WEAPON_P90 = 19 ,
		WEAPON_UMP45 = 24 ,
		WEAPON_XM1014 = 25 ,
		WEAPON_BIZON = 26 ,
		WEAPON_MAG7 = 27 ,
		WEAPON_NEGEV = 28 ,
		WEAPON_SAWEDOFF = 29 ,
		WEAPON_TEC9 = 30 ,
		WEAPON_TASER = 31 ,
		WEAPON_HKP2000 = 32 ,
		WEAPON_MP7 = 33 ,
		WEAPON_MP9 = 34 ,
		WEAPON_NOVA = 35 ,
		WEAPON_P250 = 36 ,
		WEAPON_SCAR20 = 38 ,
		WEAPON_SG553 = 39 ,
		WEAPON_SSG08 = 40 ,
		WEAPON_KNIFE = 42 ,
		WEAPON_FLASHBANG = 43 ,
		WEAPON_HEGRENADE = 44 ,
		WEAPON_SMOKEGRENADE = 45 ,
		WEAPON_MOLOTOV = 46 ,
		WEAPON_DECOY = 47 ,
		WEAPON_INCGRENADE = 48 ,
		WEAPON_C4 = 49 ,
		WEAPON_KNIFE_T = 59 ,
		WEAPON_M4A1_SILENCER = 60 ,
		WEAPON_USP_SILENCER = 61 ,
		WEAPON_CZ75A = 63 ,
		WEAPON_REVOLVER = 64 ,
		WEAPON_KNIFE_BAYONET = 500 ,
		WEAPON_KNIFE_FLIP = 505 ,
		WEAPON_KNIFE_GUT = 506 ,
		WEAPON_KNIFE_KARAMBIT = 507 ,
		WEAPON_KNIFE_M9_BAYONET = 508 ,
		WEAPON_KNIFE_TACTICAL = 509 ,
		WEAPON_KNIFE_FALCHION = 512 ,
		WEAPON_KNIFE_SURVIVAL_BOWIE = 514 ,
		WEAPON_KNIFE_BUTTERFLY = 515 ,
		WEAPON_KNIFE_PUSH = 516
	};
}
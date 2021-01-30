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
		//const char*		GetPrintName(); //don't need as it's never called!
		//[/swap_lines]
	};

	class CBaseAttributableItem : public IClientEntity
	{
	public:
		//[swap_lines]
		short*			GetItemDefinitionIndex();
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

	//CSWeaponType - 18th March 2020
	enum WEAPON_TYPE {
		WEAPON_TYPE_KNIFE = 0,
		WEAPON_TYPE_PISTOL,
		WEAPON_TYPE_SUBMACHINEGUN,
		WEAPON_TYPE_RIFLE,
		WEAPON_TYPE_SHOTGUN,
		WEAPON_TYPE_SNIPER,
		WEAPON_TYPE_MACHINEGUN,
		WEAPON_TYPE_C4,
		WEAPON_TYPE_PLACEHOLDER,
		WEAPON_TYPE_GRENADE,
		WEAPON_TYPE_UNKNOWN,
	};

		//1st January 2021
	//ItemDefinitionIndex - item_definition_index
	enum WEAPON_ID {
		//weapons
		WEAPON_DEAGLE = 1,
		WEAPON_ELITE,// = 2, //Dual Berettas
		WEAPON_FIVESEVEN,// = 3,
		WEAPON_GLOCK,// = 4,
		WEAPON_AK47 = 7,
		WEAPON_AUG,// = 8,
		WEAPON_AWP,// = 9,
		WEAPON_FAMAS,// = 10,
		WEAPON_G3SG1,// = 11,
		WEAPON_GALILAR = 13,
		WEAPON_M249,// = 14,
		WEAPON_M4A1 = 16,
		WEAPON_MAC10,// = 17,
		WEAPON_P90 = 19,
		WEAPON_ZONE_REPULSOR,// = 20, //co-op mission thingy
		WEAPON_MP5SD = 23,
		WEAPON_UMP45,// = 24,
		WEAPON_XM1014,// = 25,
		WEAPON_BIZON,// = 26,
		WEAPON_MAG7,// = 27,
		WEAPON_NEGEV,// = 28,
		WEAPON_SAWEDOFF,// = 29,
		WEAPON_TEC9,// = 30,
		WEAPON_TASER,// = 31,
		WEAPON_HKP2000,// = 32, //P2000
		WEAPON_MP7,// = 33,
		WEAPON_MP9,// = 34,
		WEAPON_NOVA,// = 35,
		WEAPON_P250,// = 36,
		WEAPON_SHIELD,// = 37, //ballistic shield
		WEAPON_SCAR20,// = 38,
		WEAPON_SG556,// = 39, //SG 553
		WEAPON_SSG08,// = 40,
		WEAPON_KNIFEGG,// = 41, //Gold Knife
		WEAPON_KNIFE,// = 42, //Default (CT) Knife
		WEAPON_FLASHBANG,// = 43,
		WEAPON_HEGRENADE,// = 44,
		WEAPON_SMOKEGRENADE,// = 45,
		WEAPON_MOLOTOV,// = 46,
		WEAPON_DECOY,// = 47,
		WEAPON_INCGRENADE,// = 48,
		WEAPON_C4,// = 49,
		ITEM_KEVLAR,// = 50, //body armour
		ITEM_ASSAULTSUIT,// = 51, //helmet + body armour
		ITEM_HEAVYASSAULTSUIT,// = 52, //special armour in co-op modes
		ITEM_NVG = 54, //night vision goggles
		ITEM_DEFUSER,// = 55, //bomb defuser
		ITEM_CUTTERS,// = 56, //hostage cutter
		WEAPON_HEALTHSHOT,// = 57,
		WEAPON_KNIFE_T = 59, //Default (T) Knife
		WEAPON_M4A1_SILENCER,// = 60,
		WEAPON_USP_SILENCER,// = 61,
		WEAPON_CZ75A = 63,
		WEAPON_REVOLVER,// = 64,
		WEAPON_TAGRENADE = 68,
		WEAPON_FISTS,// = 69,
		WEAPON_BREACHCHARGE,// = 70,
		WEAPON_TABLET = 72,
		WEAPON_MELEE = 74,
		WEAPON_AXE,// = 75,
		WEAPON_HAMMER,// = 76,
		WEAPON_SPANNER = 78, //Wrench
		WEAPON_KNIFE_GHOST = 80, //Spectral Shiv
		WEAPON_FIREBOMB,// = 81,
		WEAPON_DIVERSION,// = 82,
		WEAPON_FRAG_GRENADE,// = 83,
		WEAPON_SNOWBALL,// = 84,
		WEAPON_BUMPMINE,// = 85,
		WEAPON_BAYONET = 500, //Bayonet
		WEAPON_KNIFE_CSS = 503, //CSS Knife
		WEAPON_KNIFE_FLIP = 505, //Flip Knife
		WEAPON_KNIFE_GUT,// = 506, //Gut Knife
		WEAPON_KNIFE_KARAMBIT,// = 507, //Karambit
		WEAPON_KNIFE_M9_BAYONET,// = 508, //M9 Bayonet
		WEAPON_KNIFE_TACTICAL,// = 509, //Huntsman Knife
		WEAPON_KNIFE_FALCHION = 512, //Falchion Knife
		WEAPON_KNIFE_SURVIVAL_BOWIE = 514, //Bowie Knife
		WEAPON_KNIFE_BUTTERFLY,// = 515, //Butterfly Knife
		WEAPON_KNIFE_PUSH,// = 516 //Shadow Daggers
		WEAPON_KNIFE_CORD,// = 517, //Paracord Knife
		WEAPON_KNIFE_CANIS,// = 518, //Survival Knife
		WEAPON_KNIFE_URSUS,// = 519, //Ursus Knife
		WEAPON_KNIFE_GYPSY_JACKKNIFE,// = 520 //Navaja Knife
		WEAPON_KNIFE_OUTDOOR,// = 521, //Nomad Knife
		WEAPON_KNIFE_STILETTO,// = 522, //Stiletto Knife
		WEAPON_KNIFE_WIDOWMAKER,// = 523, //Talon Knife
		WEAPON_KNIFE_SKELETON = 525, //Skeleton Knife
		STUDDED_BROKENFANG_GLOVES = 4725, //Brokenfang Gloves
		STUDDED_BLOODHOUND_GLOVES = 5027, //Bloodhound Gloves
		T_GLOVES,// = 5028, //Default (T) Gloves
		CT_GLOVES,// = 5029, //Default (CT) Gloves
		SPORTY_GLOVES,// = 5030, //Sporty Gloves
		SLICK_GLOVES,// = 5031, //Slick Gloves
		LEATHER_HANDWRAPS,// = 5032, //Hand Wraps
		MOTORCYCLE_GLOVES,// = 5033, //Moto Gloves
		SPECIALIST_GLOVES,// = 5034, //Specalist Gloves
		STUDDED_HYDRA_GLOVES,// = 5035, //Hydra Gloves
	};
}

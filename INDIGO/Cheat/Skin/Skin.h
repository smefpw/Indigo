#pragma once

#include <unordered_map>

#include "../../Engine/Engine.h"

#include "DynSkin.h"

#include <xlocbuf>
#include <deque>
#include <codecvt>
#include <algorithm>

namespace Engine
{
	class CBaseAttributableItem;
	class CBaseEntity;
}

using namespace Engine;

struct EconomyItemCfg
{
	int iItemDefinitionIndex = 0;
	int nFallbackPaintKit = 0;
	int iEntityQuality = 4;
	int nFallbackStatTrak = -1;
	float flFallbackWear = 0.00000001f;
};

struct WeaponSkins_s
{
	vector<int> SkinPaintKit;
	vector<string> SkinNames;
};

struct KnifeSkins_s
{
	vector<int> SkinPaintKit;
	vector<string> SkinNames;
};

struct GlovesSkins_s
{
	int ItemIndex;
	int PaintKit;
	const char* szModel;
};

extern WeaponSkins_s WeaponSkins[33];
extern KnifeSkins_s KnifeSkins[10];
extern GlovesSkins_s GlovesSkin_Array[49];

extern unordered_map<int , EconomyItemCfg> g_SkinChangerCfg;
extern unordered_map<int , const char*> g_ViewModelCfg;
extern unordered_map<const char* , const char*> g_KillIconCfg;

extern char* pWeaponData[33];
extern int pWeaponItemIndexData[33];
extern char* pKnifeData[10];

#define WEAPON_DATA_SIZE ( sizeof( pWeaponData ) / sizeof( *pWeaponData ) )
#define KNIFE_DATA_SIZE ( sizeof( pKnifeData ) / sizeof( *pKnifeData ) )

int GetWeaponSkinIndexFromPaintKit( int iPaintKit );
int GetKnifeSkinIndexFromPaintKit( int iPaintKit , bool tt );

class CSkin
{
public:
//[swap_lines]
	void OnEvents( IGameEvent* pEvent );

	void SetSkinConfig();
	void SetModelConfig();
	void SetKillIconCfg();

	void InitalizeSkins();

	bool ApplyCustomSkin( CBaseAttributableItem* pWeapon , int nWeaponIndex );
	bool ApplyCustomModel( CBaseEntity* pLocal , CBaseAttributableItem* pWeapon );
	bool ApplyCustomKillIcon( IGameEvent* pEvent );
//[/swap_lines]
};

void Skin_OnFrameStageNotify( ClientFrameStage_t Stage );
void Gloves_OnFrameStageNotify( ClientFrameStage_t Stage );

void Hook_SetViewModelSequence( const CRecvProxyData *pDataConst , void *pStruct , void *pOut );

// œ»—“ŒÀ≈“€
//[enc_string_enable /]
#define WEAPON_DEAGLE_STR		"deagle"		// 0
#define WEAPON_ELITE_STR		"elite"			// 1
#define WEAPON_FIVESEVEN_STR	"fiveseven"		// 2
#define WEAPON_GLOCK_STR		"glock"			// 3
#define WEAPON_TASER_STR		"taser"
#define WEAPON_HKP2000_STR		"hkp2000"		// 4
#define WEAPON_P250_STR			"p250"			// 5
#define WEAPON_USP_S_STR		"usp_silencer"	// 6
#define WEAPON_CZ75A_STR		"cz75a"			// 7
#define WEAPON_REVOLVER_STR		"revolver"		// 8
#define WEAPON_TEC9_STR			"tec9"			// 9

// ƒ–Œ¡Œ¬» » , ¿¬“ŒÃ¿“€

#define WEAPON_AK47_STR		"ak47"				// 10
#define WEAPON_AUG_STR		"aug"				// 11
#define WEAPON_FAMAS_STR	"famas"				// 12
#define WEAPON_GALILAR_STR	"galilar"			// 13
#define WEAPON_M249_STR		"m249"				// 14
#define WEAPON_M4A1_STR		"m4a1"				// 15
#define WEAPON_M4A1_S_STR	"m4a1_silencer"		// 16
#define WEAPON_MAC10_STR	"mac10"				// 17
#define WEAPON_P90_STR		"p90"				// 18
#define WEAPON_UMP45_STR	"ump45"				// 19
#define WEAPON_XM1014_STR	"xm1014"			// 20
#define WEAPON_BIZON_STR	"bizon"				// 21
#define WEAPON_MAG7_STR		"mag7"				// 22
#define WEAPON_NEGEV_STR	"negev"				// 23
#define WEAPON_SAWEDOFF_STR	"sawedoff"			// 24
#define WEAPON_MP7_STR		"mp7"				// 25
#define WEAPON_MP9_STR		"mp9"				// 26
#define WEAPON_NOVA_STR		"nova"				// 27
#define WEAPON_SG553_STR	"sg553"				// 28
#define WEAPON_G3SG1_STR	"g3sg1"				// 29
#define WEAPON_SCAR20_STR	"scar20"			// 30

// —Õ¿…œ≈– »

#define WEAPON_AWP_STR		"awp"				// 31
#define WEAPON_SSG08_STR	"ssg08"				// 32

struct Kit_t
{
	int id;
	std::string name;

	bool operator < (const Kit_t& other) const { return (name < other.name); }
};


extern std::deque<Kit_t> k_skins;
extern std::deque<Kit_t> k_gloves;
extern std::deque<Kit_t> k_stickers;

extern void InitializeKits();
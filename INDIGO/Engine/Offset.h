#pragma once

#include "Engine.h"

#define FIND_W2MATRIX_PATTERN		"\xA1\x00\x00\x00\x00\x83\xF8\x01\x7E\x11\x69\xC8"
//[enc_string_enable /]
#define FIND_W2MATRIX_MASK			"x????xxxxxxx"

#define BASE_PLAYER					"DT_BasePlayer"
#define BASE_PLAYER_WEAPONS			"m_hMyWeapons"
#define BASE_PLAYER_WERABLES		"m_hMyWearables"
#define BASE_PLAYER_VIEWMODEL		"m_hViewModel[0]"
#define BASE_PLAYER_TICKBASE		"m_nTickBase"
#define BASE_PLAYER_OBSMODE			"m_iObserverMode"
#define BASE_PLAYER_OBSERVER		"m_hObserverTarget"

#define CS_PLAYER					"DT_CSPlayer"

#define CS_PLAYER_LIFESTATE			"m_lifeState"

#define CS_PLAYER_HASHELMET			"m_bHasHelmet"
#define CS_PLAYER_HASHDEFUSER		"m_bHasDefuser"
#define CS_PLAYER_ISSCOPED			"m_bIsScoped"

#define CS_PLAYER_FOVSTART			"m_iFOVStart"
#define CS_PLAYER_FLAGS				"m_fFlags"

#define CS_PLAYER_HEALTH			"m_iHealth"
#define CS_PLAYER_ARMOR				"m_ArmorValue"
#define CS_PLAYER_TEAMNUM			"m_iTeamNum"
#define CS_PLAYER_SHOTFIRED			"m_iShotsFired"
#define CS_PLAYER_AIMPUNCHANG		"m_aimPunchAngle"
#define CS_PLAYER_VIEWPUNCHANG		"m_viewPunchAngle"
#define CS_PLAYER_VECVELOCITY		"m_vecVelocity[0]"
#define CS_PLAYER_VECVIEW			"m_vecViewOffset[0]"
#define CS_PLAYER_EYEANGLES			"m_angEyeAngles"
#define CS_PLAYER_ACTIVEWEAPON		"m_hActiveWeapon"

#define BASE_COMBAT_WEAPON			"DT_BaseCombatWeapon"
#define BASE_COMBAT_WEAPON_CLIP		"m_iClip1"
#define BASE_COMBAT_WEAPON_PRIM		"m_flNextPrimaryAttack"

#define BASE_ATTRIBUTABLE_ITEM				"DT_BaseAttributableItem"
#define BASE_ATTRIBUTABLE_ITEM_INDEX		"m_iItemDefinitionIndex"
#define BASE_ATTRIBUTABLE_ITEM_ID_HIGH		"m_iItemIDHigh"
#define BASE_ATTRIBUTABLE_ITEM_ACCOUNT_ID	"m_iAccountID"
#define BASE_ATTRIBUTABLE_ITEM_ENTITY_QAL	"m_iEntityQuality"
#define BASE_ATTRIBUTABLE_ITEM_OWNER_LOW	"m_OriginalOwnerXuidLow"
#define BASE_ATTRIBUTABLE_ITEM_OWNER_HIGH	"m_OriginalOwnerXuidHigh"
#define BASE_ATTRIBUTABLE_ITEM_PAINT_KIT	"m_nFallbackPaintKit"
#define BASE_ATTRIBUTABLE_ITEM_FALL_WEAR	"m_flFallbackWear"
#define BASE_ATTRIBUTABLE_ITEM_FALL_SEED	"m_nFallbackSeed"
#define BASE_ATTRIBUTABLE_ITEM_FALL_STAT	"m_nFallbackStatTrak"

#define BASE_ENTITY					"DT_BaseEntity"
#define BASE_ENTITY_MODEL_INDEX		"m_nModelIndex"
#define BASE_ENTITY_SPOTTED			"m_bSpotted"

#define SimTime						"m_flSimulationTime"

#define BASE_VIEW_MODEL				"DT_BaseViewModel"
#define BASE_VIEW_MODEL_OWNER		"m_hOwner"
#define BASE_VIEW_MODEL_WEAPON		"m_hWeapon"

#define BASE_WEAPON_AWP				"DT_WeaponAWP"
#define BASE_WEAPON_AWP_ZOOMLEVEL	"m_zoomLevel"

namespace Engine
{
	namespace Offset
	{
		namespace Entity
		{
			//[swap_lines]
			extern DWORD m_hMyWeapons;
			extern DWORD m_hMyWearables;
			extern DWORD m_hViewModel;
			extern DWORD m_nTickBase;
			extern DWORD m_iObserverMode;
			extern DWORD m_hObserverTarget;

			extern DWORD m_vecOrigin;
			extern DWORD deadflag;

			extern DWORD m_lifeState;
			extern DWORD m_flLowerBodyYawTarget;

			extern DWORD m_bHasHelmet;
			extern DWORD m_bHasDefuser;
			extern DWORD m_bIsScoped;

			extern DWORD m_iFOVStart;
			extern DWORD m_fFlags;
			extern DWORD m_iHealth;
			extern DWORD m_ArmorValue;
			extern DWORD m_iTeamNum;
			extern DWORD m_iShotsFired;

			extern DWORD m_aimPunchAngle;
			extern DWORD m_viewPunchAngle;

			extern DWORD m_vecVelocity;
			extern DWORD m_vecViewOffset;

			extern DWORD m_angEyeAngles;

			extern DWORD m_hActiveWeapon;

			extern DWORD m_iClip1;
			extern DWORD m_flNextPrimaryAttack;
			extern DWORD m_bCanReload;

			extern DWORD m_iItemDefinitionIndex;
			extern DWORD m_iItemIDHigh;
			extern DWORD m_iAccountID;
			extern DWORD m_iEntityQuality;
			extern DWORD m_OriginalOwnerXuidLow;
			extern DWORD m_OriginalOwnerXuidHigh;
			extern DWORD m_nFallbackPaintKit;
			extern DWORD m_flFallbackWear;
			extern DWORD m_nFallbackSeed;
			extern DWORD m_nFallbackStatTrak;

			extern DWORD m_nModelIndex;
			extern DWORD m_hOwner;
			extern DWORD m_hWeapon;
			extern DWORD m_bSpotted;
			extern DWORD m_flSimulationTime;

			extern DWORD m_zoomLevel;
			//[/swap_lines]
		}

		bool	Initialize();
		float*	FindW2Matrix();
	}
}
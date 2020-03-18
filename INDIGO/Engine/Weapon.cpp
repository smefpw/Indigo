#include "Weapon.h"

int wID, wType;
CWeaponInfo* wInfo;

namespace Engine
{
	//[junk_enable /]
	int CBaseWeapon::GetWeaponId() {
		typedef int(__thiscall* GetWeaponIDFn)(void*);
#if ENABLE_DEBUG_FILE == 1
		if (!wID) {
			wID = GetMethod<GetWeaponIDFn>(this, 458)(this);
			CSX::Log::Add("[GetWeaponID = %X]", wID);
			return wID;
		}
#endif
		return GetMethod<GetWeaponIDFn>(this, 458)(this);
	}

	int	CBaseWeapon::GetWeaponType() {
		typedef int(__thiscall* GetWeaponTypeFn)(void*);
#if ENABLE_DEBUG_FILE == 1
		if (!wType) {
			wType = GetMethod<GetWeaponTypeFn>(this, 453)(this);
			CSX::Log::Add("[GetWeaponType = %X]", wType);
			return wType;
		}
#endif
		return GetMethod<GetWeaponTypeFn>(this, 453)(this);
	}

	int CBaseWeapon::GetWeaponAmmo()
	{
		return *(PINT)((DWORD)this + Offset::Entity::m_iClip1);
	}

	int	CBaseWeapon::GetZoomLevel()
	{
		return *(PINT)((DWORD)this + Offset::Entity::m_zoomLevel);
	}

	bool CBaseWeapon::GetWeaponReload()
	{
		return *(bool*)((DWORD)this + Offset::Entity::m_bCanReload);
	}

	float CBaseWeapon::GetNextPrimaryAttack()
	{
		return *(float*)((DWORD)this + Offset::Entity::m_flNextPrimaryAttack);
	}

	//11th March 2020
	//GetCCSWeapData - GetWeaponData - GetCSWpnData
	CWeaponInfo* CBaseWeapon::GetWeaponInfo() {
		typedef CWeaponInfo*(__thiscall* GetWeaponDataFn)(void*);
#if ENABLE_DEBUG_FILE == 1
		if (!wInfo) {
			wInfo = GetMethod<GetWeaponDataFn>(this, 460)(this); //old = 460
			CSX::Log::Add("[GetWeaponInfo = %X]", wInfo);
			return wInfo;
		}
#endif
		return GetMethod<GetWeaponDataFn>(this, 460)(this); //old = 460
	}

	CBaseAttributableItem* CBaseWeapon::GeteAttributableItem()
	{
		return (CBaseAttributableItem*)(DWORD)this;
	}

	//18th March 2020
	const char*	CBaseWeapon::GetName()
	{
		typedef const char* (__thiscall* GetNameFn)(void*);
		return GetMethod<GetNameFn>(this, 386)(this); //383
	}

	//11th March 2020
	const char*	CBaseWeapon::GetPrintName()
	{
		typedef const char* (__thiscall* GetPrintNameFn)(void*);
		return GetMethod<GetPrintNameFn>(this, 380)(this);
	}

	short* CBaseAttributableItem::GetItemDefinitionIndex()
	{
		// DT_BaseAttributableItem -> m_AttributeManager -> m_Item -> m_iItemDefinitionIndex
		return (short*)((DWORD)this + Offset::Entity::m_iItemDefinitionIndex);
	}

	int* CBaseAttributableItem::GetItemIDHigh()
	{
		// DT_BaseAttributableItem -> m_AttributeManager -> m_Item -> m_iItemIDHigh
		return (int*)((DWORD)this + Offset::Entity::m_iItemIDHigh);
	}

	int* CBaseAttributableItem::GetAccountID()
	{
		// DT_BaseAttributableItem -> m_AttributeManager -> m_Item -> m_iAccountID
		return (int*)((DWORD)this + Offset::Entity::m_iAccountID);
	}

	int* CBaseAttributableItem::GetEntityQuality()
	{
		// DT_BaseAttributableItem -> m_AttributeManager -> m_Item -> m_iEntityQuality
		return (int*)((DWORD)this + Offset::Entity::m_iEntityQuality);
	}

	int* CBaseAttributableItem::GetOriginalOwnerXuidLow()
	{
		// DT_BaseAttributableItem -> m_OriginalOwnerXuidLow
		return (int*)((DWORD)this + Offset::Entity::m_OriginalOwnerXuidLow);
	}

	int* CBaseAttributableItem::GetOriginalOwnerXuidHigh()
	{
		// DT_BaseAttributableItem -> m_OriginalOwnerXuidHigh
		return (int*)((DWORD)this + Offset::Entity::m_OriginalOwnerXuidHigh);
	}

	int* CBaseAttributableItem::GetFallbackPaintKit()
	{
		// DT_BaseAttributableItem -> m_nFallbackPaintKit
		return (int*)((DWORD)this + Offset::Entity::m_nFallbackPaintKit);
	}

	float* CBaseAttributableItem::GetFallbackWear()
	{
		// DT_BaseAttributableItem -> m_flFallbackWear
		return (float*)((DWORD)this + Offset::Entity::m_flFallbackWear);
	}

	int* CBaseAttributableItem::GetFallbackSeed()
	{
		// DT_BaseAttributableItem -> m_nFallbackSeed
		return (int*)((DWORD)this + Offset::Entity::m_nFallbackSeed);
	}

	int* CBaseAttributableItem::GetFallbackStatTrak()
	{
		// DT_BaseAttributableItem -> m_nFallbackStatTrak
		return (int*)((DWORD)this + Offset::Entity::m_nFallbackStatTrak);
	}

	CBaseViewModel* CBaseAttributableItem::GetViewModel()
	{
		return (CBaseViewModel*)(DWORD)this;
	}
}
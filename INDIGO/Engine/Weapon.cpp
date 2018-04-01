#include "Weapon.h"

namespace Engine
{
//[junk_enable /]
	int CBaseWeapon::GetWeaponId()
	{
		typedef int( __thiscall* GetWeaponIDFn )( void* );
		return GetMethod<GetWeaponIDFn>( this , 458 )( this );
	}

	int	CBaseWeapon::GetWeaponType()
	{
		typedef int( __thiscall* GetWeaponTypeFn )( void* );
		return GetMethod<GetWeaponTypeFn>( this , 453 )( this );
	}

	int CBaseWeapon::GetWeaponAmmo()
	{
		return *(PINT)( ( DWORD )this + Offset::Entity::m_iClip1 );
	}

	int	CBaseWeapon::GetZoomLevel()
	{
		return *(PINT)( ( DWORD )this + Offset::Entity::m_zoomLevel );
	}

	bool CBaseWeapon::GetWeaponReload()
	{
		return *(bool*)( ( DWORD )this + Offset::Entity::m_bCanReload );
	}

	float CBaseWeapon::GetNextPrimaryAttack()
	{
		return *(float*)( ( DWORD )this + Offset::Entity::m_flNextPrimaryAttack );
	}

	CWeaponInfo* CBaseWeapon::GetWeaponInfo()
	{
		typedef CWeaponInfo*( __thiscall* GetWeaponDataFn )( void* );
		return GetMethod< GetWeaponDataFn >( this , 446 )( this );
	}

	CBaseAttributableItem* CBaseWeapon::GeteAttributableItem()
	{
		return (CBaseAttributableItem*)( DWORD )this;
	}

	const char*	CBaseWeapon::GetName()
	{
		typedef const char* ( __thiscall* GetNameFn )( void* );
		return GetMethod<GetNameFn>( this , 378 )( this );
	}

	const char*	CBaseWeapon::GetPrintName()
	{
		typedef const char* ( __thiscall* GetPrintNameFn )( void* );
		return GetMethod<GetPrintNameFn>( this , 379 )( this );
	}

	int* CBaseAttributableItem::GetItemDefinitionIndex()
	{
		// DT_BaseAttributableItem -> m_AttributeManager -> m_Item -> m_iItemDefinitionIndex
		return (PINT)( ( DWORD )this + Offset::Entity::m_iItemDefinitionIndex );
	}

	int* CBaseAttributableItem::GetItemIDHigh()
	{
		// DT_BaseAttributableItem -> m_AttributeManager -> m_Item -> m_iItemIDHigh
		return (int*)( ( DWORD )this + Offset::Entity::m_iItemIDHigh );
	}

	int* CBaseAttributableItem::GetAccountID()
	{
		// DT_BaseAttributableItem -> m_AttributeManager -> m_Item -> m_iAccountID
		return (int*)( ( DWORD )this + Offset::Entity::m_iAccountID );
	}

	int* CBaseAttributableItem::GetEntityQuality()
	{
		// DT_BaseAttributableItem -> m_AttributeManager -> m_Item -> m_iEntityQuality
		return (int*)( ( DWORD )this + Offset::Entity::m_iEntityQuality );
	}

	int* CBaseAttributableItem::GetOriginalOwnerXuidLow()
	{
		// DT_BaseAttributableItem -> m_OriginalOwnerXuidLow
		return (int*)( ( DWORD )this + Offset::Entity::m_OriginalOwnerXuidLow );
	}

	int* CBaseAttributableItem::GetOriginalOwnerXuidHigh()
	{
		// DT_BaseAttributableItem -> m_OriginalOwnerXuidHigh
		return (int*)( ( DWORD )this + Offset::Entity::m_OriginalOwnerXuidHigh );
	}

	int* CBaseAttributableItem::GetFallbackPaintKit()
	{
		// DT_BaseAttributableItem -> m_nFallbackPaintKit
		return (int*)( ( DWORD )this + Offset::Entity::m_nFallbackPaintKit );
	}

	float* CBaseAttributableItem::GetFallbackWear()
	{
		// DT_BaseAttributableItem -> m_flFallbackWear
		return (float*)( ( DWORD )this + Offset::Entity::m_flFallbackWear );
	}

	int* CBaseAttributableItem::GetFallbackSeed()
	{
		// DT_BaseAttributableItem -> m_nFallbackSeed
		return (int*)( ( DWORD )this + Offset::Entity::m_nFallbackSeed );
	}

	int* CBaseAttributableItem::GetFallbackStatTrak()
	{
		// DT_BaseAttributableItem -> m_nFallbackStatTrak
		return (int*)( ( DWORD )this + Offset::Entity::m_nFallbackStatTrak );
	}

	CBaseViewModel* CBaseAttributableItem::GetViewModel()
	{
		return (CBaseViewModel*)( DWORD )this;
	}
}
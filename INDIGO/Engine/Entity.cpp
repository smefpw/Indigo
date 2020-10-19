#include "Entity.h"
#define ptr( x, x1, x2 ) *(x*)( (DWORD)x1 + (DWORD)x2 )

bool isP, isP2;

namespace Engine
{
	//[junk_enable /]
	char* CBaseEntity::GetPlayerName()
	{
		if (IsPlayer())
		{
			static PlayerInfo Info;

			if (Interfaces::Engine()->GetPlayerInfo(EntIndex(), &Info))
				return Info.m_szPlayerName;
		}

		return "";
	}

	//26th September 2019 - 19 October 2020
	bool CBaseEntity::IsPlayer() {
		typedef bool(__thiscall* IsPlayerFn)(void*);
		return GetMethod<IsPlayerFn>(this, 157)(this); //152
	}

	bool CBaseEntity::IsValid()
	{
		return (!IsDead() && GetHealth() > 0 && !IsDormant()); //IsDormant lol
	}

	bool CBaseEntity::IsDead()
	{
		const auto LifeState = *reinterpret_cast<PBYTE>(DWORD(this) + Offset::Entity::m_lifeState);
		return (LifeState != LIFE_ALIVE);
	}

	Vector CBaseEntity::GetOrigin()
	{
		return *reinterpret_cast<Vector*>(DWORD(this) + Offset::Entity::m_vecOrigin);
	}

	bool CBaseEntity::IsVisible(CBaseEntity* pLocalEntity)
	{
		if (!pLocalEntity->IsValid())
			return false;

		Vector vSrcOrigin = pLocalEntity->GetEyePosition();

		if (vSrcOrigin.IsZero() || !vSrcOrigin.IsValid())
			return false;

		BYTE bHitBoxCheckVisible[6] = {
			HITBOX_HEAD,
			HITBOX_BODY,
			HITBOX_RIGHT_FOOT,
			HITBOX_LEFT_FOOT,
			HITBOX_RIGHT_HAND,
			HITBOX_LEFT_HAND,
		};

		CTraceFilter filter;

		filter.pSkip = pLocalEntity;

		for (int nHit = 0; nHit < 6; nHit++)
		{
			Vector vHitBox = GetHitboxPosition(bHitBoxCheckVisible[nHit]);

			if (vHitBox.IsZero() || !vHitBox.IsValid())
				continue;

			trace_t tr;
			Ray_t ray;

			ray.Init(vSrcOrigin, vHitBox);

			Interfaces::EngineTrace()->TraceRay(ray, PlayerVisibleMask, &filter, &tr);

			if (tr.m_pEnt == static_cast<IClientEntity*>(this) && !tr.allsolid)
				return true;
		}

		return false;
	}

	int CBaseEntity::GetMoveType()
	{
		if (this != nullptr && reinterpret_cast<DWORD>(this) != 0xE)
		{
			return ptr(int, this, 0x258);
			//return *(int*)((DWORD)this + (DWORD)0x258);
		}
	}

	bool CBaseEntity::HasHelmet()
	{
		return *reinterpret_cast<bool*>(DWORD(this) + Offset::Entity::m_bHasHelmet);
	}

	bool CBaseEntity::HasDefuser()
	{
		return *reinterpret_cast<bool*>(DWORD(this) + Offset::Entity::m_bHasDefuser);
	}

	int CBaseEntity::IsDefusing()
	{
		return *reinterpret_cast<bool*>(DWORD(this) + DWORD(Offset::Entity::m_bIsDefusing));
	}

	bool* CBaseEntity::IsSpotted()
	{
		return reinterpret_cast<bool*>(DWORD(this) + Offset::Entity::m_bSpotted);
	}

	float CBaseEntity::GetFlashDuration()
	{
		return *reinterpret_cast<float*>(DWORD(this) + Offset::Entity::m_flFlashDuration);
	}

	int CBaseEntity::IsFlashed()
	{
		return GetFlashDuration() > 0 ? true : false;
	}

	bool* CBaseEntity::IsReloading()
	{
		return reinterpret_cast<bool*>(DWORD(this) + Offset::Entity::m_bInReload);
	}

	int CBaseEntity::GetFovStart()
	{
		return *reinterpret_cast<PINT>(DWORD(this) + Offset::Entity::m_iFOVStart);
	}

	int CBaseEntity::GetFlags()
	{
		return *reinterpret_cast<PINT>(DWORD(this) + Offset::Entity::m_fFlags);
	}

	int CBaseEntity::GetHealth()
	{
		return *reinterpret_cast<PINT>(DWORD(this) + Offset::Entity::m_iHealth);
	}

	int CBaseEntity::GetArmor()
	{
		return *reinterpret_cast<PINT>(DWORD(this) + Offset::Entity::m_ArmorValue);
	}

	int CBaseEntity::GetTeam()
	{
		return *reinterpret_cast<PINT>(DWORD(this) + Offset::Entity::m_iTeamNum);
	}

	float CBaseEntity::GetLowerBodyYaw()
	{
		return *reinterpret_cast<float*>(DWORD(this) + Offset::Entity::m_flLowerBodyYawTarget);
	}

	float CBaseEntity::GetSimTime()
	{
		return *reinterpret_cast<float*>(DWORD(this) + Offset::Entity::m_flSimulationTime);
	}

	int CBaseEntity::GetShotsFired()
	{
		return *reinterpret_cast<PINT>(DWORD(this) + DWORD(Offset::Entity::m_iShotsFired));
	}

	int CBaseEntity::GetIsScoped()
	{
		return *reinterpret_cast<bool*>(DWORD(this) + DWORD(Offset::Entity::m_bIsScoped));
	}

	int CBaseEntity::GetTickBase()
	{
		return *reinterpret_cast<PINT>(DWORD(this) + DWORD(Offset::Entity::m_nTickBase));
	}

	ObserverMode_t CBaseEntity::GetObserverMode()
	{
		return *reinterpret_cast<ObserverMode_t*>(DWORD(this) + DWORD(Offset::Entity::m_iObserverMode));
	}

	PVOID CBaseEntity::GetObserverTarget()
	{
		return PVOID(*reinterpret_cast<PDWORD>(DWORD(this) + DWORD(Offset::Entity::m_hObserverTarget)));
	}

	PVOID CBaseEntity::GetActiveWeapon()
	{
		return reinterpret_cast<PVOID>(DWORD(this) + DWORD(Offset::Entity::m_hActiveWeapon));
	}

	//reverted, as it wasn't working.
	CBaseWeapon* CBaseEntity::GetBaseWeapon() {
		return (CBaseWeapon*)Interfaces::EntityList()->GetClientEntityFromHandle((PVOID)*(PDWORD)GetActiveWeapon());
	}

	UINT* CBaseEntity::GetWeapons()
	{
		// DT_BasePlayer -> m_hMyWeapons
		return reinterpret_cast<UINT*>(DWORD(this) + Offset::Entity::m_hMyWeapons);
	}

	UINT* CBaseEntity::GetWearables()
	{
		return reinterpret_cast<UINT*>(DWORD(this) + Offset::Entity::m_hMyWearables);
	}

	CBaseViewModel* CBaseEntity::GetViewModel()
	{
		// DT_BasePlayer -> m_hViewModel
		return static_cast<CBaseViewModel*>(Interfaces::EntityList()->GetClientEntityFromHandle(
			reinterpret_cast<PVOID>(*PDWORD(DWORD(this) + Offset::Entity::m_hViewModel))));
	}

	Vector* CBaseEntity::GetVAngles()
	{
		return reinterpret_cast<Vector*>(uintptr_t(this) + Offset::Entity::deadflag + 0x4);
	}

	Vector CBaseEntity::GetAimPunchAngle()
	{
		return *reinterpret_cast<Vector*>(DWORD(this) + Offset::Entity::m_aimPunchAngle);
	}

	Vector CBaseEntity::GetViewPunchAngle()
	{
		return *reinterpret_cast<Vector*>(DWORD(this) + Offset::Entity::m_viewPunchAngle);
	}

	Vector CBaseEntity::GetVelocity()
	{
		return *reinterpret_cast<Vector*>(DWORD(this) + Offset::Entity::m_vecVelocity);
	}

	Vector CBaseEntity::GetViewOffset()
	{
		return *reinterpret_cast<Vector*>(DWORD(this) + Offset::Entity::m_vecViewOffset);
	}

	Vector CBaseEntity::GetEyePosition()
	{
		return GetRenderOrigin() + GetViewOffset();
	}

	QAngle CBaseEntity::GetEyeAngles()
	{
		return *reinterpret_cast<QAngle*>(DWORD(this) + Offset::Entity::m_angEyeAngles);
	}

	Vector CBaseEntity::GetBonePosition(int nBone)
	{
		Vector vRet;
		matrix3x4_t MatrixArray[MAXSTUDIOBONES];

		if (!SetupBones(MatrixArray, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, Interfaces::GlobalVars()->curtime))
			return vRet;

		matrix3x4_t HitboxMatrix = MatrixArray[nBone];

		vRet = Vector(HitboxMatrix[0][3], HitboxMatrix[1][3], HitboxMatrix[2][3]);

		return vRet;
	}

	studiohdr_t* CBaseEntity::GetStudioModel()
	{
		const model_t* model = nullptr;

		model = GetModel();

		if (!model)
			return nullptr;

		studiohdr_t* pStudioModel = Interfaces::ModelInfo()->GetStudioModel(model);

		if (!pStudioModel)
			return nullptr;

		return pStudioModel;
	}

	mstudiobone_t* CBaseEntity::GetBone(int nBone)
	{
		mstudiobone_t* pBoneBox = nullptr;

		studiohdr_t* pStudioModel = GetStudioModel();

		if (!pStudioModel)
			return pBoneBox;

		mstudiobone_t* pBone = pStudioModel->pBone(nBone);

		if (!pBone)
			return nullptr;

		return pBone;
	}

	mstudiobbox_t* CBaseEntity::GetHitBox(int nHitbox)
	{
		if (nHitbox < 0 || nHitbox >= HITBOX_MAX)
			return nullptr;

		mstudiohitboxset_t* pHitboxSet = nullptr;
		mstudiobbox_t* pHitboxBox = nullptr;

		pHitboxSet = GetHitBoxSet();

		if (!pHitboxSet)
			return pHitboxBox;

		pHitboxBox = pHitboxSet->pHitbox(nHitbox);

		if (!pHitboxBox)
			return nullptr;

		return pHitboxBox;
	}

	mstudiohitboxset_t* CBaseEntity::GetHitBoxSet()
	{
		studiohdr_t* pStudioModel = nullptr;
		mstudiohitboxset_t* pHitboxSet = nullptr;

		pStudioModel = GetStudioModel();

		if (!pStudioModel)
			return pHitboxSet;

		pHitboxSet = pStudioModel->pHitboxSet(0);

		if (!pHitboxSet)
			return nullptr;

		return pHitboxSet;
	}

	Vector CBaseEntity::GetHitboxPosition(int nHitbox)
	{
		matrix3x4_t MatrixArray[MAXSTUDIOBONES];
		Vector vRet, vMin, vMax;

		vRet = Vector(0, 0, 0);

		mstudiobbox_t* pHitboxBox = GetHitBox(nHitbox);

		if (!pHitboxBox || !IsValid())
			return vRet;

		if (!SetupBones(MatrixArray, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, 0/*Interfaces::GlobalVars()->curtime*/))
			return vRet;

		if (!pHitboxBox->m_Bone || !pHitboxBox->m_vBbmin.IsValid() || !pHitboxBox->m_vBbmax.IsValid())
			return vRet;

		VectorTransform(pHitboxBox->m_vBbmin, MatrixArray[pHitboxBox->m_Bone], vMin);
		VectorTransform(pHitboxBox->m_vBbmax, MatrixArray[pHitboxBox->m_Bone], vMax);

		vRet = (vMin + vMax) * 0.5f;

		return vRet;
	}

	int CBaseViewModel::GetModelIndex()
	{
		// DT_BaseViewModel -> m_nModelIndex
		return *reinterpret_cast<int*>(DWORD(this) + Offset::Entity::m_nModelIndex);
	}

	void CBaseViewModel::SetModelIndex(int nModelIndex)
	{
		VirtualFn(void)(PVOID, int);
		GetMethod<OriginalFn>(this, 75)(this, nModelIndex);
		// DT_BaseViewModel -> m_nModelIndex
		//*(int*)( ( DWORD )this + Offset::Entity::m_nModelIndex ) = nModelIndex;
	}

	void CBaseViewModel::SetWeaponModel(const char* Filename, IClientEntity* Weapon)
	{
		typedef void (__thiscall* SetWeaponModelFn)(void*, const char*, IClientEntity*);
		return GetMethod<SetWeaponModelFn>(this, 242)(this, Filename, Weapon);
	}

	DWORD CBaseViewModel::GetOwner()
	{
		// DT_BaseViewModel -> m_hOwner
		return *reinterpret_cast<PDWORD>(DWORD(this) + Offset::Entity::m_hOwner);
	}

	DWORD CBaseViewModel::GetWeapon()
	{
		// DT_BaseViewModel -> m_hWeapon
		return *reinterpret_cast<PDWORD>(DWORD(this) + Offset::Entity::m_hWeapon);
	}
}

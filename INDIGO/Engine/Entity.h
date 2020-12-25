#pragma once

#include "Engine.h"
#include "Offset.h"

namespace Engine {
	class CBaseWeapon;
	class CBaseViewModel;

	class CBaseEntity : public IClientEntity {
	public:
		//[swap_lines]
		char*	GetPlayerName();

		bool	IsPlayer();
		bool	IsValid();
		bool	IsDead();
		Vector  GetOrigin();
		bool	IsVisible(CBaseEntity* pLocalEntity);
		int     GetMoveType();
		bool	HasHelmet();
		bool	HasDefuser();
		bool*	IsSpotted();
		int     IsDefusing();
		int		IsFlashed();
		bool*   IsReloading();

		int		GetFovStart();
		int		GetFlags();
		int		GetHealth();
		int		GetArmor();
		int		GetTeam();
		float	GetLowerBodyYaw();
		float   GetFlashDuration();
		float	GetSimTime();
		int		GetShotsFired();
		int		GetIsScoped();
		int		GetTickBase();
		ObserverMode_t GetObserverMode();
		PVOID GetObserverTarget();

		PVOID			GetActiveWeapon();
		CBaseWeapon*	GetBaseWeapon();
		UINT*			GetWeapons();
		UINT*			GetWearables();
		CBaseViewModel* GetViewModel();

		Vector * GetVAngles();
		Vector	GetAimPunchAngle();
		Vector	GetViewPunchAngle();
		Vector	GetVelocity();
		Vector	GetViewOffset();
		Vector	GetEyePosition();

		QAngle	GetEyeAngles();

		Vector	GetBonePosition(int nBone);
		Vector	GetHitboxPosition(int nHitbox);

		studiohdr_t* GetStudioModel();
		mstudiobone_t* GetBone(int nBone);
		mstudiobbox_t* GetHitBox(int nHitbox);
		mstudiohitboxset_t* GetHitBoxSet();
		//[/swap_lines]
	};

	class CBaseViewModel : public IClientEntity {
	public:
		//[swap_lines]
		int		GetModelIndex();
		void	SetModelIndex(int nModelIndex);
		void	SetWeaponModel(const char* Filename, IClientEntity* Weapon);

		DWORD	GetOwner();
		DWORD	GetWeapon();
		//[/swap_lines]
	};

	enum PLAYER_TEAM {
		TEAM_SPEC = 1,
		TEAM_TT,
		TEAM_CT
	};

	//14 Dec 2020 - ClientClass - ClassID_t
	enum class CLIENT_CLASS_ID {
		CTestTraceline = 224,
		CTEWorldDecal = 225,
		CTESpriteSpray = 222,
		CTESprite = 221,
		CTESparks = 220,
		CTESmoke = 219,
		CTEShowLine = 217,
		CTEProjectedDecal = 214,
		CFEPlayerDecal = 71,
		CTEPlayerDecal = 213,
		CTEPhysicsProp = 210,
		CTEParticleSystem = 209,
		CTEMuzzleFlash = 208,
		CTELargeFunnel = 206,
		CTEKillPlayerAttachments = 205,
		CTEImpact = 204,
		CTEGlowSprite = 203,
		CTEShatterSurface = 216,
		CTEFootprintDecal = 200,
		CTEFizz = 199,
		CTEExplosion = 197,
		CTEEnergySplash = 196,
		CTEEffectDispatch = 195,
		CTEDynamicLight = 194,
		CTEDecal = 192,
		CTEClientProjectile = 191,
		CTEBubbleTrail = 190,
		CTEBubbles = 189,
		CTEBSPDecal = 188,
		CTEBreakModel = 187,
		CTEBloodStream = 186,
		CTEBloodSprite = 185,
		CTEBeamSpline = 184,
		CTEBeamRingPoint = 183,
		CTEBeamRing = 182,
		CTEBeamPoints = 181,
		CTEBeamLaser = 180,
		CTEBeamFollow = 179,
		CTEBeamEnts = 178,
		CTEBeamEntPoint = 177,
		CTEBaseBeam = 176,
		CTEArmorRicochet = 175,
		CTEMetalSparks = 207,
		CSteamJet = 168,
		CSmokeStack = 158,
		DustTrail = 277,
		CFireTrail = 74,
		SporeTrail = 283,
		SporeExplosion = 282,
		RocketTrail = 280,
		SmokeTrail = 281,
		CPropVehicleDriveable = 145,
		ParticleSmokeGrenade = 279,
		CParticleFire = 117,
		MovieExplosion = 278,
		CTEGaussExplosion = 202,
		CEnvQuadraticBeam = 66,
		CEmbers = 55,
		CEnvWind = 70,
		CPrecipitation = 138,
		CPrecipitationBlocker = 139,
		CBaseTempEntity = 18,
		NextBotCombatCharacter = 0,
		CEconWearable = 54,
		CBaseAttributableItem = 4,
		CEconEntity = 53,
		CWeaponZoneRepulsor = 274,
		CWeaponXM1014 = 273,
		CWeaponTaser = 268,
		CTablet = 172,
		CSnowball = 159,
		CSmokeGrenade = 156,
		CWeaponShield = 266,
		CWeaponSG552 = 264,
		CSensorGrenade = 152,
		CWeaponSawedoff = 260,
		CWeaponNOVA = 256,
		CIncendiaryGrenade = 99,
		CMolotovGrenade = 113,
		CMelee = 112,
		CWeaponM3 = 248,
		CKnifeGG = 108,
		CKnife = 107,
		CHEGrenade = 96,
		CFlashbang = 77,
		CFists = 76,
		CWeaponElite = 239,
		CDecoyGrenade = 47,
		CDEagle = 46,
		CWeaponUSP = 272,
		CWeaponM249 = 247,
		CWeaponUMP45 = 271,
		CWeaponTMP = 270,
		CWeaponTec9 = 269,
		CWeaponSSG08 = 267,
		CWeaponSG556 = 265,
		CWeaponSG550 = 263,
		CWeaponScout = 262,
		CWeaponSCAR20 = 261,
		CSCAR17 = 150,
		CWeaponP90 = 259,
		CWeaponP250 = 258,
		CWeaponP228 = 257,
		CWeaponNegev = 255,
		CWeaponMP9 = 254,
		CWeaponMP7 = 253,
		CWeaponMP5Navy = 252,
		CWeaponMag7 = 251,
		CWeaponMAC10 = 250,
		CWeaponM4A1 = 249,
		CWeaponHKP2000 = 246,
		CWeaponGlock = 245,
		CWeaponGalilAR = 244,
		CWeaponGalil = 243,
		CWeaponG3SG1 = 242,
		CWeaponFiveSeven = 241,
		CWeaponFamas = 240,
		CWeaponBizon = 235,
		CWeaponAWP = 233,
		CWeaponAug = 232,
		CAK47 = 1,
		CWeaponCSBaseGun = 237,
		CWeaponCSBase = 236,
		CC4 = 34,
		CBumpMine = 32,
		CBumpMineProjectile = 33,
		CBreachCharge = 28,
		CBreachChargeProjectile = 29,
		CWeaponBaseItem = 234,
		CBaseCSGrenade = 8,
		CSnowballProjectile = 161,
		CSnowballPile = 160,
		CSmokeGrenadeProjectile = 157,
		CSensorGrenadeProjectile = 153,
		CMolotovProjectile = 114,
		CItem_Healthshot = 104,
		CItemDogtags = 106,
		CDecoyProjectile = 48,
		CPhysPropRadarJammer = 127,
		CPhysPropWeaponUpgrade = 128,
		CPhysPropAmmoBox = 125,
		CPhysPropLootCrate = 126,
		CItemCash = 105,
		CEnvGasCanister = 63,
		CDronegun = 50,
		CParadropChopper = 116,
		CSurvivalSpawnChopper = 171,
		CBRC4Target = 27,
		CInfoMapRegion = 102,
		CFireCrackerBlast = 72,
		CInferno = 100,
		CChicken = 36,
		CDrone = 49,
		CFootstepControl = 79,
		CCSGameRulesProxy = 39,
		CWeaponCubemap = 0,
		CWeaponCycler = 238,
		CTEPlantBomb = 211,
		CTEFireBullets = 198,
		CTERadioIcon = 215,
		CPlantedC4 = 129,
		CCSTeam = 43,
		CCSPlayerResource = 41,
		CCSPlayer = 40,
		CPlayerPing = 131,
		CCSRagdoll = 42,
		CTEPlayerAnimEvent = 212,
		CHostage = 97,
		CHostageCarriableProp = 98,
		CBaseCSGrenadeProjectile = 9,
		CHandleTest = 95,
		CTeamplayRoundBasedRulesProxy = 174,
		CSpriteTrail = 166,
		CSpriteOriented = 165,
		CSprite = 164,
		CRagdollPropAttached = 148,
		CRagdollProp = 147,
		CPropCounter = 142,
		CPredictedViewModel = 140,
		CPoseController = 136,
		CGrassBurn = 94,
		CGameRulesProxy = 93,
		CInfoLadderDismount = 101,
		CFuncLadder = 85,
		CTEFoundryHelpers = 201,
		CEnvDetailController = 61,
		CDangerZone = 44,
		CDangerZoneController = 45,
		CWorldVguiText = 276,
		CWorld = 275,
		CWaterLODControl = 231,
		CWaterBullet = 230,
		CMapVetoPickController = 110,
		CVoteController = 229,
		CVGuiScreen = 228,
		CPropJeep = 144,
		CPropVehicleChoreoGeneric = 0,
		CTriggerSoundOperator = 227,
		CBaseVPhysicsTrigger = 22,
		CTriggerPlayerMovement = 226,
		CBaseTrigger = 20,
		CTest_ProxyToggle_Networkable = 223,
		CTesla = 218,
		CBaseTeamObjectiveResource = 17,
		CTeam = 173,
		CSunlightShadowControl = 170,
		CSun = 169,
		CParticlePerformanceMonitor = 118,
		CSpotlightEnd = 163,
		CSpatialEntity = 162,
		CSlideshowDisplay = 155,
		CShadowControl = 154,
		CSceneEntity = 151,
		CRopeKeyframe = 149,
		CRagdollManager = 146,
		CPhysicsPropMultiplayer = 123,
		CPhysBoxMultiplayer = 121,
		CPropDoorRotating = 143,
		CBasePropDoor = 16,
		CDynamicProp = 52,
		CProp_Hallucination = 141,
		CPostProcessController = 137,
		CPointWorldText = 135,
		CPointCommentaryNode = 134,
		CPointCamera = 133,
		CPlayerResource = 132,
		CPlasma = 130,
		CPhysMagnet = 124,
		CPhysicsProp = 122,
		CStatueProp = 167,
		CPhysBox = 120,
		CParticleSystem = 119,
		CMovieDisplay = 115,
		CMaterialModifyControl = 111,
		CLightGlow = 109,
		CItemAssaultSuitUseable = 0,
		CItem = 0,
		CInfoOverlayAccessor = 103,
		CFuncTrackTrain = 92,
		CFuncSmokeVolume = 91,
		CFuncRotating = 90,
		CFuncReflectiveGlass = 89,
		CFuncOccluder = 88,
		CFuncMoveLinear = 87,
		CFuncMonitor = 86,
		CFunc_LOD = 81,
		CTEDust = 193,
		CFunc_Dust = 80,
		CFuncConveyor = 84,
		CFuncBrush = 83,
		CBreakableSurface = 31,
		CFuncAreaPortalWindow = 82,
		CFish = 75,
		CFireSmoke = 73,
		CEnvTonemapController = 69,
		CEnvScreenEffect = 67,
		CEnvScreenOverlay = 68,
		CEnvProjectedTexture = 65,
		CEnvParticleScript = 64,
		CFogController = 78,
		CEnvDOFController = 62,
		CCascadeLight = 35,
		CEnvAmbientLight = 60,
		CEntityParticleTrail = 59,
		CEntityFreezing = 58,
		CEntityFlame = 57,
		CEntityDissolve = 56,
		CDynamicLight = 51,
		CColorCorrectionVolume = 38,
		CColorCorrection = 37,
		CBreakableProp = 30,
		CBeamSpotlight = 25,
		CBaseButton = 5,
		CBaseToggle = 19,
		CBasePlayer = 15,
		CBaseFlex = 12,
		CBaseEntity = 11,
		CBaseDoor = 10,
		CBaseCombatCharacter = 6,
		CBaseAnimatingOverlay = 3,
		CBoneFollower = 26,
		CBaseAnimating = 2,
		CAI_BaseNPC = 0,
		CBeam = 24,
		CBaseViewModel = 21,
		CBaseParticleEntity = 14,
		CBaseGrenade = 13,
		CBaseCombatWeapon = 7,
		CBaseWeaponWorldModel = 23,
	};
}
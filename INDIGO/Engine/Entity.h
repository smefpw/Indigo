#pragma once

#include "Engine.h"

namespace Engine
{
	class CBaseWeapon;
	class CBaseViewModel;

	class CBaseEntity : public IClientEntity
	{
	public:
//[swap_lines]
		char*	GetPlayerName();

		bool	IsPlayer();
		bool	IsValid();
		bool	IsDead();
		Vector  GetOrigin();
		bool	IsVisible( CBaseEntity* pLocalEntity );

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

		Vector	GetBonePosition( int nBone );
		Vector	GetHitboxPosition( int nHitbox );

		studiohdr_t* GetStudioModel();

		mstudiobone_t* GetBone( int nBone );
		mstudiobbox_t* GetHitBox( int nHitbox );

		mstudiohitboxset_t* GetHitBoxSet();
//[/swap_lines]
	};

	class CBaseViewModel : public IClientEntity
	{
	public:
//[swap_lines]
		int		GetModelIndex();
		void	SetModelIndex( int nModelIndex );
		void	SetWeaponModel( const char* Filename , IClientEntity* Weapon );

		DWORD	GetOwner();
		DWORD	GetWeapon();
//[/swap_lines]
	};

	enum PLAYER_TEAM
	{
		TEAM_SPEC = 1 ,
		TEAM_TT ,
		TEAM_CT
	};

	enum class CLIENT_CLASS_ID {
		NextBotCombatCharacter = 0,
		CWeaponCubemap = 0,
		CPropVehicleChoreoGeneric = 0,
		CItemAssaultSuitUseable = 0,
		CItem = 0,
		CAI_BaseNPC = 0,
		CAK47 = 1,
		CBaseAnimating = 2,
		CBaseAnimatingOverlay = 3,
		CBaseAttributableItem = 4,
		CBaseButton = 5,
		CBaseCombatCharacter = 6,
		CBaseCombatWeapon = 7,
		CBaseCSGrenade = 8,
		CBaseCSGrenadeProjectile = 9,
		CBaseDoor = 10,
		CBaseEntity = 11,
		CBaseFlex = 12,
		CBaseGrenade = 13,
		CBaseParticleEntity = 14,
		CBasePlayer = 15,
		CBasePropDoor = 16,
		CBaseTeamObjectiveResource = 17,
		CBaseTempEntity = 18,
		CBaseToggle = 19,
		CBaseTrigger = 20,
		CBaseViewModel = 21,
		CBaseVPhysicsTrigger = 22,
		CBeam = 24,
		CBeamSpotlight = 25,
		CBoneFollower = 26,
		CBRC4Target = 27,
		CBreachCharge = 28,
		CBreachChargeProjectile = 29,
		CBreakableProp = 30,
		CBreakableSurface = 31,
		CC4 = 32,
		CCascadeLight = 33,
		CChicken = 34,
		CColorCorrection = 35,
		CColorCorrectionVolume = 36,
		CCSGameRulesProxy = 37,
		CCSPlayer = 38,
		CCSPlayerResource = 39,
		CCSRagdoll = 40,
		CCSTeam = 41,
		CDangerZone = 42,
		CDangerZoneController = 43,
		CDEagle = 44,
		CDecoyGrenade = 45,
		CDecoyProjectile = 46,
		CDrone = 47,
		CDronegun = 48,
		CDynamicLight = 49,
		CDynamicProp = 50,
		CEconEntity = 51,
		CEconWearable = 52,
		CEmbers = 53,
		CEntityDissolve = 54,
		CEntityFlame = 55,
		CEntityFreezing = 56,
		CEntityParticleTrail = 57,
		CEnvAmbientLight = 58,
		CEnvDetailController = 59,
		CEnvDOFController = 60,
		CEnvGasCanister = 61,
		CEnvParticleScript = 62,
		CEnvProjectedTexture = 63,
		CEnvQuadraticBeam = 64,
		CEnvScreenEffect = 65,
		CEnvScreenOverlay = 66,
		CEnvTonemapController = 67,
		CEnvWind = 68,
		CFEPlayerDecal = 69,
		CFireCrackerBlast = 70,
		CFireSmoke = 71,
		CFireTrail = 72,
		CFish = 73,
		CFists = 74,
		CFlashbang = 75,
		CFogController = 76,
		CFootstepControl = 77,
		CFunc_Dust = 78,
		CFunc_LOD = 79,
		CFuncAreaPortalWindow = 80,
		CFuncBrush = 81,
		CFuncConveyor = 82,
		CFuncLadder = 83,
		CFuncMonitor = 84,
		CFuncMoveLinear = 85,
		CFuncOccluder = 86,
		CFuncReflectiveGlass = 87,
		CFuncRotating = 88,
		CFuncSmokeVolume = 89,
		CFuncTrackTrain = 90,
		CGameRulesProxy = 91,
		CGrassBurn = 92,
		CHandleTest = 93,
		CHEGrenade = 94,
		CHostage = 95,
		CHostageCarriableProp = 96,
		CIncendiaryGrenade = 97,
		CInferno = 98,
		CInfoLadderDismount = 99,
		CInfoMapRegion = 100,
		CInfoOverlayAccessor = 101,
		CItem_Healthshot = 102,
		CItemCash = 103,
		CItemDogtags = 104,
		CKnife = 105,
		CKnifeGG = 106,
		CLightGlow = 107,
		CMaterialModifyControl = 108,
		CMelee = 109,
		CMolotovGrenade = 110,
		CMolotovProjectile = 111,
		CMovieDisplay = 112,
		CParadropChopper = 113,
		CParticleFire = 114,
		CParticlePerformanceMonitor = 115,
		CParticleSystem = 116,
		CPhysBox = 117,
		CPhysBoxMultiplayer = 118,
		CPhysicsProp = 119,
		CPhysicsPropMultiplayer = 120,
		CPhysMagnet = 121,
		CPhysPropAmmoBox = 122,
		CPhysPropLootCrate = 123,
		CPhysPropRadarJammer = 124,
		CPhysPropWeaponUpgrade = 125,
		CPlantedC4 = 126,
		CPlasma = 127,
		CPlayerResource = 128,
		CPointCamera = 129,
		CPointCommentaryNode = 130,
		CPointWorldText = 131,
		CPoseController = 132,
		CPostProcessController = 133,
		CPrecipitation = 134,
		CPrecipitationBlocker = 135,
		CPredictedViewModel = 136,
		CProp_Hallucination = 137,
		CPropCounter = 138,
		CPropDoorRotating = 139,
		CPropJeep = 140,
		CPropVehicleDriveable = 141,
		CRagdollManager = 142,
		CRagdollProp = 143,
		CRagdollPropAttached = 144,
		CRopeKeyframe = 145,
		CSCAR17 = 146,
		CSceneEntity = 147,
		CSensorGrenade = 148,
		CSensorGrenadeProjectile = 149,
		CShadowControl = 150,
		CSlideshowDisplay = 151,
		CSmokeGrenade = 152,
		CSmokeGrenadeProjectile = 153,
		CSmokeStack = 154,
		CSpatialEntity = 155,
		CSpotlightEnd = 156,
		CSprite = 157,
		CSpriteOriented = 158,
		CSpriteTrail = 159,
		CStatueProp = 160,
		CSteamJet = 161,
		CSun = 162,
		CSunlightShadowControl = 163,
		CSurvivalSpawnChopper = 164,
		CTablet = 165,
		CTeam = 166,
		CTeamplayRoundBasedRulesProxy = 167,
		CTEArmorRicochet = 168,
		CTEBaseBeam = 169,
		CTEBeamEntPoint = 170,
		CTEBeamEnts = 171,
		CTEBeamFollow = 172,
		CTEBeamLaser = 173,
		CTEBeamPoints = 174,
		CTEBeamRing = 175,
		CTEBeamRingPoint = 176,
		CTEBeamSpline = 177,
		CTEBloodSprite = 178,
		CTEBloodStream = 179,
		CTEBreakModel = 180,
		CTEBSPDecal = 181,
		CTEBubbles = 182,
		CTEBubbleTrail = 183,
		CTEClientProjectile = 184,
		CTEDecal = 185,
		CTEDust = 186,
		CTEDynamicLight = 187,
		CTEEffectDispatch = 188,
		CTEEnergySplash = 189,
		CTEExplosion = 190,
		CTEFireBullets = 191,
		CTEFizz = 192,
		CTEFootprintDecal = 193,
		CTEFoundryHelpers = 194,
		CTEGaussExplosion = 195,
		CTEGlowSprite = 196,
		CTEImpact = 197,
		CTEKillPlayerAttachments = 198,
		CTELargeFunnel = 199,
		CTEMetalSparks = 200,
		CTEMuzzleFlash = 201,
		CTEParticleSystem = 202,
		CTEPhysicsProp = 203,
		CTEPlantBomb = 204,
		CTEPlayerAnimEvent = 205,
		CTEPlayerDecal = 206,
		CTEProjectedDecal = 207,
		CTERadioIcon = 208,
		CTEShatterSurface = 209,
		CTEShowLine = 210,
		CTesla = 211,
		CTESmoke = 212,
		CTESparks = 213,
		CTESprite = 214,
		CTESpriteSpray = 215,
		CTest_ProxyToggle_Networkable = 216,
		CTestTraceline = 217,
		CTEWorldDecal = 218,
		CTriggerPlayerMovement = 219,
		CTriggerSoundOperator = 220,
		CVGuiScreen = 221,
		CVoteController = 222,
		CWaterBullet = 223,
		CWaterLODControl = 224,
		CWeaponAug = 225,
		CWeaponAWP = 226,
		CWeaponBaseItem = 227,
		CWeaponBizon = 228,
		CWeaponCSBase = 229,
		CWeaponCSBaseGun = 230,
		CWeaponCycler = 231,
		CWeaponElite = 232,
		CWeaponFamas = 233,
		CWeaponFiveSeven = 234,
		CWeaponG3SG1 = 235,
		CWeaponGalil = 236,
		CWeaponGalilAR = 237,
		CWeaponGlock = 238,
		CWeaponHKP2000 = 239,
		CWeaponM249 = 240,
		CWeaponM3 = 241,
		CWeaponM4A1 = 242,
		CWeaponMAC10 = 243,
		CWeaponMag7 = 244,
		CWeaponMP5Navy = 245,
		CWeaponMP7 = 246,
		CWeaponMP9 = 247,
		CWeaponNegev = 248,
		CWeaponNOVA = 249,
		CWeaponP228 = 250,
		CWeaponP250 = 251,
		CWeaponP90 = 252,
		CWeaponSawedoff = 253,
		CWeaponSCAR20 = 254,
		CWeaponScout = 255,
		CWeaponSG550 = 256,
		CWeaponSG552 = 257,
		CWeaponSG556 = 258,
		CWeaponSSG08 = 259,
		CWeaponTaser = 260,
		CWeaponTec9 = 261,
		CWeaponTMP = 262,
		CWeaponUMP45 = 263,
		CWeaponUSP = 264,
		CWeaponXM1014 = 265,
		CWorld = 266,
		CWorldVguiText = 267,
		DustTrail = 268,
		MovieExplosion = 269,
		ParticleSmokeGrenade = 270,
		RocketTrail = 271,
		SmokeTrail = 272,
		SporeExplosion = 273,
		SporeTrail = 274,
	};
}
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
		Vector GetOrigin();
		bool	IsVisible( CBaseEntity* pLocalEntity );

		bool	HasHelmet();
		bool	HasDefuser();
		bool*	IsSpotted();

		int		GetFovStart();
		int		GetFlags();
		int		GetHealth();
		int		GetArmor();
		int		GetTeam();
		float GetLowerBodyYaw();
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

	enum class CLIENT_CLASS_ID
	{
		CTestTraceline = 194 ,
		CTEWorldDecal = 195 ,
		CTESpriteSpray = 192 ,
		CTESprite = 191 ,
		CTESparks = 190 ,
		CTESmoke = 189 ,
		CTEShowLine = 187 ,
		CTEProjectedDecal = 184 ,
		CFEPlayerDecal = 61 ,
		CTEPlayerDecal = 183 ,
		CTEPhysicsProp = 180 ,
		CTEParticleSystem = 179 ,
		CTEMuzzleFlash = 178 ,
		CTELargeFunnel = 176 ,
		CTEKillPlayerAttachments = 175 ,
		CTEImpact = 174 ,
		CTEGlowSprite = 173 ,
		CTEShatterSurface = 186 ,
		CTEFootprintDecal = 170 ,
		CTEFizz = 169 ,
		CTEExplosion = 167 ,
		CTEEnergySplash = 166 ,
		CTEEffectDispatch = 165 ,
		CTEDynamicLight = 164 ,
		CTEDecal = 162 ,
		CTEClientProjectile = 161 ,
		CTEBubbleTrail = 160 ,
		CTEBubbles = 159 ,
		CTEBSPDecal = 158 ,
		CTEBreakModel = 157 ,
		CTEBloodStream = 156 ,
		CTEBloodSprite = 155 ,
		CTEBeamSpline = 154 ,
		CTEBeamRingPoint = 153 ,
		CTEBeamRing = 152 ,
		CTEBeamPoints = 151 ,
		CTEBeamLaser = 150 ,
		CTEBeamFollow = 149 ,
		CTEBeamEnts = 148 ,
		CTEBeamEntPoint = 147 ,
		CTEBaseBeam = 146 ,
		CTEArmorRicochet = 145 ,
		CTEMetalSparks = 177 ,
		CSteamJet = 140 ,
		CSmokeStack = 133 ,
		DustTrail = 244 ,
		CFireTrail = 64 ,
		SporeTrail = 250 ,
		SporeExplosion = 249 ,
		RocketTrail = 247 ,
		SmokeTrail = 248 ,
		CPropVehicleDriveable = 120 ,
		ParticleSmokeGrenade = 246 ,
		CParticleFire = 99 ,
		MovieExplosion = 245 ,
		CTEGaussExplosion = 172 ,
		CEnvQuadraticBeam = 56 ,
		CEmbers = 46 ,
		CEnvWind = 60 ,
		CPrecipitation = 114 ,
		CPrecipitationBlocker = 115 ,
		CBaseTempEntity = 18 ,
		NextBotCombatCharacter = 0 ,
		CEconWearable = 45 ,
		CBaseAttributableItem = 4 ,
		CEconEntity = 44 ,
		CWeaponXM1014 = 242 ,
		CWeaponTaser = 237 ,
		CSmokeGrenade = 131 ,
		CWeaponSG552 = 234 ,
		CSensorGrenade = 127 ,
		CWeaponSawedoff = 230 ,
		CWeaponNOVA = 226 ,
		CIncendiaryGrenade = 87 ,
		CMolotovGrenade = 96 ,
		CWeaponM3 = 218 ,
		CKnifeGG = 93 ,
		CKnife = 92 ,
		CHEGrenade = 84 ,
		CFlashbang = 66 ,
		CWeaponElite = 209 ,
		CDecoyGrenade = 40 ,
		CDEagle = 39 ,
		CWeaponUSP = 241 ,
		CWeaponM249 = 217 ,
		CWeaponUMP45 = 240 ,
		CWeaponTMP = 239 ,
		CWeaponTec9 = 238 ,
		CWeaponSSG08 = 236 ,
		CWeaponSG556 = 235 ,
		CWeaponSG550 = 233 ,
		CWeaponScout = 232 ,
		CWeaponSCAR20 = 231 ,
		CSCAR17 = 125 ,
		CWeaponP90 = 229 ,
		CWeaponP250 = 228 ,
		CWeaponP228 = 227 ,
		CWeaponNegev = 225 ,
		CWeaponMP9 = 224 ,
		CWeaponMP7 = 223 ,
		CWeaponMP5Navy = 222 ,
		CWeaponMag7 = 221 ,
		CWeaponMAC10 = 220 ,
		CWeaponM4A1 = 219 ,
		CWeaponHKP2000 = 216 ,
		CWeaponGlock = 215 ,
		CWeaponGalilAR = 214 ,
		CWeaponGalil = 213 ,
		CWeaponG3SG1 = 212 ,
		CWeaponFiveSeven = 211 ,
		CWeaponFamas = 210 ,
		CWeaponBizon = 205 ,
		CWeaponAWP = 203 ,
		CWeaponAug = 202 ,
		CAK47 = 1 ,
		CWeaponCSBaseGun = 207 ,
		CWeaponCSBase = 206 ,
		CC4 = 29 ,
		CWeaponBaseItem = 204 ,
		CBaseCSGrenade = 8 ,
		CSmokeGrenadeProjectile = 132 ,
		CSensorGrenadeProjectile = 128 ,
		CMolotovProjectile = 97 ,
		CItem_Healthshot = 91 ,
		CDecoyProjectile = 41 ,
		CFireCrackerBlast = 62 ,
		CInferno = 88 ,
		CChicken = 31 ,
		CFootstepControl = 68 ,
		CCSGameRulesProxy = 34 ,
		CWeaponCubemap = 0 ,
		CWeaponCycler = 208 ,
		CTEPlantBomb = 181 ,
		CTEFireBullets = 168 ,
		CTERadioIcon = 185 ,
		CPlantedC4 = 108 ,
		CCSTeam = 38 ,
		CCSPlayerResource = 36 ,
		CCSPlayer = 35 ,
		CCSRagdoll = 37 ,
		CTEPlayerAnimEvent = 182 ,
		CHostage = 85 ,
		CHostageCarriableProp = 86 ,
		CBaseCSGrenadeProjectile = 9 ,
		CHandleTest = 83 ,
		CTeamplayRoundBasedRulesProxy = 144 ,
		CSpriteTrail = 138 ,
		CSpriteOriented = 137 ,
		CSprite = 136 ,
		CRagdollPropAttached = 123 ,
		CRagdollProp = 122 ,
		CPredictedViewModel = 116 ,
		CPoseController = 112 ,
		CGameRulesProxy = 82 ,
		CInfoLadderDismount = 89 ,
		CFuncLadder = 74 ,
		CTEFoundryHelpers = 171 ,
		CEnvDetailController = 52 ,
		CWorld = 243 ,
		CWaterLODControl = 201 ,
		CWaterBullet = 200 ,
		CVoteController = 199 ,
		CVGuiScreen = 198 ,
		CPropJeep = 119 ,
		CPropVehicleChoreoGeneric = 0 ,
		CTriggerSoundOperator = 197 ,
		CBaseVPhysicsTrigger = 22 ,
		CTriggerPlayerMovement = 196 ,
		CBaseTrigger = 20 ,
		CTest_ProxyToggle_Networkable = 193 ,
		CTesla = 188 ,
		CBaseTeamObjectiveResource = 17 ,
		CTeam = 143 ,
		CSunlightShadowControl = 142 ,
		CSun = 141 ,
		CParticlePerformanceMonitor = 100 ,
		CSpotlightEnd = 135 ,
		CSpatialEntity = 134 ,
		CSlideshowDisplay = 130 ,
		CShadowControl = 129 ,
		CSceneEntity = 126 ,
		CRopeKeyframe = 124 ,
		CRagdollManager = 121 ,
		CPhysicsPropMultiplayer = 105 ,
		CPhysBoxMultiplayer = 103 ,
		CPropDoorRotating = 118 ,
		CBasePropDoor = 16 ,
		CDynamicProp = 43 ,
		CProp_Hallucination = 117 ,
		CPostProcessController = 113 ,
		CPointCommentaryNode = 111 ,
		CPointCamera = 110 ,
		CPlayerResource = 109 ,
		CPlasma = 108 ,
		CPhysMagnet = 106 ,
		CPhysicsProp = 104 ,
		CStatueProp = 139 ,
		CPhysBox = 102 ,
		CParticleSystem = 101 ,
		CMovieDisplay = 98 ,
		CMaterialModifyControl = 95 ,
		CLightGlow = 94 ,
		CInfoOverlayAccessor = 90 ,
		CFuncTrackTrain = 81 ,
		CFuncSmokeVolume = 80 ,
		CFuncRotating = 79 ,
		CFuncReflectiveGlass = 78 ,
		CFuncOccluder = 77 ,
		CFuncMoveLinear = 76 ,
		CFuncMonitor = 75 ,
		CFunc_LOD = 70 ,
		CTEDust = 163 ,
		CFunc_Dust = 69 ,
		CFuncConveyor = 73 ,
		CFuncBrush = 72 ,
		CBreakableSurface = 28 ,
		CFuncAreaPortalWindow = 71 ,
		CFish = 65 ,
		CFireSmoke = 63 ,
		CEnvTonemapController = 59 ,
		CEnvScreenEffect = 57 ,
		CEnvScreenOverlay = 58 ,
		CEnvProjectedTexture = 55 ,
		CEnvParticleScript = 54 ,
		CFogController = 67 ,
		CEnvDOFController = 53 ,
		CCascadeLight = 30 ,
		CEnvAmbientLight = 51 ,
		CEntityParticleTrail = 50 ,
		CEntityFreezing = 49 ,
		CEntityFlame = 48 ,
		CEntityDissolve = 47 ,
		CDynamicLight = 42 ,
		CColorCorrectionVolume = 33 ,
		CColorCorrection = 32 ,
		CBreakableProp = 27 ,
		CBeamSpotlight = 25 ,
		CBaseButton = 5 ,
		CBaseToggle = 19 ,
		CBasePlayer = 15 ,
		CBaseFlex = 12 ,
		CBaseEntity = 11 ,
		CBaseDoor = 10 ,
		CBaseCombatCharacter = 6 ,
		CBaseAnimatingOverlay = 3 ,
		CBoneFollower = 26 ,
		CBaseAnimating = 2 ,
		CAI_BaseNPC = 0 ,
		CBeam = 24 ,
		CBaseViewModel = 21 ,
		CBaseParticleEntity = 14 ,
		CBaseGrenade = 13 ,
		CBaseCombatWeapon = 7 ,
	};
}
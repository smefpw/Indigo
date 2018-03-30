#pragma once

#include "../Main/Main.h"

#include "Weapon.h"
#include "Entity.h"
#include "Hook.h"
#include "NetVar.h"
#include "Offset.h"
#include "Render.h"
#include "Players.h"

//[enc_string_enable /]

#define VGUI2_DLL "vgui2.dll"
#define VGUIMAT_DLL	"vguimatsurface.dll"
#define D3D9_DLL "d3d9.dll"
#define SHADERPIDX9_DLL "shaderapidx9.dll"
#define GAMEOVERLAYRENDERER_DLL "GameOverlayRenderer.dll"
#define D3D9_MASK "x????xxxxxx"
#define GMOR_MASK "x????x????xxxx????xxxxxxx"
#define FORCE_FULL_UPDATE_MASK "x????xxxxxxxxxxxxxxx"

//[enc_string_disable /]

#define D3D9_PATTERN "\xA1\x00\x00\x00\x00\x50\x8B\x08\xFF\x51\x0C"
#define FORCE_FULL_UPDATE_PATTERN "\xA1\x00\x00\x00\x00\xB9\x00\x00\x00\x00\x56\xFF\x50\x14\x8B\x34\x85"
#define GMOR_PATTERN "\xA3\x00\x00\x00\x00\x68\x00\x00\x00\x00\xFF\x76\x54\xE8\x00\x00\x00\x00\x83\xC4\x08\x84\xC0\x75\x17"
#define SMOK_PATTERN "\x55\x8B\xEC\x83\xEC\x08\x8B\x15\x00\x00\x00\x00\x0F\x57\xC0"

typedef void(*LPSEARCHFUNC)(LPCTSTR lpszFileName);

namespace Engine
{
	enum WEAPON_TYPE;
	//[swap_lines]
	bool		Initialize();
	void		Shutdown();
	WEAPON_TYPE GetWeaponType(int iItemDefinitionIndex);
	bool		IsLocalAlive();
	void		ClanTag();
	void		ChatSpamRegular();
	void		ChatSpamRandom();
	void		ForceFullUpdate();
	int			GetWeaponSettingsSelectID();
	bool		ScanColorFromCvar(const char* str, float* colors);
	bool		WorldToScreen(const Vector& vOrigin, Vector& vScreen);
	bool		GetVisibleOrigin(const Vector& vOrigin);
	void		AngleVectors(const Vector &vAngles, Vector& vForward);
	Vector AngleVector(QAngle meme);
	float	DistancePointToLine(Vector Point, Vector LineOrigin, Vector Dir);
	void		VectorAngles(const Vector vForward, Vector& vAngle);
	void		AngleNormalize(Vector& vAngles);
	void		SmoothAngles(Vector MyViewAngles, Vector AimAngles, Vector &OutAngles, float Smoothing);
	float		DistanceScreen(Vector2D vDstPos, Vector2D vSrcPos);
	bool		LineGoesThroughSmoke(Vector vStartPos, Vector vEndPos);
	bool		CheckPlayerFov(Vector vScreen, int iFov);
	bool		CheckPlayerFov(Vector vScreen, int iFovX, int iFovY);
	IMaterial*	CreateMaterial(bool bFlat, bool bShouldIgnoreZ);
	void		ForceMaterial(Color color, IMaterial* material, bool useColor = true, bool forceMaterial = true);
	BOOL SearchFiles(LPCTSTR lpszFileName, LPSEARCHFUNC lpSearchFunc, BOOL bInnerFolders);
	//[/swap_lines]
}

class CTimer
{
public:
	ULONGLONG dwTime;
	bool bEnable;
	CTimer()
	{
		dwTime = 0;
		bEnable = true;
	}
	bool delay(DWORD dwMsec);
	void reset();
	void stop();
};
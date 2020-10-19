#pragma once

#include "../Main/Main.h"

#include "Weapon.h"
#include "Entity.h"
#include "Hook.h"
#include "NetVar.h"
#include "Offset.h"
#include "Render.h"
#include "Players.h"
#include "EventLogs.h"
#include "../License/License.h"

//[enc_string_enable /]

#define VGUI2_DLL "vgui2.dll"
#define VGUIMAT_DLL	"vguimatsurface.dll"
#define D3D9_DLL "d3d9.dll"
#define SHADERPIDX9_DLL "shaderapidx9.dll"
#define GAMEOVERLAYRENDERER_DLL "GameOverlayRenderer.dll"

//all latest as of 19 October 2020
#define D3D9_MASK "x????xxxxxx"
#define GMOR_MASK "x????x????xxxx????xxx"
#define D3D9_PATTERN "\xA1\x00\x00\x00\x00\x50\x8B\x08\xFF\x51\x0C"
#define GMOR_PATTERN "\x68\x00\x00\x00\x00\x68\x00\x00\x00\x00\xFF\x76\x44\xE8\x00\x00\x00\x00\x83\xC4\x10"


//[enc_string_disable /]

typedef void(*LPSEARCHFUNC)(LPCTSTR lpszFileName);

template<typename... Args> extern void PrintToConsole(const char* format, Args... args)
{
	Interfaces::GetConVar()->ConsoleColorPrintf(Color(242, 34, 214, 255), "\x5B\x73\x6D\x65\x66\x2E\x63\x63\x5D\x20"); // pls no rename me sad!!!!!!
	Interfaces::GetConVar()->ConsolePrintf(format, args...);
	Interfaces::GetConVar()->ConsolePrintf("\n");
};
template<class T, class U> T clampMinMax(T in, U low, U high)
{
	if (in <= low)
		return low;
	if (in >= high)
		return high;
	return in;
}
namespace Engine
{
	enum WEAPON_TYPE;
	//[swap_lines]
	void        ClanTagApply(const char* TagName);
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
	void		correct_movement(QAngle vOldAngles, CUserCmd * pCmd, float fOldForward, float fOldSidemove);
	bool		sanitize_angles(QAngle & angles);
	bool		GetVisibleOrigin(const Vector& vOrigin);
	void		AngleVectors(const Vector &vAngles, Vector& vForward);
	Vector		AngleVector(QAngle meme);
	float		DistancePointToLine(Vector Point, Vector LineOrigin, Vector Dir);
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
	extern bool stub_68616b65;
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
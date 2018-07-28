#pragma once

#include "Engine.h"
using FrameStageNotify = void(__thiscall*)(IBaseClientDLL*, ClientFrameStage_t);
using CreateMove = void(__thiscall *)(IClientMode*, float flInputSampleTime, CUserCmd* pCmd);
using GameEvent = bool(__thiscall*)(void*, IGameEvent*);
using ResetHook = void(__thiscall *)(LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);
using GetFov = void(__thiscall *)(CViewSetup*);
using PlaySound = void(__thiscall *)(ISurface*, const char*);
using EmitsoundXD = void(__stdcall*)(IRecipientFilter& filter, int iEntIndex, int iChannel, const char *pSoundEntry, unsigned int nSoundEntryHash, const char *pSample,
	float flVolume, soundlevel_t iSoundlevel, int nSeed, int iFlags, int iPitch,
	const Vector *pOrigin, const Vector *pDirection, CUtlVector< Vector >* pUtlVecOrigins, bool bUpdatePositions, float soundtime, int speakerentity, int test);
namespace Engine
{
	namespace Hook
	{
		bool Initialize();
		void Shutdown();
	}
}
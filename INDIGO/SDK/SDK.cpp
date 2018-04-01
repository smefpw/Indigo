#include "SDK.h"

//[enc_string_enable /]

namespace SDK
{
	//[swap_lines]
	IVEngineClient*     Interfaces::g_pEngine = nullptr;
	IBaseClientDLL*     Interfaces::g_pClient = nullptr;
	IClientEntityList*  Interfaces::g_pEntityList = nullptr;
	CGlobalVarsBase*    Interfaces::g_pGlobals = nullptr;
	CInput*             Interfaces::g_pInput = nullptr;
	IEngineTrace*       Interfaces::g_pEngineTrace = nullptr;
	IClientMode*        Interfaces::g_pClientMode = nullptr;
	IVModelInfoClient*	Interfaces::g_pModelInfo = nullptr;
	IEngineSound*		Interfaces::g_pSound = nullptr;
	IVModelRender*		Interfaces::g_pModelRender = nullptr;
	IVRenderView*		Interfaces::g_pRenderView = nullptr;
	IMaterialSystem*	Interfaces::g_pMaterialSystem = nullptr;
	ISurface*			Interfaces::g_pSurface = nullptr;
	IGameEventManager2*	Interfaces::g_pGameEventMgr = nullptr;
	IInputSystem*		Interfaces::g_pInputSystem = nullptr;
	ConVar*             Interfaces::g_pConVar = nullptr;
	CEffects*			Interfaces::g_pEffects = nullptr;
	ILocalize*          Interfaces::g_pILocalize = nullptr;
	ISteamGameCoordinator* Interfaces::g_pSteamGameCoordinator = nullptr;
	ISteamUser* Interfaces::g_pSteamUser = nullptr;
	//[/swap_lines]
	//[junk_enable /]
	CreateInterfaceFn CaptureFactory(char* FactoryModule)
	{
		CreateInterfaceFn Interface = 0;

		while (!Interface)
		{
			HMODULE hFactoryModule = GetModuleHandleA(FactoryModule);

			if (hFactoryModule)
			{
				Interface = (CreateInterfaceFn)(GetProcAddress(hFactoryModule, "CreateInterface"));
			}

			Sleep(100);
		}

		return Interface;
	}

	template<typename T>
	T* CaptureInterface(CreateInterfaceFn Interface, char* InterfaceName)
	{
		T* dwPointer = nullptr;

		while (!dwPointer)
		{
			dwPointer = (T*)(Interface(InterfaceName, 0));

			Sleep(100);
		}

		return dwPointer;
	}

	IVEngineClient* Interfaces::Engine()
	{
		if (!g_pEngine)
		{
			CreateInterfaceFn pfnFactory = CaptureFactory(ENGINE_DLL);
			g_pEngine = CaptureInterface<IVEngineClient>(pfnFactory, VENGINE_CLIENT_INTERFACE_VERSION);
#if ENABLE_DEBUG_FILE == 1
			CSX::Log::Add("::g_pEngine = %X", g_pEngine);
#endif
		}

		return g_pEngine;
	}

	IBaseClientDLL* Interfaces::Client()
	{
		if (!g_pClient)
		{
			CreateInterfaceFn pfnFactory = CaptureFactory(CLIENT_DLL);
			g_pClient = CaptureInterface<IBaseClientDLL>(pfnFactory, CLIENT_DLL_INTERFACE_VERSION);

			while (!((DWORD)**(IClientMode***)((*(uint32_t**)Client())[10] + 0x5)) || !((DWORD)**(CGlobalVarsBase***)((*(uint32_t**)Client())[0] + 0x1B)))
			{
				Sleep(1000);
			}

#if ENABLE_DEBUG_FILE == 1
			CSX::Log::Add("::g_pClient = %X", g_pClient);
#endif
		}

		return g_pClient;
	}

	IClientEntityList* Interfaces::EntityList()
	{
		if (!g_pEntityList)
		{
			CreateInterfaceFn pfnFactory = CaptureFactory(CLIENT_DLL);
			g_pEntityList = CaptureInterface<IClientEntityList>(pfnFactory, VCLIENTENTITYLIST_INTERFACE_VERSION);
#if ENABLE_DEBUG_FILE == 1
			CSX::Log::Add("::g_pEntityList = %X", g_pEntityList);
#endif
		}

		return g_pEntityList;
	}

	CGlobalVarsBase* Interfaces::GlobalVars()
	{
		if (!g_pGlobals)
		{
			auto pClientVFTable = *(uint32_t**)Client();
			g_pGlobals = **(CGlobalVarsBase***)(pClientVFTable[0] + 0x1B);
#if ENABLE_DEBUG_FILE == 1
			CSX::Log::Add("::g_pGlobals = %X", g_pGlobals);
#endif
		}

		return g_pGlobals;
	}

	CInput* Interfaces::Input()
	{
		if (!g_pInput)
		{
			auto pdwClient = *(PDWORD_PTR*)g_pClient;
			g_pInput = *(CInput**)(pdwClient[15] + 0x1);
#if ENABLE_DEBUG_FILE == 1
			CSX::Log::Add("::g_pInput = %X", g_pInput);
#endif
		}

		return g_pInput;
	}

	IEngineTrace* Interfaces::EngineTrace()
	{
		if (!g_pEngineTrace)
		{
			CreateInterfaceFn pfnFactory = CaptureFactory(ENGINE_DLL);
			g_pEngineTrace = CaptureInterface<IEngineTrace>(pfnFactory, INTERFACEVERSION_ENGINETRACE_CLIENT);
#if ENABLE_DEBUG_FILE == 1
			CSX::Log::Add("::g_pEngineTrace = %X", g_pEngineTrace);
#endif
		}

		return g_pEngineTrace;
	}

	IClientMode* Interfaces::ClientMode()
	{
		if (!g_pClientMode)
		{
			auto pClientVFTable = *(uint32_t**)Client();
			g_pClientMode = **(IClientMode***)(pClientVFTable[10] + 0x5);

#if ENABLE_DEBUG_FILE == 1
			CSX::Log::Add("::g_pClientMode = %X", g_pClientMode);
#endif
		}

		return g_pClientMode;
	}

	IVModelInfoClient* Interfaces::ModelInfo()
	{
		if (!g_pModelInfo)
		{
			CreateInterfaceFn pfnFactory = CaptureFactory(ENGINE_DLL);
			g_pModelInfo = CaptureInterface<IVModelInfoClient>(pfnFactory, VMODELINFO_CLIENT_INTERACE_VERSION);
#if ENABLE_DEBUG_FILE == 1
			CSX::Log::Add("::g_pModelInfo = %X", g_pModelInfo);
#endif
		}

		return g_pModelInfo;
	}

	IEngineSound* Interfaces::Sound()
	{
		if (!g_pSound)
		{
			CreateInterfaceFn pfnFactory = CaptureFactory(ENGINE_DLL);
			g_pSound = CaptureInterface<IEngineSound>(pfnFactory, IENGINESOUND_CLIENT_INTERFACE_VERSION);
#if ENABLE_DEBUG_FILE == 1
			CSX::Log::Add("::g_pSound = %X", g_pSound);
#endif
		}

		return g_pSound;
	}

	IVModelRender* Interfaces::ModelRender()
	{
		if (!g_pModelRender)
		{
			CreateInterfaceFn pfnFactory = CaptureFactory(ENGINE_DLL);
			g_pModelRender = CaptureInterface<IVModelRender>(pfnFactory, VENGINE_HUDMODEL_INTERFACE_VERSION);
#if ENABLE_DEBUG_FILE == 1
			CSX::Log::Add("::g_pModelRender = %X", g_pModelRender);
#endif
		}

		return g_pModelRender;
	}

	IVRenderView* Interfaces::RenderView()
	{
		if (!g_pRenderView)
		{
			CreateInterfaceFn pfnFactory = CaptureFactory(ENGINE_DLL);
			g_pRenderView = CaptureInterface<IVRenderView>(pfnFactory, VENGINE_RENDERVIEW_INTERFACE_VERSION);
#if ENABLE_DEBUG_FILE == 1
			CSX::Log::Add("::g_pRenderView = %X", g_pRenderView);
#endif
		}

		return g_pRenderView;
	}

	IMaterialSystem* Interfaces::MaterialSystem()
	{
		if (!g_pMaterialSystem)
		{
			CreateInterfaceFn pfnFactory = CaptureFactory(MATERIAL_DLL);
			g_pMaterialSystem = CaptureInterface<IMaterialSystem>(pfnFactory, MATERIAL_SYSTEM_INTERFACE_VERSION);
#if ENABLE_DEBUG_FILE == 1
			CSX::Log::Add("::g_pMaterialSystem = %X", g_pMaterialSystem);
#endif
		}

		return g_pMaterialSystem;
	}

	ISurface* Interfaces::Surface()
	{
		if (!g_pSurface)
		{
			CreateInterfaceFn pfnFactory = CaptureFactory(VGUIMT_DLL);
			g_pSurface = CaptureInterface<ISurface>(pfnFactory, VGUI_SURFACE_INTERFACE_VERSION);
#if ENABLE_DEBUG_FILE == 1
			CSX::Log::Add("::g_pSurface = %X", g_pSurface);
#endif
		}

		return g_pSurface;
	}

	IGameEventManager2*	Interfaces::GameEvent()
	{
		if (!g_pGameEventMgr)
		{
			CreateInterfaceFn pfnFactory = CaptureFactory(ENGINE_DLL);
			g_pGameEventMgr = CaptureInterface<IGameEventManager2>(pfnFactory, GAMEEVENTMANAGER_INTERFACE_VERSION);
#if ENABLE_DEBUG_FILE == 1
			CSX::Log::Add("::g_pGameEventMgr = %X", g_pGameEventMgr);
#endif
		}

		return g_pGameEventMgr;
	}

	IInputSystem* Interfaces::InputSystem()
	{
		if (!g_pInputSystem)
		{
			CreateInterfaceFn pfnFactory = CaptureFactory(INPUTSYSTEM_DLL);
			g_pInputSystem = CaptureInterface<IInputSystem>(pfnFactory, INPUTSYSTEM_INTERFACE_VERSION);
#if ENABLE_DEBUG_FILE == 1
			CSX::Log::Add("::g_pInputSystem = %X", g_pInputSystem);
#endif
		}

		return g_pInputSystem;
	}

	ISteamUser* Interfaces::SteamUser()
	{
		if (!g_pSteamUser) {
			SteamGameCoordinator();
		}
		return g_pSteamUser;
	}

	ISteamGameCoordinator* Interfaces::SteamGameCoordinator()
	{
		if (!g_pSteamGameCoordinator) {
			typedef uint32_t SteamPipeHandle;
			typedef uint32_t SteamUserHandle;
			SteamUserHandle hSteamUser = ((SteamUserHandle(__cdecl*)(void))GetProcAddress(GetModuleHandle("steam_api.dll"), "SteamAPI_GetHSteamUser"))();
			SteamPipeHandle hSteamPipe = ((SteamPipeHandle(__cdecl*)(void))GetProcAddress(GetModuleHandle("steam_api.dll"), "SteamAPI_GetHSteamPipe"))();
			auto SteamClient = ((ISteamClient*(__cdecl*)(void))GetProcAddress(GetModuleHandle("steam_api.dll"), "SteamClient"))();
			auto SteamHTTP = SteamClient->GetISteamHTTP(hSteamUser, hSteamPipe, "STEAMHTTP_INTERFACE_VERSION002");
			g_pSteamUser = SteamClient->GetISteamUser(hSteamUser, hSteamPipe, "SteamUser019");
			auto SteamFriends = SteamClient->GetISteamFriends(hSteamUser, hSteamPipe, "SteamFriends015");
			auto SteamInventory = SteamClient->GetISteamInventory(hSteamUser, hSteamPipe, "STEAMINVENTORY_INTERFACE_V002");
			g_pSteamGameCoordinator = (ISteamGameCoordinator*)SteamClient->GetISteamGenericInterface(hSteamUser, hSteamPipe, "SteamGameCoordinator001");
		}
		return g_pSteamGameCoordinator;
	}

	ConVar* Interfaces::GetConVar()
	{
		if (!g_pConVar)
		{
			CreateInterfaceFn pfnFactory = CaptureFactory(VSTDLIB_DLL);
			g_pConVar = CaptureInterface<ConVar>(pfnFactory, VENGINEVCAR_INTERFACE_VERSION);

		}

		return g_pConVar;
	}

	ILocalize* Interfaces::GetLocalize()
	{
		if (!g_pILocalize)
		{
			CreateInterfaceFn pfnFactory = CaptureFactory("localize.dll");
			g_pILocalize = CaptureInterface<ILocalize>(pfnFactory, "Localize_001");

		}

		return g_pILocalize;
	}

	CEffects* Interfaces::Effects()
	{
		if (!g_pEffects)
		{
			CreateInterfaceFn pfnFactory = CaptureFactory(ENGINE_DLL);
			g_pEffects = CaptureInterface<CEffects>(pfnFactory, VENGINEVEFFECTS_INTERFACE_VERSION);
		}
		return g_pEffects;
	}


}
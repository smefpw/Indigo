#include "Main.h"
#include "../RL/ReflectiveLoader.h"


//[enc_string_enable /]
//[junk_enable /]
DWORD WINAPI CheatEntry( LPVOID lpThreadParameter )
{
	HMODULE hModule = (HMODULE)lpThreadParameter;

	if ( Engine::Initialize() )
	{
		return 0;
	}

	return 0;
}

BOOL WINAPI DllMain( HINSTANCE hinstDLL , DWORD fdwReason , LPVOID lpReserved )
{
	if ( fdwReason == DLL_PROCESS_ATTACH )
	{
		DisableThreadLibraryCalls( hinstDLL );

		Client::BaseDir = CSX::Utils::GetHackWorkingDirectory().c_str();

		if ( Client::BaseDir.size() < 1 )
		{
			Client::BaseDir = CSX::Utils::GetModuleBaseDir( hinstDLL );
		}

#if ENABLE_DEBUG_FILE == 1
		AllocConsole();
		AttachConsole(GetCurrentProcessId());
		freopen("CONIN$", "r", stdin);
		freopen("CONOUT$", "w", stdout);
		freopen("CONOUT$", "w", stderr);

		Client::LogFile = Client::BaseDir + "\\debug.log";
		DeleteFileA( Client::LogFile.c_str() );
		CSX::Log::LogFile = Client::LogFile;
		printf("Client::BaseDir = %s\nClient::LogFile = %s\n", Client::BaseDir.c_str(), Client::LogFile.c_str());
		CSX::Log::Add( "::Init::" );

#endif

		CreateThread( 0 , 0 , CheatEntry , hinstDLL , 0 , 0 );
	}
	else if ( fdwReason == DLL_PROCESS_DETACH )
	{
		Engine::Shutdown();
	}

	return TRUE;
}
#include "SDK.h"

//11th March 2020
namespace SDK {
	bool KeyValues::LoadFromBuffer(KeyValues *pThis, const char *pszFirst, const char *pszSecond, PVOID pSomething, PVOID pAnother, PVOID pLast, PVOID New1Feb) {
		typedef bool(__thiscall *_LoadFromBuffer)(KeyValues*, const char*, const char*, PVOID, PVOID, PVOID, PVOID);
		static _LoadFromBuffer LoadFromBufferFn = 0;
		static bool SearchFunction = false;

		if (!SearchFunction) {
			//LoadFromBuffer
			static DWORD dwFunctionAddress = CSX::Memory::FindPattern(CLIENT_DLL, "55 8B EC 83 E4 F8 83 EC 34 53 8B 5D 0C 89", 0);
#if ENABLE_DEBUG_FILE == 1
			CSX::Log::Add("[FindPattern - LoadFromBuffer = %X]", dwFunctionAddress);
#endif
			if (dwFunctionAddress) {
				LoadFromBufferFn = (_LoadFromBuffer)dwFunctionAddress;
				SearchFunction = true;
			}
		}

		if (LoadFromBufferFn && SearchFunction) {
			return LoadFromBufferFn(pThis, pszFirst, pszSecond, pSomething, pAnother, pLast, New1Feb);
		}
		return false;
	}
}
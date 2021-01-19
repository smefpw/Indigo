#include "CSX_Memory.h"
#include <vector>
#include <Psapi.h>
#pragma comment(lib,"psapi")

#define INRANGE(x,a,b) (x >= a && x <= b) //IsInRange
#define getBits(x) (INRANGE((x&(~0x20)),'A','F') ? ((x&(~0x20)) - 'A' + 0xa) : (INRANGE(x,'0','9') ? x - '0' : 0))
#define GetByte(x) (getBits(x[0]) << 4 | getBits(x[1]))
#define GetBits2(x) (IsInRange(x, '0', '9') ? (x - '0') : ((x&(~0x20)) - 'A' + 0xA))

MODULEINFO GetModuleInfo(PCHAR szModule) {
	MODULEINFO modinfo = { 0 };
	HMODULE hModule = GetModuleHandleA(szModule);

	if(hModule == 0)
		return modinfo;

	GetModuleInformation(GetCurrentProcess(), hModule, &modinfo, sizeof(MODULEINFO));
	return modinfo;
}

bool bCompare(const BYTE* Data, const BYTE* Mask, const char* szMask) {
	for (; *szMask; ++szMask, ++Mask, ++Data) {
		if (*szMask == 'x' && *Mask != *Data) {
			return false;
		}
	}
	return (*szMask) == 0;
}

DWORD WaitOnModuleHandle(std::string moduleName) {
	DWORD ModuleHandle = NULL;
	while (!ModuleHandle) {
		ModuleHandle = (DWORD)GetModuleHandle(moduleName.c_str());
		if (!ModuleHandle)
			Sleep(50);
	}
	return ModuleHandle;
}

namespace CSX {
	namespace Memory {
		//for SigMaker code patterns 
		DWORD FindCodePattern(std::string moduleName, BYTE* Mask, char* szMask) {
			DWORD Address = WaitOnModuleHandle(moduleName.c_str());
			MODULEINFO ModInfo; GetModuleInformation(GetCurrentProcess(), (HMODULE)Address, &ModInfo, sizeof(MODULEINFO));
			DWORD Length = ModInfo.SizeOfImage;
			for (DWORD c = 0; c < Length; c += 1) {
				if (bCompare((BYTE*)(Address + c), Mask, szMask)) {
					return (DWORD)(Address + c);
				}
			}
			return 0;
		}

		/* Find Push String ( 0x68, dword ptr [str] ) */
		DWORD FindPatternV2(std::string moduleName, std::string Mask) {
			const char* pat = Mask.c_str();
			DWORD firstMatch = 0;
			DWORD rangeStart = (DWORD)GetModuleHandleA(moduleName.c_str());
			MODULEINFO miModInfo; GetModuleInformation(GetCurrentProcess(), (HMODULE)rangeStart, &miModInfo, sizeof(MODULEINFO));
			DWORD rangeEnd = rangeStart + miModInfo.SizeOfImage;
			for (DWORD pCur = rangeStart; pCur < rangeEnd; pCur++) {
				if (!*pat) {
					return firstMatch;
				}
				if (*(PBYTE)pat == '\?' || *(BYTE*)pCur == GetByte(pat)) {
					if (!firstMatch) {
						firstMatch = pCur;
					}
					if (!pat[2]) {
						return firstMatch;
					}
					if (*(PWORD)pat == '\?\?' || *(PBYTE)pat != '\?') {
						pat += 3;
					}
					else {
						pat += 2; //one ?
					}
				}
				else {
					pat = Mask.c_str();
					firstMatch = 0;
				}
			}
			return NULL;
		}

		DWORD FindSig(DWORD dwAddress, DWORD dwLength, const char* szPattern) {
			if (!dwAddress || !dwLength || !szPattern)
				return 0;

			const char* pat = szPattern;
			DWORD firstMatch = NULL;

			for (DWORD pCur = dwAddress; pCur < dwLength; pCur++) {
				if (!*pat)
					return firstMatch;

				if (*(PBYTE)pat == '\?' || *(BYTE*)pCur == GetByte(pat)) {
					if (!firstMatch)
						firstMatch = pCur;

					if (!pat[2])
						return firstMatch;

					if(*(PWORD)pat == '\?\?' || *(PBYTE)pat != '\?') {
						pat += 3;
					}
					else pat += 2;
				}
				else {
					pat = szPattern;
					firstMatch = 0;
				}
			}
			return 0;
		}
		DWORD FindSignature(const char* szModuleName, const char* PatternName, char* szPattern) {
			HMODULE hModule = GetModuleHandleA(szModuleName);
			PIMAGE_DOS_HEADER pDOSHeader = (PIMAGE_DOS_HEADER)hModule;
			PIMAGE_NT_HEADERS pNTHeaders = (PIMAGE_NT_HEADERS)(((DWORD)hModule) + pDOSHeader->e_lfanew);

			DWORD ret = FindSig(((DWORD)hModule) + pNTHeaders->OptionalHeader.BaseOfCode, ((DWORD)hModule) + pNTHeaders->OptionalHeader.SizeOfCode, szPattern);;
			return ret;
		}

		/* Find Push String ( 0x68, dword ptr [str] ) */
		DWORD FindPushString(DWORD dwStart, DWORD dwEnd, DWORD dwAddress) {
			char szPattern[5] = { 0x68 , 0x00 , 0x00 , 0x00 , 0x00 };
			*(PDWORD)&szPattern[1] = dwAddress;
			return FindPattern(szPattern , sizeof(szPattern), dwStart, dwEnd, 0);
		}

		DWORD FindPushString(PCHAR szModule, DWORD dwAddress) {
			MODULEINFO mInfo = GetModuleInfo(szModule);

			DWORD dwStart = (DWORD)mInfo.lpBaseOfDll;
			DWORD dwSize = (DWORD)mInfo.SizeOfImage;

			return FindPushString(dwStart, dwStart + dwSize, dwAddress);
		}

		/* Code Style Use Mask \x8B\xFF\xFF\xFF\xFF x???? */
		DWORD FindPattern(PCHAR pPattern, PCHAR pszMask, DWORD dwStart, DWORD dwEnd, DWORD dwOffset) {
			bool bFound = false;
			DWORD dwPtLen = lstrlenA(pszMask);

			for (DWORD dwPtr = dwStart; dwPtr < dwEnd - dwPtLen; dwPtr++) {
				bFound = true;

				for (DWORD idx = 0; idx < dwPtLen; idx++) {
					if (pszMask[idx] == 'x' && pPattern[idx] != *(PCHAR)(dwPtr + idx)) {
						bFound = false;
						break;
					}
				}

				if (bFound) {
					return dwPtr + dwOffset;
				}
			}

			return 0;
		}

		ULONG FindPBYTEPattern(std::string sModuleName, PBYTE pbPattern, std::string sMask, ULONG uCodeBase, ULONG uSizeOfCode) {
			BOOL bPatternDidMatch = FALSE;
			HMODULE hModule = GetModuleHandle(sModuleName.c_str());

			if (!hModule)
				return 0x0;

			PIMAGE_DOS_HEADER pDsHeader = PIMAGE_DOS_HEADER(hModule);
			PIMAGE_NT_HEADERS pPeHeader = PIMAGE_NT_HEADERS(LONG(hModule) + pDsHeader->e_lfanew);
			PIMAGE_OPTIONAL_HEADER pOptionalHeader = &pPeHeader->OptionalHeader;

			if (uCodeBase == 0x0)
				uCodeBase = (ULONG)hModule + pOptionalHeader->BaseOfCode;

			if (uSizeOfCode == 0x0)
				uSizeOfCode = pOptionalHeader->SizeOfCode;

			ULONG uArraySize = sMask.length();

			if (!uCodeBase || !uSizeOfCode || !uArraySize)
				return 0x0;

			for (size_t i = uCodeBase; i <= uCodeBase + uSizeOfCode; i++) {
				for (size_t t = 0; t < uArraySize; t++) {
					if(*((PBYTE)i + t) == pbPattern[t] || sMask.c_str()[t] == '?') {
						bPatternDidMatch = TRUE;
					}
					else {
						bPatternDidMatch = FALSE;
						break;
					}
				}

				if (bPatternDidMatch)
					return i;
			}
			return 0x0;
		}

		DWORD FindPattern(PCHAR szModule, PCHAR pPattern, PCHAR pszMask, DWORD dwOffset) {
			MODULEINFO mInfo = GetModuleInfo(szModule);

			DWORD dwStart = (DWORD)mInfo.lpBaseOfDll;
			DWORD dwSize = (DWORD)mInfo.SizeOfImage;

			return FindPattern(pPattern, pszMask, dwStart, dwStart + dwSize, dwOffset);
		}

		/* Code Style No Use Mask \x55\x56\xFF\x00 */
		DWORD FindPattern(PCHAR pPattern, DWORD dwPtLen, DWORD dwStart, DWORD dwEnd, DWORD dwOffset) {
			bool bFound = false;

			for (DWORD dwPtr = dwStart; dwPtr < dwEnd - dwPtLen; dwPtr++) {
				bFound = true;

				for (DWORD idx = 0; idx < dwPtLen; idx++) {
					if (pPattern[idx] != *(PCHAR)(dwPtr + idx)) {
						bFound = false;
						break;
					}
				}

				if (bFound)
					return dwPtr + dwOffset;
			}
			return 0;
		}

		DWORD FindPattern(PCHAR szModule, PCHAR pPattern, DWORD dwPtLen, DWORD dwOffset) {
			MODULEINFO mInfo = GetModuleInfo(szModule);

			DWORD dwStart = (DWORD)mInfo.lpBaseOfDll;
			DWORD dwSize = (DWORD)mInfo.SizeOfImage;

			return FindPattern(pPattern, dwPtLen, dwStart, dwStart + dwSize, dwOffset);
		}

		/* Find String */
		DWORD FindString(PCHAR szModule, PCHAR pszStr) {
			return FindPattern(szModule, pszStr, lstrlenA(pszStr), 0);
		}

		/* IDA Style 00 FF ?? */
		DWORD FindPattern(PCHAR pPattern, DWORD dwStart, DWORD dwEnd, DWORD dwOffset) {
			const char* pPat = pPattern;
			DWORD dwFind = 0;

			for (DWORD dwPtr = dwStart; dwPtr < dwEnd; dwPtr++) {
				if (!*pPat)
					return dwFind;

				if (*(PBYTE)pPat == '\?' || *(BYTE*)dwPtr == GetByte(pPat)) {
					if (!dwFind)
						dwFind = dwPtr;

					if (!pPat[2])
						return dwFind + dwOffset;

					if (*(PWORD)pPat == '\?\?' || *(PBYTE)pPat != '\?') {
						pPat += 3;
					}
					else
						pPat += 2;
				}
				else {
					pPat = pPattern;
					dwFind = 0;
				}
			}
			return 0;
		}

		DWORD FindPattern(PCHAR szModule, PCHAR pPattern, DWORD dwOffset) {
			MODULEINFO mInfo = GetModuleInfo( szModule );

			DWORD dwStart = (DWORD)mInfo.lpBaseOfDll;
			DWORD dwSize = (DWORD)mInfo.SizeOfImage;

			return FindPattern(pPattern, dwStart, dwStart + dwSize, dwOffset);
		}

		/* Native memory Func */
		void nt_memset( PVOID pBuffer , DWORD dwLen , DWORD dwSym ) {
			_asm {
				pushad
				mov edi , [pBuffer]
				mov ecx , [dwLen]
				mov eax , [dwSym]
				rep stosb
				popad
			}
		}

		std::uint8_t* NewPatternScan(void* module, const char* signature) {
			static auto pattern_to_byte = [](const char* pattern) {
				auto bytes = std::vector<int>{};
				auto start = const_cast<char*>(pattern);
				auto end = const_cast<char*>(pattern) + strlen(pattern);

				for (auto current = start; current < end; ++current) {
					if (*current == '?') {
						++current;
						if (*current == '?')
							++current;
						bytes.push_back(-1);
					}
					else {
						bytes.push_back(strtoul(current, &current, 16));
					}
				}
				return bytes;
			};

			auto dosHeader = (PIMAGE_DOS_HEADER)module;

			//check for null dosHeader->e_lfanew - this indicates an out of date sig.
			if (!dosHeader->e_lfanew)
				return nullptr;

			auto ntHeaders = (PIMAGE_NT_HEADERS)((std::uint8_t*)module + dosHeader->e_lfanew);
			auto sizeOfImage = ntHeaders->OptionalHeader.SizeOfImage;
			auto patternBytes = pattern_to_byte(signature);
			auto scanBytes = reinterpret_cast<std::uint8_t*>(module);

			auto s = patternBytes.size();
			auto d = patternBytes.data();

			for (auto i = 0ul; i < sizeOfImage - s; ++i) {
				bool found = true;
				for (auto j = 0ul; j < s; ++j) {
					if (scanBytes[i + j] != d[j] && d[j] != -1) {
						found = false;
						break;
					}
				}
				if (found) {
					return &scanBytes[i];
				}
			}
			return nullptr;
		}

		void nt_memcpy(PVOID pDst, PVOID pSrc, size_t Count) {
			_asm {
				mov	edi , [pDst]
				mov	esi , [pSrc]
				mov	ecx , [Count]
				rep	movsb
			}
		}
	}
}
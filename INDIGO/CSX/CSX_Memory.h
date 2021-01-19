#pragma once

#include "CSX.h"

namespace CSX {
	namespace Memory {
		//Find Sigmaker code patterns :D
		DWORD FindCodePattern(std::string moduleName, BYTE* Mask, char* szMask);

		//standard
		DWORD FindSignature(const char* szModuleName, const char* PatternName, char* szPattern);

		/* Find Push String ( 0x68, dword ptr [str] ) */
		DWORD FindPatternV2(std::string moduleName, std::string Mask);
		DWORD FindPushString(DWORD dwStart, DWORD dwEnd, DWORD dwAddress);
		DWORD FindPushString(PCHAR szModule, DWORD dwAddress);

		/* Code Style Use Mask \x8B\xFF\xFF\xFF\xFF x???? */
		DWORD FindPattern(PCHAR pPattern, PCHAR pszMask, DWORD dwStart, DWORD dwEnd, DWORD dwOffset);
		DWORD FindPattern(PCHAR szModule, PCHAR pPattern, PCHAR pszMask, DWORD dwOffset);
		
		/* Code Style No Use Mask \x55\x56\xFF\x00 */
		DWORD FindPattern(PCHAR pPattern, DWORD dwPtLen, DWORD dwStart, DWORD dwEnd, DWORD dwOffset);
		DWORD FindPattern(PCHAR szModule, PCHAR pPattern, DWORD dwPtLen, DWORD dwOffset);
		ULONG FindPBYTEPattern(std::string sModuleName, PBYTE pbPattern, std::string sMask, ULONG uCodeBase, ULONG uSizeOfCode);

		/* Find String */
		DWORD FindString(PCHAR szModule, PCHAR pszStr);

		/* IDA Style 00 FF ?? */
		DWORD FindPattern(PCHAR pPattern, DWORD dwStart, DWORD dwEnd, DWORD dwOffset);
		DWORD FindPattern(PCHAR szModule, PCHAR pPattern, DWORD dwOffset);

		/* Native memory Func */
		void nt_memset(PVOID pBuffer, DWORD dwLen, DWORD dwSym);
		std::uint8_t* NewPatternScan(void* module, const char* signature);
		void nt_memcpy(PVOID pDst, PVOID pSrc, size_t Count);
	}
}
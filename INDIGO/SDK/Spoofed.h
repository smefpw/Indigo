#pragma once
#include "SDK.h"

namespace SDK
{
	class SpoofedConvar {
	public:
		SpoofedConvar();
		SpoofedConvar(const char* szCVar);
		SpoofedConvar(ConVar* pCVar);

		~SpoofedConvar();

		bool           IsSpoofed();
		void           Spoof();

		void           SetFlags(int flags);
		int            GetFlags();

		void           SetBool(bool bValue);
		void           SetInt(int iValue);
		void           SetFloat(float flValue);
		void           SetString(const char* szValue);

	private:
		ConVar* m_pOriginalCVar = NULL;
		ConVar* m_pDummyCVar = NULL;

		char m_szDummyName[128];
		char m_szDummyValue[128];
		char m_szOriginalName[128];
		char m_szOriginalValue[128];
		int m_iOriginalFlags;
	};
}
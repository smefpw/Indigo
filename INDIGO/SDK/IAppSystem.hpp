#pragma once

#include "Definitions.hpp"

namespace SDK
{
	struct AppSystemInfo_t
	{
		const char *m_pModuleName;
		const char *m_pInterfaceName;
	};

	enum InitReturnVal_t
	{
		INIT_FAILED = 0 ,
		INIT_OK ,

		INIT_LAST_VAL ,
	};

	enum AppSystemTier_t
	{
		APP_SYSTEM_TIER0 = 0 ,
		APP_SYSTEM_TIER1 ,
		APP_SYSTEM_TIER2 ,
		APP_SYSTEM_TIER3 ,

		APP_SYSTEM_TIER_OTHER ,
	};

	class IAppSystem {
	public:
		virtual bool                            Connect( CreateInterfaceFn factory ) = 0;
		virtual void                            Disconnect() = 0;
		virtual void*                           QueryInterface( const char *pInterfaceName ) = 0;
		virtual InitReturnVal_t					Init() = 0;
		virtual void                            Shutdown() = 0;
		virtual AppSystemInfo_t*				GetDependencies() = 0;
		virtual AppSystemTier_t					GetTier() = 0;
		virtual void                            Reconnect( CreateInterfaceFn factory , const char *pInterfaceName ) = 0;
		virtual void                            UnkFunc() = 0;
	};
	struct CVarDLLIdentifier_t;

	using LocalizeStringIndex_t = unsigned;

	class ILocalizeTextQuery
	{
	public:
		virtual int ComputeTextWidth(const wchar_t* pString) = 0;
	};

	class ILocalizationChangeCallback
	{
	public:
		virtual void OnLocalizationChanged() = 0;
	};

	class ILocalize : public IAppSystem
	{
	public:
		virtual bool					AddFile(const char* fileName, const char* pPathID = nullptr, bool bIncludeFallbackSearchPaths = false) = 0;
		virtual void					RemoveAll() = 0;
		virtual wchar_t*				Find(const char* tokenName) = 0;
		virtual const wchar_t*			FindSafe(const char* tokenName) = 0;
		virtual int						ConvertANSIToUnicode(const char* ansi, wchar_t* unicode, int unicodeBufferSizeInBytes) = 0;
		virtual int						ConvertUnicodeToANSI(const wchar_t* unicode, char* ansi, int ansiBufferSize) = 0;
		virtual LocalizeStringIndex_t	FindIndex(const char* tokenName) = 0;
		virtual void					ConstructString(wchar_t* unicodeOuput, int unicodeBufferSizeInBytes, const wchar_t* formatString, int numFormatParameters, ...) = 0;
		virtual const char*				GetNameByIndex(LocalizeStringIndex_t index) = 0;
		virtual wchar_t*				GetValueByIndex(LocalizeStringIndex_t index) = 0;
	};

}
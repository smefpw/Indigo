#pragma once

#include "SteamID.h"

#define INVALID_HTTP_REQUEST_HANDLE 0
#define INVALID_STEAM_API_CALL_HANDLE 0

#define CAST(cast, address, add) reinterpret_cast<cast>((uint32_t)address + (uint32_t)add)

class ISteamUser;
class ISteamGameServer;
class ISteamFriends;
class ISteamUtils;
class ISteamMatchmaking;
class ISteamContentServer;
class ISteamMatchmakingServers;
class ISteamUserStats;
class ISteamApps;
class ISteamNetworking;
class ISteamRemoteStorage;
class ISteamScreenshots;
class ISteamMusic;
class ISteamMusicRemote;
class ISteamGameServerStats;
class ISteamHTTP;
class ISteamUnifiedMessages;
class ISteamController;
class ISteamUGC;
class ISteamAppList;
class ISteamHTMLSurface;
class ISteamInventory;
class ISteamVideo;
class CGameID;

typedef uint32_t AppId_t;
typedef uint64_t SteamAPICall_t;
typedef int32_t HSteamPipe;
typedef int32_t HSteamUser;
typedef int32_t HSteamCall;
typedef uint32_t HTTPRequestHandle;
typedef uint64_t SteamAPICallHandle;
typedef uint32_t SteamPipeHandle;
typedef uint32_t SteamUserHandle;
typedef size_t(*MemAllocFailHandler_t)(size_t);
typedef uint32_t(*SteamAPI_CheckCallbackRegistered_t)(int iCallbackNum);
typedef void(__cdecl *SteamAPIWarningMessageHook_t)(int, const char *);
typedef void(*SteamAPI_PostAPIResultInProcess_t)(SteamAPICallHandle callHandle, void*, uint32_t unCallbackSize, int iCallbackNum);

class IMemAlloc
{
public:
	// Release versions
	virtual void *Alloc(size_t nSize) = 0;
	virtual void *Realloc(void *pMem, size_t nSize) = 0;
	virtual void  Free(void *pMem) = 0;
	virtual void *Expand_NoLongerSupported(void *pMem, size_t nSize) = 0;

	// Debug versions
	virtual void *Alloc(size_t nSize, const char *pFileName, int nLine) = 0;
	virtual void *Realloc(void *pMem, size_t nSize, const char *pFileName, int nLine) = 0;
	virtual void  Free(void *pMem, const char *pFileName, int nLine) = 0;
	virtual void *Expand_NoLongerSupported(void *pMem, size_t nSize, const char *pFileName, int nLine) = 0;

	// Returns size of a particular allocation
	virtual size_t GetSize(void *pMem) = 0;

	// Force file + line information for an allocation
	virtual void PushAllocDbgInfo(const char *pFileName, int nLine) = 0;
	virtual void PopAllocDbgInfo() = 0;

	virtual long CrtSetBreakAlloc(long lNewBreakAlloc) = 0;
	virtual	int CrtSetReportMode(int nReportType, int nReportMode) = 0;
	virtual int CrtIsValidHeapPointer(const void *pMem) = 0;
	virtual int CrtIsValidPointer(const void *pMem, unsigned int size, int access) = 0;
	virtual int CrtCheckMemory(void) = 0;
	virtual int CrtSetDbgFlag(int nNewFlag) = 0;
	virtual void CrtMemCheckpoint(_CrtMemState *pState) = 0;
	virtual int heapchk() = 0;

	virtual void DumpStats() = 0;
	virtual void DumpStatsFileBase(char const *pchFileBase) = 0;
};

class ISteamUser
{
public:
	virtual HSteamUser GetHSteamUser() = 0;
	virtual bool BLoggedOn() = 0;
	virtual CSteamID GetSteamID() = 0;
	virtual int InitiateGameConnection(void *pAuthBlob, int cbMaxAuthBlob, CSteamID steamIDGameServer, uint32_t unIPServer, uint16_t usPortServer, bool bSecure) = 0;
	virtual void TerminateGameConnection(uint32_t unIPServer, uint16_t usPortServer) = 0;
	virtual void TrackAppUsageEvent(CGameID gameID, int eAppUsageEvent, const char *pchExtraInfo = "") = 0;
	virtual bool GetUserDataFolder(char *pchBuffer, int cubBuffer) = 0;
	virtual void StartVoiceRecording() = 0;
	virtual void StopVoiceRecording() = 0;
	virtual EVoiceResult GetAvailableVoice(uint32_t *pcbCompressed, uint32_t *pcbUncompressed, uint32_t nUncompressedVoiceDesiredSampleRate) = 0;
	virtual EVoiceResult GetVoice(bool bWantCompressed, void *pDestBuffer, uint32_t cbDestBufferSize, uint32_t *nBytesWritten, bool bWantUncompressed, void *pUncompressedDestBuffer, uint32_t cbUncompressedDestBufferSize, uint32_t *nUncompressBytesWritten, uint32_t nUncompressedVoiceDesiredSampleRate) = 0;
	virtual EVoiceResult DecompressVoice(const void *pCompressed, uint32_t cbCompressed, void *pDestBuffer, uint32_t cbDestBufferSize, uint32_t *nBytesWritten, uint32_t nDesiredSampleRate) = 0;
	virtual uint32_t GetVoiceOptimalSampleRate() = 0;
	virtual HAuthTicket GetAuthSessionTicket(void *pTicket, int cbMaxTicket, uint32_t *pcbTicket) = 0;
	virtual EBeginAuthSessionResult BeginAuthSession(const void *pAuthTicket, int cbAuthTicket, CSteamID steamID) = 0;
	virtual void EndAuthSession(CSteamID steamID) = 0;
	virtual void CancelAuthTicket(HAuthTicket hAuthTicket) = 0;
	virtual EUserHasLicenseForAppResult UserHasLicenseForApp(CSteamID steamID, AppId_t appID) = 0;
	virtual bool BIsBehindNAT() = 0;
	virtual void AdvertiseGame(CSteamID steamIDGameServer, uint32_t unIPServer, uint16_t usPortServer) = 0;
	virtual SteamAPICall_t RequestEncryptedAppTicket(void *pDataToInclude, int cbDataToInclude) = 0;
	virtual bool GetEncryptedAppTicket(void *pTicket, int cbMaxTicket, uint32_t *pcbTicket) = 0;
	virtual int GetGameBadgeLevel(int nSeries, bool bFoil) = 0;
	virtual int GetPlayerSteamLevel() = 0;
	virtual SteamAPICall_t RequestStoreAuthURL(const char *pchRedirectURL) = 0;

};

class ISteamClient
{
public:
	virtual SteamPipeHandle CreateSteamPipe() = 0;
	virtual bool BReleaseSteamPipe(SteamPipeHandle hSteamPipe) = 0;
	virtual SteamUserHandle ConnectToGlobalUser(SteamPipeHandle hSteamPipe) = 0;
	virtual SteamUserHandle CreateLocalUser(SteamPipeHandle *phSteamPipe, EAccountType eAccountType) = 0;
	virtual void ReleaseUser(SteamPipeHandle hSteamPipe, SteamUserHandle hUser) = 0;
	virtual ISteamUser* GetISteamUser(SteamUserHandle hSteamUser, SteamPipeHandle hSteamPipe, const char *pchVersion) = 0;
	virtual ISteamGameServer* GetISteamGameServer(SteamUserHandle hSteamUser, SteamPipeHandle hSteamPipe, const char *pchVersion) = 0;
	virtual void SetLocalIPBinding(uint32_t unIP, uint16_t usPort) = 0;
	virtual ISteamFriends* GetISteamFriends(SteamUserHandle hSteamUser, SteamPipeHandle hSteamPipe, const char *pchVersion) = 0;
	virtual ISteamUtils* GetISteamUtils(SteamPipeHandle hSteamPipe, const char *pchVersion) = 0;
	virtual ISteamMatchmaking* GetISteamMatchmaking(SteamUserHandle hSteamUser, SteamPipeHandle hSteamPipe, const char *pchVersion) = 0;
	virtual ISteamMatchmakingServers* GetISteamMatchmakingServers(SteamUserHandle hSteamUser, SteamPipeHandle hSteamPipe, const char *pchVersion) = 0;
	virtual void* GetISteamGenericInterface(SteamUserHandle hSteamUser, SteamPipeHandle hSteamPipe, const char *pchVersion) = 0;
	virtual ISteamUserStats* GetISteamUserStats(SteamUserHandle hSteamUser, SteamPipeHandle hSteamPipe, const char *pchVersion) = 0;
	virtual ISteamGameServerStats* GetISteamGameServerStats(SteamUserHandle hSteamuser, SteamPipeHandle hSteamPipe, const char *pchVersion) = 0;
	virtual ISteamApps* GetISteamApps(SteamUserHandle hSteamUser, SteamPipeHandle hSteamPipe, const char *pchVersion) = 0;
	virtual ISteamNetworking* GetISteamNetworking(SteamUserHandle hSteamUser, SteamPipeHandle hSteamPipe, const char *pchVersion) = 0;
	virtual ISteamRemoteStorage* GetISteamRemoteStorage(SteamUserHandle hSteamuser, SteamPipeHandle hSteamPipe, const char *pchVersion) = 0;
	virtual ISteamScreenshots* GetISteamScreenshots(SteamUserHandle hSteamuser, SteamPipeHandle hSteamPipe, const char *pchVersion) = 0;
	virtual void RunFrame() = 0;
	virtual uint32_t GetIPCCallCount() = 0;
	virtual void SetWarningMessageHook(SteamAPIWarningMessageHook_t pFunction) = 0;
	virtual bool ShutdownIfAllPipesClosed() = 0;
	virtual ISteamHTTP* GetISteamHTTP(SteamUserHandle hSteamuser, SteamPipeHandle hSteamPipe, const char *pchVersion) = 0;
	virtual ISteamUnifiedMessages* GetISteamUnifiedMessages(SteamUserHandle hSteamuser, SteamPipeHandle hSteamPipe, const char *pchVersion) = 0;
	virtual ISteamController* GetISteamController(SteamUserHandle hSteamUser, SteamPipeHandle hSteamPipe, const char *pchVersion) = 0;
	virtual ISteamUGC* GetISteamUGC(SteamUserHandle hSteamUser, SteamPipeHandle hSteamPipe, const char *pchVersion) = 0;
	virtual ISteamAppList* GetISteamAppList(SteamUserHandle hSteamUser, SteamPipeHandle hSteamPipe, const char *pchVersion) = 0;
	virtual ISteamMusic* GetISteamMusic(SteamUserHandle hSteamuser, SteamPipeHandle hSteamPipe, const char *pchVersion) = 0;
	virtual ISteamMusicRemote* GetISteamMusicRemote(SteamUserHandle hSteamuser, SteamPipeHandle hSteamPipe, const char *pchVersion) = 0;
	virtual ISteamHTMLSurface* GetISteamHTMLSurface(SteamUserHandle hSteamuser, SteamPipeHandle hSteamPipe, const char *pchVersion) = 0;
	virtual void Set_SteamAPI_CPostAPIResultInProcess(SteamAPI_PostAPIResultInProcess_t func) = 0;
	virtual void Remove_SteamAPI_CPostAPIResultInProcess(SteamAPI_PostAPIResultInProcess_t func) = 0;
	virtual void Set_SteamAPI_CCheckCallbackRegisteredInProcess(SteamAPI_CheckCallbackRegistered_t func) = 0;
	virtual ISteamInventory* GetISteamInventory(SteamUserHandle hSteamuser, SteamPipeHandle hSteamPipe, const char *pchVersion) = 0;
	virtual ISteamVideo* GetISteamVideo(SteamUserHandle hSteamuser, SteamPipeHandle hSteamPipe, const char *pchVersion) = 0;
};
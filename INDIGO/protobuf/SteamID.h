#pragma once

#include <Windows.h>

typedef uint32_t AccountID_t;

enum EGCResult
{
	EGCResultOK = 0,
	EGCResultNoMessage = 1,			// There is no message in the queue
	EGCResultBufferTooSmall = 2,		// The buffer is too small for the requested message
	EGCResultNotLoggedOn = 3,			// The client is not logged onto Steam
	EGCResultInvalidMessage = 4,		// Something was wrong with the message being sent with SendMessage
};

enum EUniverse
{
	k_EUniverseInvalid = 0,
	k_EUniversePublic = 1,
	k_EUniverseBeta = 2,
	k_EUniverseInternal = 3,
	k_EUniverseDev = 4,
	// k_EUniverseRC = 5,				// no such universe anymore
	k_EUniverseMax
};

enum EResult
{
	k_EResultOK = 1,							// success
	k_EResultFail = 2,							// generic failure 
	k_EResultNoConnection = 3,					// no/failed network connection
												//	k_EResultNoConnectionRetry = 4,				// OBSOLETE - removed
												k_EResultInvalidPassword = 5,				// password/ticket is invalid
												k_EResultLoggedInElsewhere = 6,				// same user logged in elsewhere
												k_EResultInvalidProtocolVer = 7,			// protocol version is incorrect
												k_EResultInvalidParam = 8,					// a parameter is incorrect
												k_EResultFileNotFound = 9,					// file was not found
												k_EResultBusy = 10,							// called method busy - action not taken
												k_EResultInvalidState = 11,					// called object was in an invalid state
												k_EResultInvalidName = 12,					// name is invalid
												k_EResultInvalidEmail = 13,					// email is invalid
												k_EResultDuplicateName = 14,				// name is not unique
												k_EResultAccessDenied = 15,					// access is denied
												k_EResultTimeout = 16,						// operation timed out
												k_EResultBanned = 17,						// VAC2 banned
												k_EResultAccountNotFound = 18,				// account not found
												k_EResultInvalidSteamID = 19,				// steamID is invalid
												k_EResultServiceUnavailable = 20,			// The requested service is currently unavailable
												k_EResultNotLoggedOn = 21,					// The user is not logged on
												k_EResultPending = 22,						// Request is pending (may be in process, or waiting on third party)
												k_EResultEncryptionFailure = 23,			// Encryption or Decryption failed
												k_EResultInsufficientPrivilege = 24,		// Insufficient privilege
												k_EResultLimitExceeded = 25,				// Too much of a good thing
												k_EResultRevoked = 26,						// Access has been revoked (used for revoked guest passes)
												k_EResultExpired = 27,						// License/Guest pass the user is trying to access is expired
												k_EResultAlreadyRedeemed = 28,				// Guest pass has already been redeemed by account, cannot be acked again
												k_EResultDuplicateRequest = 29,				// The request is a duplicate and the action has already occurred in the past, ignored this time
												k_EResultAlreadyOwned = 30,					// All the games in this guest pass redemption request are already owned by the user
												k_EResultIPNotFound = 31,					// IP address not found
												k_EResultPersistFailed = 32,				// failed to write change to the data store
												k_EResultLockingFailed = 33,				// failed to acquire access lock for this operation
												k_EResultLogonSessionReplaced = 34,
												k_EResultConnectFailed = 35,
												k_EResultHandshakeFailed = 36,
												k_EResultIOFailure = 37,
												k_EResultRemoteDisconnect = 38,
												k_EResultShoppingCartNotFound = 39,			// failed to find the shopping cart requested
												k_EResultBlocked = 40,						// a user didn't allow it
												k_EResultIgnored = 41,						// target is ignoring sender
												k_EResultNoMatch = 42,						// nothing matching the request found
												k_EResultAccountDisabled = 43,
												k_EResultServiceReadOnly = 44,				// this service is not accepting content changes right now
												k_EResultAccountNotFeatured = 45,			// account doesn't have value, so this feature isn't available
												k_EResultAdministratorOK = 46,				// allowed to take this action, but only because requester is admin
												k_EResultContentVersion = 47,				// A Version mismatch in content transmitted within the Steam protocol.
												k_EResultTryAnotherCM = 48,					// The current CM can't service the user making a request, user should try another.
												k_EResultPasswordRequiredToKickSession = 49,// You are already logged in elsewhere, this cached credential login has failed.
												k_EResultAlreadyLoggedInElsewhere = 50,		// You are already logged in elsewhere, you must wait
												k_EResultSuspended = 51,					// Long running operation (content download) suspended/paused
												k_EResultCancelled = 52,					// Operation canceled (typically by user: content download)
												k_EResultDataCorruption = 53,				// Operation canceled because data is ill formed or unrecoverable
												k_EResultDiskFull = 54,						// Operation canceled - not enough disk space.
												k_EResultRemoteCallFailed = 55,				// an remote call or IPC call failed
												k_EResultPasswordUnset = 56,				// Password could not be verified as it's unset server side
												k_EResultExternalAccountUnlinked = 57,		// External account (PSN, Facebook...) is not linked to a Steam account
												k_EResultPSNTicketInvalid = 58,				// PSN ticket was invalid
												k_EResultExternalAccountAlreadyLinked = 59,	// External account (PSN, Facebook...) is already linked to some other account, must explicitly request to replace/delete the link first
												k_EResultRemoteFileConflict = 60,			// The sync cannot resume due to a conflict between the local and remote files
												k_EResultIllegalPassword = 61,				// The requested new password is not legal
												k_EResultSameAsPreviousValue = 62,			// new value is the same as the old one ( secret question and answer )
												k_EResultAccountLogonDenied = 63,			// account login denied due to 2nd factor authentication failure
												k_EResultCannotUseOldPassword = 64,			// The requested new password is not legal
												k_EResultInvalidLoginAuthCode = 65,			// account login denied due to auth code invalid
												k_EResultAccountLogonDeniedNoMail = 66,		// account login denied due to 2nd factor auth failure - and no mail has been sent
												k_EResultHardwareNotCapableOfIPT = 67,		// 
												k_EResultIPTInitError = 68,					// 
												k_EResultParentalControlRestricted = 69,	// operation failed due to parental control restrictions for current user
												k_EResultFacebookQueryError = 70,			// Facebook query returned an error
												k_EResultExpiredLoginAuthCode = 71,			// account login denied due to auth code expired
												k_EResultIPLoginRestrictionFailed = 72,
												k_EResultAccountLockedDown = 73,
												k_EResultAccountLogonDeniedVerifiedEmailRequired = 74,
												k_EResultNoMatchingURL = 75,
												k_EResultBadResponse = 76,					// parse failure, missing field, etc.
												k_EResultRequirePasswordReEntry = 77,		// The user cannot complete the action until they re-enter their password
												k_EResultValueOutOfRange = 78,				// the value entered is outside the acceptable range
												k_EResultUnexpectedError = 79,				// something happened that we didn't expect to ever happen
												k_EResultDisabled = 80,						// The requested service has been configured to be unavailable
												k_EResultInvalidCEGSubmission = 81,			// The set of files submitted to the CEG server are not valid !
												k_EResultRestrictedDevice = 82,				// The device being used is not allowed to perform this action
												k_EResultRegionLocked = 83,					// The action could not be complete because it is region restricted
												k_EResultRateLimitExceeded = 84,			// Temporary rate limit exceeded, try again later, different from k_EResultLimitExceeded which may be permanent
												k_EResultAccountLoginDeniedNeedTwoFactor = 85,	// Need two-factor code to login
												k_EResultItemDeleted = 86,					// The thing we're trying to access has been deleted
												k_EResultAccountLoginDeniedThrottle = 87,	// login attempt failed, try to throttle response to possible attacker
												k_EResultTwoFactorCodeMismatch = 88,		// two factor code mismatch
												k_EResultTwoFactorActivationCodeMismatch = 89,	// activation code for two-factor didn't match
												k_EResultAccountAssociatedToMultiplePartners = 90,	// account has been associated with multiple partners
												k_EResultNotModified = 91,					// data not modified
												k_EResultNoMobileDevice = 92,				// the account does not have a mobile device associated with it
												k_EResultTimeNotSynced = 93,				// the time presented is out of range or tolerance
												k_EResultSmsCodeFailed = 94,				// SMS code failure (no match, none pending, etc.)
												k_EResultAccountLimitExceeded = 95,			// Too many accounts access this resource
												k_EResultAccountActivityLimitExceeded = 96,	// Too many changes to this account
												k_EResultPhoneActivityLimitExceeded = 97,	// Too many changes to this phone
												k_EResultRefundToWallet = 98,				// Cannot refund to payment method, must use wallet
												k_EResultEmailSendFailure = 99,				// Cannot send an email
												k_EResultNotSettled = 100,					// Can't perform operation till payment has settled
};

enum EVoiceResult
{
	k_EVoiceResultOK = 0,
	k_EVoiceResultNotInitialized = 1,
	k_EVoiceResultNotRecording = 2,
	k_EVoiceResultNoData = 3,
	k_EVoiceResultBufferTooSmall = 4,
	k_EVoiceResultDataCorrupted = 5,
	k_EVoiceResultRestricted = 6,
	k_EVoiceResultUnsupportedCodec = 7,
	k_EVoiceResultReceiverOutOfDate = 8,
	k_EVoiceResultReceiverDidNotAnswer = 9,

};

enum EDenyReason
{
	k_EDenyInvalid = 0,
	k_EDenyInvalidVersion = 1,
	k_EDenyGeneric = 2,
	k_EDenyNotLoggedOn = 3,
	k_EDenyNoLicense = 4,
	k_EDenyCheater = 5,
	k_EDenyLoggedInElseWhere = 6,
	k_EDenyUnknownText = 7,
	k_EDenyIncompatibleAnticheat = 8,
	k_EDenyMemoryCorruption = 9,
	k_EDenyIncompatibleSoftware = 10,
	k_EDenySteamConnectionLost = 11,
	k_EDenySteamConnectionError = 12,
	k_EDenySteamResponseTimedOut = 13,
	k_EDenySteamValidationStalled = 14,
	k_EDenySteamOwnerLeftGuestUser = 15,
};

typedef uint32_t HAuthTicket;
const HAuthTicket k_HAuthTicketInvalid = 0;

enum EBeginAuthSessionResult
{
	k_EBeginAuthSessionResultOK = 0,						// Ticket is valid for this game and this steamID.
	k_EBeginAuthSessionResultInvalidTicket = 1,				// Ticket is not valid.
	k_EBeginAuthSessionResultDuplicateRequest = 2,			// A ticket has already been submitted for this steamID
	k_EBeginAuthSessionResultInvalidVersion = 3,			// Ticket is from an incompatible interface version
	k_EBeginAuthSessionResultGameMismatch = 4,				// Ticket is not for this game
	k_EBeginAuthSessionResultExpiredTicket = 5,				// Ticket has expired
};

enum EAuthSessionResponse
{
	k_EAuthSessionResponseOK = 0,							// Steam has verified the user is online, the ticket is valid and ticket has not been reused.
	k_EAuthSessionResponseUserNotConnectedToSteam = 1,		// The user in question is not connected to steam
	k_EAuthSessionResponseNoLicenseOrExpired = 2,			// The license has expired.
	k_EAuthSessionResponseVACBanned = 3,					// The user is VAC banned for this game.
	k_EAuthSessionResponseLoggedInElseWhere = 4,			// The user account has logged in elsewhere and the session containing the game instance has been disconnected.
	k_EAuthSessionResponseVACCheckTimedOut = 5,				// VAC has been unable to perform anti-cheat checks on this user
	k_EAuthSessionResponseAuthTicketCanceled = 6,			// The ticket has been canceled by the issuer
	k_EAuthSessionResponseAuthTicketInvalidAlreadyUsed = 7,	// This ticket has already been used, it is not valid.
	k_EAuthSessionResponseAuthTicketInvalid = 8,			// This ticket is not from a user instance currently connected to steam.
	k_EAuthSessionResponsePublisherIssuedBan = 9,			// The user is banned for this game. The ban came via the web api and not VAC
};

enum EUserHasLicenseForAppResult
{
	k_EUserHasLicenseResultHasLicense = 0,					// User has a license for specified app
	k_EUserHasLicenseResultDoesNotHaveLicense = 1,			// User does not have a license for the specified app
	k_EUserHasLicenseResultNoAuth = 2,						// User has not been authenticated
};

enum EAccountType
{
	k_EAccountTypeInvalid = 0,
	k_EAccountTypeIndividual = 1,		// single user account
	k_EAccountTypeMultiseat = 2,		// multiseat (e.g. cybercafe) account
	k_EAccountTypeGameServer = 3,		// game server account
	k_EAccountTypeAnonGameServer = 4,	// anonymous game server account
	k_EAccountTypePending = 5,			// pending
	k_EAccountTypeContentServer = 6,	// content server
	k_EAccountTypeClan = 7,
	k_EAccountTypeChat = 8,
	k_EAccountTypeConsoleUser = 9,		// Fake SteamID for local PSN account on PS3 or Live account on 360, etc.
	k_EAccountTypeAnonUser = 10,

	// Max of 16 items in this field
	k_EAccountTypeMax
};

enum EAppReleaseState
{
	k_EAppReleaseState_Unknown = 0,	// unknown, required appinfo or license info is missing
	k_EAppReleaseState_Unavailable = 1,	// even if user 'just' owns it, can see game at all
	k_EAppReleaseState_Prerelease = 2,	// can be purchased and is visible in games list, nothing else. Common appInfo section released
	k_EAppReleaseState_PreloadOnly = 3,	// owners can preload app, not play it. AppInfo fully released.
	k_EAppReleaseState_Released = 4,	// owners can download and play app.
};

enum EAppOwnershipFlags
{
	k_EAppOwnershipFlags_None = 0x0000,	// unknown
	k_EAppOwnershipFlags_OwnsLicense = 0x0001,	// owns license for this game
	k_EAppOwnershipFlags_FreeLicense = 0x0002,	// not paid for game
	k_EAppOwnershipFlags_RegionRestricted = 0x0004,	// owns app, but not allowed to play in current region
	k_EAppOwnershipFlags_LowViolence = 0x0008,	// only low violence version
	k_EAppOwnershipFlags_InvalidPlatform = 0x0010,	// app not supported on current platform
	k_EAppOwnershipFlags_SharedLicense = 0x0020,	// license was granted by authorized local device
	k_EAppOwnershipFlags_FreeWeekend = 0x0040,	// owned by a free weekend licenses
	k_EAppOwnershipFlags_RetailLicense = 0x0080,	// has a retail license for game, (CD-Key etc)
	k_EAppOwnershipFlags_LicenseLocked = 0x0100,	// shared license is locked (in use) by other user
	k_EAppOwnershipFlags_LicensePending = 0x0200,	// owns app, but transaction is still pending. Can't install or play
	k_EAppOwnershipFlags_LicenseExpired = 0x0400,	// doesn't own app anymore since license expired
	k_EAppOwnershipFlags_LicensePermanent = 0x0800,	// permanent license, not borrowed, or guest or freeweekend etc
	k_EAppOwnershipFlags_LicenseRecurring = 0x1000,	// Recurring license, user is charged periodically
	k_EAppOwnershipFlags_LicenseCanceled = 0x2000,	// Mark as canceled, but might be still active if recurring
	k_EAppOwnershipFlags_AutoGrant = 0x4000,	// Ownership is based on any kind of autogrant license
};

enum EAppType
{
	k_EAppType_Invalid = 0x000,	// unknown / invalid
	k_EAppType_Game = 0x001,	// playable game, default type
	k_EAppType_Application = 0x002,	// software application
	k_EAppType_Tool = 0x004,	// SDKs, editors & dedicated servers
	k_EAppType_Demo = 0x008,	// game demo
	k_EAppType_Media_DEPRECATED = 0x010,	// legacy - was used for game trailers, which are now just videos on the web
	k_EAppType_DLC = 0x020,	// down loadable content
	k_EAppType_Guide = 0x040,	// game guide, PDF etc
	k_EAppType_Driver = 0x080,	// hardware driver updater (ATI, Razor etc)
	k_EAppType_Config = 0x100,	// hidden app used to config Steam features (backpack, sales, etc)
	k_EAppType_Hardware = 0x200,	// a hardware device (Steam Machine, Steam Controller, Steam Link, etc.)
									// 0x400 is up for grabs here
									k_EAppType_Video = 0x800,	// A video component of either a Film or TVSeries (may be the feature, an episode, preview, making-of, etc)
									k_EAppType_Plugin = 0x1000,	// Plug-in types for other Apps
									k_EAppType_Music = 0x2000,	// Music files

									k_EAppType_Shortcut = 0x40000000,	// just a shortcut, client side only
									k_EAppType_DepotOnly = 0x80000000,	// placeholder since depots and apps share the same namespace
};

enum ESteamUserStatType
{
	k_ESteamUserStatTypeINVALID = 0,
	k_ESteamUserStatTypeINT = 1,
	k_ESteamUserStatTypeFLOAT = 2,
	// Read as FLOAT, set with count / session length
	k_ESteamUserStatTypeAVGRATE = 3,
	k_ESteamUserStatTypeACHIEVEMENTS = 4,
	k_ESteamUserStatTypeGROUPACHIEVEMENTS = 5,

	// max, for sanity checks
	k_ESteamUserStatTypeMAX
};

enum EChatEntryType
{
	k_EChatEntryTypeInvalid = 0,
	k_EChatEntryTypeChatMsg = 1,		// Normal text message from another user
	k_EChatEntryTypeTyping = 2,			// Another user is typing (not used in multi-user chat)
	k_EChatEntryTypeInviteGame = 3,		// Invite from other user into that users current game
	k_EChatEntryTypeEmote = 4,			// text emote message (deprecated, should be treated as ChatMsg)
										//k_EChatEntryTypeLobbyGameStart = 5,	// lobby game is starting (dead - listen for LobbyGameCreated_t callback instead)
										k_EChatEntryTypeLeftConversation = 6, // user has left the conversation ( closed chat window )
																			  // Above are previous FriendMsgType entries, now merged into more generic chat entry types
																			  k_EChatEntryTypeEntered = 7,		// user has entered the conversation (used in multi-user chat and group chat)
																			  k_EChatEntryTypeWasKicked = 8,		// user was kicked (data: 64-bit steamid of actor performing the kick)
																			  k_EChatEntryTypeWasBanned = 9,		// user was banned (data: 64-bit steamid of actor performing the ban)
																			  k_EChatEntryTypeDisconnected = 10,	// user disconnected
																			  k_EChatEntryTypeHistoricalChat = 11,	// a chat message from user's chat history or offilne message
																			  k_EChatEntryTypeReserved1 = 12,
																			  k_EChatEntryTypeReserved2 = 13,
																			  k_EChatEntryTypeLinkBlocked = 14, // a link was removed by the chat filter.
};

enum EChatRoomEnterResponse
{
	k_EChatRoomEnterResponseSuccess = 1,		// Success
	k_EChatRoomEnterResponseDoesntExist = 2,	// Chat doesn't exist (probably closed)
	k_EChatRoomEnterResponseNotAllowed = 3,		// General Denied - You don't have the permissions needed to join the chat
	k_EChatRoomEnterResponseFull = 4,			// Chat room has reached its maximum size
	k_EChatRoomEnterResponseError = 5,			// Unexpected Error
	k_EChatRoomEnterResponseBanned = 6,			// You are banned from this chat room and may not join
	k_EChatRoomEnterResponseLimited = 7,		// Joining this chat is not allowed because you are a limited user (no value on account)
	k_EChatRoomEnterResponseClanDisabled = 8,	// Attempt to join a clan chat when the clan is locked or disabled
	k_EChatRoomEnterResponseCommunityBan = 9,	// Attempt to join a chat when the user has a community lock on their account
	k_EChatRoomEnterResponseMemberBlockedYou = 10, // Join failed - some member in the chat has blocked you from joining
	k_EChatRoomEnterResponseYouBlockedMember = 11, // Join failed - you have blocked some member already in the chat
												   // k_EChatRoomEnterResponseNoRankingDataLobby = 12,  // No longer used
												   // k_EChatRoomEnterResponseNoRankingDataUser = 13,  //  No longer used
												   // k_EChatRoomEnterResponseRankOutOfRange = 14, //  No longer used
};

typedef void(*PFNLegacyKeyRegistration)(const char *pchCDKey, const char *pchInstallPath);
typedef bool(*PFNLegacyKeyInstalled)();

const unsigned int k_unSteamAccountIDMask = 0xFFFFFFFF;
const unsigned int k_unSteamAccountInstanceMask = 0x000FFFFF;
const unsigned int k_unSteamUserDesktopInstance = 1;
const unsigned int k_unSteamUserConsoleInstance = 2;
const unsigned int k_unSteamUserWebInstance = 4;

enum EChatSteamIDInstanceFlags
{
	k_EChatAccountInstanceMask = 0x00000FFF, // top 8 bits are flags

	k_EChatInstanceFlagClan = (k_unSteamAccountInstanceMask + 1) >> 1,	// top bit
	k_EChatInstanceFlagLobby = (k_unSteamAccountInstanceMask + 1) >> 2,	// next one down, etc
	k_EChatInstanceFlagMMSLobby = (k_unSteamAccountInstanceMask + 1) >> 3,	// next one down, etc

																			// Max of 8 flags
};

enum EMarketingMessageFlags
{
	k_EMarketingMessageFlagsNone = 0,
	k_EMarketingMessageFlagsHighPriority = 1 << 0,
	k_EMarketingMessageFlagsPlatformWindows = 1 << 1,
	k_EMarketingMessageFlagsPlatformMac = 1 << 2,
	k_EMarketingMessageFlagsPlatformLinux = 1 << 3,

	//aggregate flags
	k_EMarketingMessageFlagsPlatformRestrictions =
	k_EMarketingMessageFlagsPlatformWindows |
	k_EMarketingMessageFlagsPlatformMac |
	k_EMarketingMessageFlagsPlatformLinux,
};

enum ENotificationPosition
{
	k_EPositionTopLeft = 0,
	k_EPositionTopRight = 1,
	k_EPositionBottomLeft = 2,
	k_EPositionBottomRight = 3,
};

enum EBroadcastUploadResult
{
	k_EBroadcastUploadResultNone = 0,	// broadcast state unknown
	k_EBroadcastUploadResultOK = 1,		// broadcast was good, no problems
	k_EBroadcastUploadResultInitFailed = 2,	// broadcast init failed
	k_EBroadcastUploadResultFrameFailed = 3,	// broadcast frame upload failed
	k_EBroadcastUploadResultTimeout = 4,	// broadcast upload timed out
	k_EBroadcastUploadResultBandwidthExceeded = 5,	// broadcast send too much data
	k_EBroadcastUploadResultLowFPS = 6,	// broadcast FPS too low
	k_EBroadcastUploadResultMissingKeyFrames = 7,	// broadcast sending not enough key frames
	k_EBroadcastUploadResultNoConnection = 8,	// broadcast client failed to connect to relay
	k_EBroadcastUploadResultRelayFailed = 9,	// relay dropped the upload
	k_EBroadcastUploadResultSettingsChanged = 10,	// the client changed broadcast settings 
	k_EBroadcastUploadResultMissingAudio = 11,	// client failed to send audio data
	k_EBroadcastUploadResultTooFarBehind = 12,	// clients was too slow uploading
};

class ISteamGameCoordinator
{
public:

	virtual EGCResult SendMessage(uint32_t unMsgType, const void *pubData, uint32_t cubData) = 0;
	virtual bool IsMessageAvailable(uint32_t *pcubMsgSize) = 0;
	virtual EGCResult RetrieveMessage(uint32_t *punMsgType, void *pubDest, uint32_t cubDest, uint32_t *pcubMsgSize) = 0;
};

class CSteamID
{
public:

	//-----------------------------------------------------------------------------
	// Purpose: Constructor
	//-----------------------------------------------------------------------------
	CSteamID()
	{
		m_steamid.m_comp.m_unAccountID = 0;
		m_steamid.m_comp.m_EAccountType = k_EAccountTypeInvalid;
		m_steamid.m_comp.m_EUniverse = k_EUniverseInvalid;
		m_steamid.m_comp.m_unAccountInstance = 0;
	}


	//-----------------------------------------------------------------------------
	// Purpose: Constructor
	// Input  : unAccountID -	32-bit account ID
	//			eUniverse -		Universe this account belongs to
	//			eAccountType -	Type of account
	//-----------------------------------------------------------------------------
	CSteamID(uint32_t unAccountID, EUniverse eUniverse, EAccountType eAccountType)
	{
		Set(unAccountID, eUniverse, eAccountType);
	}


	//-----------------------------------------------------------------------------
	// Purpose: Constructor
	// Input  : unAccountID -	32-bit account ID
	//			unAccountInstance - instance 
	//			eUniverse -		Universe this account belongs to
	//			eAccountType -	Type of account
	//-----------------------------------------------------------------------------
	CSteamID(uint32_t unAccountID, unsigned int unAccountInstance, EUniverse eUniverse, EAccountType eAccountType)
	{
#if defined(_SERVER) && defined(Assert)
		Assert(!((k_EAccountTypeIndividual == eAccountType) && (unAccountInstance > k_unSteamUserWebInstance)));	// enforce that for individual accounts, instance is always 1
#endif // _SERVER
		InstancedSet(unAccountID, unAccountInstance, eUniverse, eAccountType);
	}


	//-----------------------------------------------------------------------------
	// Purpose: Constructor
	// Input  : ulSteamID -		64-bit representation of a Steam ID
	// Note:	Will not accept a uint32 or int32 as input, as that is a probable mistake.
	//			See the stubbed out overloads in the private: section for more info.
	//-----------------------------------------------------------------------------
	CSteamID(uint64_t ulSteamID)
	{
		SetFromUint64(ulSteamID);
	}
#ifdef INT64_DIFFERENT_FROM_INT64_T
	CSteamID(uint64_t ulSteamID)
	{
		SetFromUint64((uint64)ulSteamID);
	}
#endif


	//-----------------------------------------------------------------------------
	// Purpose: Sets parameters for steam ID
	// Input  : unAccountID -	32-bit account ID
	//			eUniverse -		Universe this account belongs to
	//			eAccountType -	Type of account
	//-----------------------------------------------------------------------------
	void Set(uint32_t unAccountID, EUniverse eUniverse, EAccountType eAccountType)
	{
		m_steamid.m_comp.m_unAccountID = unAccountID;
		m_steamid.m_comp.m_EUniverse = eUniverse;
		m_steamid.m_comp.m_EAccountType = eAccountType;

		if (eAccountType == k_EAccountTypeClan || eAccountType == k_EAccountTypeGameServer)
		{
			m_steamid.m_comp.m_unAccountInstance = 0;
		}
		else
		{
			// by default we pick the desktop instance
			m_steamid.m_comp.m_unAccountInstance = k_unSteamUserDesktopInstance;
		}
	}


	//-----------------------------------------------------------------------------
	// Purpose: Sets parameters for steam ID
	// Input  : unAccountID -	32-bit account ID
	//			eUniverse -		Universe this account belongs to
	//			eAccountType -	Type of account
	//-----------------------------------------------------------------------------
	void InstancedSet(uint32_t unAccountID, uint32_t unInstance, EUniverse eUniverse, EAccountType eAccountType)
	{
		m_steamid.m_comp.m_unAccountID = unAccountID;
		m_steamid.m_comp.m_EUniverse = eUniverse;
		m_steamid.m_comp.m_EAccountType = eAccountType;
		m_steamid.m_comp.m_unAccountInstance = unInstance;
	}


	//-----------------------------------------------------------------------------
	// Purpose: Initializes a steam ID from its 52 bit parts and universe/type
	// Input  : ulIdentifier - 52 bits of goodness
	//-----------------------------------------------------------------------------
	void FullSet(uint64_t ulIdentifier, EUniverse eUniverse, EAccountType eAccountType)
	{
		m_steamid.m_comp.m_unAccountID = (ulIdentifier & k_unSteamAccountIDMask);						// account ID is low 32 bits
		m_steamid.m_comp.m_unAccountInstance = ((ulIdentifier >> 32) & k_unSteamAccountInstanceMask);			// account instance is next 20 bits
		m_steamid.m_comp.m_EUniverse = eUniverse;
		m_steamid.m_comp.m_EAccountType = eAccountType;
	}


	//-----------------------------------------------------------------------------
	// Purpose: Initializes a steam ID from its 64-bit representation
	// Input  : ulSteamID -		64-bit representation of a Steam ID
	//-----------------------------------------------------------------------------
	void SetFromUint64(uint64_t ulSteamID)
	{
		m_steamid.m_unAll64Bits = ulSteamID;
	}


	//-----------------------------------------------------------------------------
	// Purpose: Clear all fields, leaving an invalid ID.
	//-----------------------------------------------------------------------------
	void Clear()
	{
		m_steamid.m_comp.m_unAccountID = 0;
		m_steamid.m_comp.m_EAccountType = k_EAccountTypeInvalid;
		m_steamid.m_comp.m_EUniverse = k_EUniverseInvalid;
		m_steamid.m_comp.m_unAccountInstance = 0;
	}


#if defined( INCLUDED_STEAM2_USERID_STRUCTS ) 
	//-----------------------------------------------------------------------------
	// Purpose: Initializes a steam ID from a Steam2 ID structure
	// Input:	pTSteamGlobalUserID -	Steam2 ID to convert
	//			eUniverse -				universe this ID belongs to
	//-----------------------------------------------------------------------------
	void SetFromSteam2(TSteamGlobalUserID *pTSteamGlobalUserID, EUniverse eUniverse)
	{
		m_steamid.m_comp.m_unAccountID = pTSteamGlobalUserID->m_SteamLocalUserID.Split.Low32bits * 2 +
			pTSteamGlobalUserID->m_SteamLocalUserID.Split.High32bits;
		m_steamid.m_comp.m_EUniverse = eUniverse;		// set the universe
		m_steamid.m_comp.m_EAccountType = k_EAccountTypeIndividual; // Steam 2 accounts always map to account type of individual
		m_steamid.m_comp.m_unAccountInstance = k_unSteamUserDesktopInstance; // Steam2 only knew desktop instances
	}

	//-----------------------------------------------------------------------------
	// Purpose: Fills out a Steam2 ID structure
	// Input:	pTSteamGlobalUserID -	Steam2 ID to write to
	//-----------------------------------------------------------------------------
	void ConvertToSteam2(TSteamGlobalUserID *pTSteamGlobalUserID) const
	{
		// only individual accounts have any meaning in Steam 2, only they can be mapped
		// Assert( m_steamid.m_comp.m_EAccountType == k_EAccountTypeIndividual );

		pTSteamGlobalUserID->m_SteamInstanceID = 0;
		pTSteamGlobalUserID->m_SteamLocalUserID.Split.High32bits = m_steamid.m_comp.m_unAccountID % 2;
		pTSteamGlobalUserID->m_SteamLocalUserID.Split.Low32bits = m_steamid.m_comp.m_unAccountID / 2;
	}
#endif // defined( INCLUDED_STEAM_COMMON_STEAMCOMMON_H )

	//-----------------------------------------------------------------------------
	// Purpose: Converts steam ID to its 64-bit representation
	// Output : 64-bit representation of a Steam ID
	//-----------------------------------------------------------------------------
	uint64_t ConvertToUint64() const
	{
		return m_steamid.m_unAll64Bits;
	}


	//-----------------------------------------------------------------------------
	// Purpose: Converts the static parts of a steam ID to a 64-bit representation.
	//			For multiseat accounts, all instances of that account will have the
	//			same static account key, so they can be grouped together by the static
	//			account key.
	// Output : 64-bit static account key
	//-----------------------------------------------------------------------------
	uint64_t GetStaticAccountKey() const
	{
		// note we do NOT include the account instance (which is a dynamic property) in the static account key
		return (uint64_t)((((uint64_t)m_steamid.m_comp.m_EUniverse) << 56) + ((uint64_t)m_steamid.m_comp.m_EAccountType << 52) + m_steamid.m_comp.m_unAccountID);
	}


	//-----------------------------------------------------------------------------
	// Purpose: create an anonymous game server login to be filled in by the AM
	//-----------------------------------------------------------------------------
	void CreateBlankAnonLogon(EUniverse eUniverse)
	{
		m_steamid.m_comp.m_unAccountID = 0;
		m_steamid.m_comp.m_EAccountType = k_EAccountTypeAnonGameServer;
		m_steamid.m_comp.m_EUniverse = eUniverse;
		m_steamid.m_comp.m_unAccountInstance = 0;
	}


	//-----------------------------------------------------------------------------
	// Purpose: create an anonymous game server login to be filled in by the AM
	//-----------------------------------------------------------------------------
	void CreateBlankAnonUserLogon(EUniverse eUniverse)
	{
		m_steamid.m_comp.m_unAccountID = 0;
		m_steamid.m_comp.m_EAccountType = k_EAccountTypeAnonUser;
		m_steamid.m_comp.m_EUniverse = eUniverse;
		m_steamid.m_comp.m_unAccountInstance = 0;
	}

	//-----------------------------------------------------------------------------
	// Purpose: Is this an anonymous game server login that will be filled in?
	//-----------------------------------------------------------------------------
	bool BBlankAnonAccount() const
	{
		return m_steamid.m_comp.m_unAccountID == 0 && BAnonAccount() && m_steamid.m_comp.m_unAccountInstance == 0;
	}

	//-----------------------------------------------------------------------------
	// Purpose: Is this a game server account id?  (Either persistent or anonymous)
	//-----------------------------------------------------------------------------
	bool BGameServerAccount() const
	{
		return m_steamid.m_comp.m_EAccountType == k_EAccountTypeGameServer || m_steamid.m_comp.m_EAccountType == k_EAccountTypeAnonGameServer;
	}

	//-----------------------------------------------------------------------------
	// Purpose: Is this a persistent (not anonymous) game server account id?
	//-----------------------------------------------------------------------------
	bool BPersistentGameServerAccount() const
	{
		return m_steamid.m_comp.m_EAccountType == k_EAccountTypeGameServer;
	}

	//-----------------------------------------------------------------------------
	// Purpose: Is this an anonymous game server account id?
	//-----------------------------------------------------------------------------
	bool BAnonGameServerAccount() const
	{
		return m_steamid.m_comp.m_EAccountType == k_EAccountTypeAnonGameServer;
	}

	//-----------------------------------------------------------------------------
	// Purpose: Is this a content server account id?
	//-----------------------------------------------------------------------------
	bool BContentServerAccount() const
	{
		return m_steamid.m_comp.m_EAccountType == k_EAccountTypeContentServer;
	}


	//-----------------------------------------------------------------------------
	// Purpose: Is this a clan account id?
	//-----------------------------------------------------------------------------
	bool BClanAccount() const
	{
		return m_steamid.m_comp.m_EAccountType == k_EAccountTypeClan;
	}


	//-----------------------------------------------------------------------------
	// Purpose: Is this a chat account id?
	//-----------------------------------------------------------------------------
	bool BChatAccount() const
	{
		return m_steamid.m_comp.m_EAccountType == k_EAccountTypeChat;
	}

	//-----------------------------------------------------------------------------
	// Purpose: Is this a chat account id?
	//-----------------------------------------------------------------------------
	bool IsLobby() const
	{
		return (m_steamid.m_comp.m_EAccountType == k_EAccountTypeChat)
			&& (m_steamid.m_comp.m_unAccountInstance & k_EChatInstanceFlagLobby);
	}


	//-----------------------------------------------------------------------------
	// Purpose: Is this an individual user account id?
	//-----------------------------------------------------------------------------
	bool BIndividualAccount() const
	{
		return m_steamid.m_comp.m_EAccountType == k_EAccountTypeIndividual || m_steamid.m_comp.m_EAccountType == k_EAccountTypeConsoleUser;
	}


	//-----------------------------------------------------------------------------
	// Purpose: Is this an anonymous account?
	//-----------------------------------------------------------------------------
	bool BAnonAccount() const
	{
		return m_steamid.m_comp.m_EAccountType == k_EAccountTypeAnonUser || m_steamid.m_comp.m_EAccountType == k_EAccountTypeAnonGameServer;
	}

	//-----------------------------------------------------------------------------
	// Purpose: Is this an anonymous user account? ( used to create an account or reset a password )
	//-----------------------------------------------------------------------------
	bool BAnonUserAccount() const
	{
		return m_steamid.m_comp.m_EAccountType == k_EAccountTypeAnonUser;
	}

	//-----------------------------------------------------------------------------
	// Purpose: Is this a faked up Steam ID for a PSN friend account?
	//-----------------------------------------------------------------------------
	bool BConsoleUserAccount() const
	{
		return m_steamid.m_comp.m_EAccountType == k_EAccountTypeConsoleUser;
	}

	// simple accessors
	void SetAccountID(uint32_t unAccountID) { m_steamid.m_comp.m_unAccountID = unAccountID; }
	void SetAccountInstance(uint32_t unInstance) { m_steamid.m_comp.m_unAccountInstance = unInstance; }
	void ClearIndividualInstance() { if (BIndividualAccount()) m_steamid.m_comp.m_unAccountInstance = 0; }
	bool HasNoIndividualInstance() const { return BIndividualAccount() && (m_steamid.m_comp.m_unAccountInstance == 0); }
	AccountID_t GetAccountID() const { return m_steamid.m_comp.m_unAccountID; }
	uint32_t GetUnAccountInstance() const { return m_steamid.m_comp.m_unAccountInstance; }
	EAccountType GetEAccountType() const { return (EAccountType)m_steamid.m_comp.m_EAccountType; }
	EUniverse GetEUniverse() const { return m_steamid.m_comp.m_EUniverse; }
	void SetEUniverse(EUniverse eUniverse) { m_steamid.m_comp.m_EUniverse = eUniverse; }
	bool IsValid() const;

	// this set of functions is hidden, will be moved out of class
	explicit CSteamID(const char *pchSteamID, EUniverse eDefaultUniverse = k_EUniverseInvalid);
	const char * Render() const;				// renders this steam ID to string
	static const char * Render(uint64_t ulSteamID);	// static method to render a uint64 representation of a steam ID to a string

	void SetFromString(const char *pchSteamID, EUniverse eDefaultUniverse);
	// SetFromString allows many partially-correct strings, constraining how
	// we might be able to change things in the future.
	// SetFromStringStrict requires the exact string forms that we support
	// and is preferred when the caller knows it's safe to be strict.
	// Returns whether the string parsed correctly.
	bool SetFromStringStrict(const char *pchSteamID, EUniverse eDefaultUniverse);
	bool SetFromSteam2String(const char *pchSteam2ID, EUniverse eUniverse);

	inline bool operator==(const CSteamID &val) const { return m_steamid.m_unAll64Bits == val.m_steamid.m_unAll64Bits; }
	inline bool operator!=(const CSteamID &val) const { return !operator==(val); }
	inline bool operator<(const CSteamID &val) const { return m_steamid.m_unAll64Bits < val.m_steamid.m_unAll64Bits; }
	inline bool operator>(const CSteamID &val) const { return m_steamid.m_unAll64Bits > val.m_steamid.m_unAll64Bits; }

	// DEBUG function
	bool BValidExternalSteamID() const;

private:
	// These are defined here to prevent accidental implicit conversion of a u32AccountID to a CSteamID.
	// If you get a compiler error about an ambiguous constructor/function then it may be because you're
	// passing a 32-bit int to a function that takes a CSteamID. You should explicitly create the SteamID
	// using the correct Universe and account Type/Instance values.
	CSteamID(uint32_t);
	CSteamID(int32_t);

	// 64 bits total
	union SteamID_t
	{
		struct SteamIDComponent_t
		{
#ifdef VALVE_BIG_ENDIAN
			EUniverse			m_EUniverse : 8;	// universe this account belongs to
			unsigned int		m_EAccountType : 4;			// type of account - can't show as EAccountType, due to signed / unsigned difference
			unsigned int		m_unAccountInstance : 20;	// dynamic instance ID
			uint32				m_unAccountID : 32;			// unique account identifier
#else
			uint32_t				m_unAccountID : 32;			// unique account identifier
			unsigned int		m_unAccountInstance : 20;	// dynamic instance ID
			unsigned int		m_EAccountType : 4;			// type of account - can't show as EAccountType, due to signed / unsigned difference
			EUniverse			m_EUniverse : 8;	// universe this account belongs to
#endif
		} m_comp;

		uint64_t m_unAll64Bits;
	} m_steamid;
};

inline bool CSteamID::IsValid() const
{
	if (m_steamid.m_comp.m_EAccountType <= k_EAccountTypeInvalid || m_steamid.m_comp.m_EAccountType >= k_EAccountTypeMax)
		return false;

	if (m_steamid.m_comp.m_EUniverse <= k_EUniverseInvalid || m_steamid.m_comp.m_EUniverse >= k_EUniverseMax)
		return false;

	if (m_steamid.m_comp.m_EAccountType == k_EAccountTypeIndividual)
	{
		if (m_steamid.m_comp.m_unAccountID == 0 || m_steamid.m_comp.m_unAccountInstance > k_unSteamUserWebInstance)
			return false;
	}

	if (m_steamid.m_comp.m_EAccountType == k_EAccountTypeClan)
	{
		if (m_steamid.m_comp.m_unAccountID == 0 || m_steamid.m_comp.m_unAccountInstance != 0)
			return false;
	}

	if (m_steamid.m_comp.m_EAccountType == k_EAccountTypeGameServer)
	{
		if (m_steamid.m_comp.m_unAccountID == 0)
			return false;
		// Any limit on instances?  We use them for local users and bots
	}
	return true;
}
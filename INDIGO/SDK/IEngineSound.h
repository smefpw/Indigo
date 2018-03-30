#pragma once

#include "SDK.h"

namespace SDK
{
	class IRecipientFilter
	{
	public:
		virtual			~IRecipientFilter() { }

		virtual bool	IsReliable( void ) const = 0;
		virtual bool	IsInitMessage( void ) const = 0;

		virtual int		GetRecipientCount( void ) const = 0;
		virtual int		GetRecipientIndex( int slot ) const = 0;
	};

	//-----------------------------------------------------------------------------
	// channels
	//-----------------------------------------------------------------------------
	enum
	{
		CHAN_REPLACE = -1 ,
		CHAN_AUTO = 0 ,
		CHAN_WEAPON = 1 ,
		CHAN_VOICE = 2 ,
		CHAN_ITEM = 3 ,
		CHAN_BODY = 4 ,
		CHAN_STREAM = 5 ,		// allocate stream channel from the static or dynamic area
		CHAN_STATIC = 6 ,		// allocate channel from the static area 
		CHAN_VOICE2 = 7 ,
		CHAN_VOICE_BASE = 8 ,		// allocate channel for network voice data
		CHAN_USER_BASE = ( CHAN_VOICE_BASE + 128 )		// Anything >= this number is allocated to game code.
	};

	//-----------------------------------------------------------------------------
	// common volume values
	//-----------------------------------------------------------------------------
#define VOL_NORM		1.0f


//-----------------------------------------------------------------------------
// common attenuation values
//-----------------------------------------------------------------------------
#define ATTN_NONE		0.0f
#define ATTN_NORM		0.8f
#define ATTN_IDLE		2.0f
#define ATTN_STATIC		1.25f 
#define ATTN_RICOCHET	1.5f

// HL2 world is 8x bigger now! We want to hear gunfire from farther.
// Don't change this without consulting Kelly or Wedge (sjb).
#define ATTN_GUNFIRE	0.27f

	enum soundlevel_t
	{
		SNDLVL_NONE = 0 ,

		SNDLVL_20dB = 20 ,			// rustling leaves
		SNDLVL_25dB = 25 ,			// whispering
		SNDLVL_30dB = 30 ,			// library
		SNDLVL_35dB = 35 ,
		SNDLVL_40dB = 40 ,
		SNDLVL_45dB = 45 ,			// refrigerator

		SNDLVL_50dB = 50 ,	// 3.9	// average home
		SNDLVL_55dB = 55 ,	// 3.0

		SNDLVL_IDLE = 60 ,	// 2.0	
		SNDLVL_60dB = 60 ,	// 2.0	// normal conversation, clothes dryer

		SNDLVL_65dB = 65 ,	// 1.5	// washing machine, dishwasher
		SNDLVL_STATIC = 66 ,	// 1.25

		SNDLVL_70dB = 70 ,	// 1.0	// car, vacuum cleaner, mixer, electric sewing machine

		SNDLVL_NORM = 75 ,
		SNDLVL_75dB = 75 ,	// 0.8	// busy traffic

		SNDLVL_80dB = 80 ,	// 0.7	// mini-bike, alarm clock, noisy restaurant, office tabulator, outboard motor, passing snowmobile
		SNDLVL_TALKING = 80 ,	// 0.7
		SNDLVL_85dB = 85 ,	// 0.6	// average factory, electric shaver
		SNDLVL_90dB = 90 ,	// 0.5	// screaming child, passing motorcycle, convertible ride on frw
		SNDLVL_95dB = 95 ,
		SNDLVL_100dB = 100 ,	// 0.4	// subway train, diesel truck, woodworking shop, pneumatic drill, boiler shop, jackhammer
		SNDLVL_105dB = 105 ,			// helicopter, power mower
		SNDLVL_110dB = 110 ,			// snowmobile drvrs seat, inboard motorboat, sandblasting
		SNDLVL_120dB = 120 ,			// auto horn, propeller aircraft
		SNDLVL_130dB = 130 ,			// air raid siren

		SNDLVL_GUNFIRE = 140 ,	// 0.27	// THRESHOLD OF PAIN, gunshot, jet engine
		SNDLVL_140dB = 140 ,	// 0.2

		SNDLVL_150dB = 150 ,	// 0.2

		SNDLVL_180dB = 180 ,			// rocket launching

										// NOTE: Valid soundlevel_t values are 0-255.
										//       256-511 are reserved for sounds using goldsrc compatibility attenuation.
	};

#define MAX_SNDLVL_BITS		9	// Used to encode 0-255 for regular soundlevel_t's and 256-511 for goldsrc-compatible ones.
#define MIN_SNDLVL_VALUE	0
#define MAX_SNDLVL_VALUE	((1<<MAX_SNDLVL_BITS)-1)


#define ATTN_TO_SNDLVL( a ) (soundlevel_t)(int)((a) ? (50 + 20 / ((float)a)) : 0 )
#define SNDLVL_TO_ATTN( a ) ((a > 50) ? (20.0f / (float)(a - 50)) : 4.0 )

// This is a limit due to network encoding.
// It encodes attenuation * 64 in 8 bits, so the maximum is (255 / 64)
#define MAX_ATTENUATION		3.98f

//-----------------------------------------------------------------------------
// Flags to be or-ed together for the iFlags field
//-----------------------------------------------------------------------------
	enum SoundFlags_t
	{
		SND_NOFLAGS = 0 ,			// to keep the compiler happy
		SND_CHANGE_VOL = ( 1 << 0 ) ,		// change sound vol
		SND_CHANGE_PITCH = ( 1 << 1 ) ,		// change sound pitch
		SND_STOP = ( 1 << 2 ) ,		// stop the sound
		SND_SPAWNING = ( 1 << 3 ) ,		// we're spawning, used in some cases for ambients
										// not sent over net, only a param between dll and server.
										SND_DELAY = ( 1 << 4 ) ,		// sound has an initial delay
										SND_STOP_LOOPING = ( 1 << 5 ) ,		// stop all looping sounds on the entity.
										SND_SPEAKER = ( 1 << 6 ) ,		// being played again by a microphone through a speaker

										SND_SHOULDPAUSE = ( 1 << 7 ) ,		// this sound should be paused if the game is paused
										SND_IGNORE_PHONEMES = ( 1 << 8 ) ,
										SND_IGNORE_NAME = ( 1 << 9 ) ,		// used to change all sounds emitted by an entity, regardless of scriptname

										SND_DO_NOT_OVERWRITE_EXISTING_ON_CHANNEL = ( 1 << 10 ) ,
	};

#define SND_FLAG_BITS_ENCODE 11

#define MAX_SOUND_INDEX_BITS	14
#define	MAX_SOUNDS				(1<<MAX_SOUND_INDEX_BITS)

#define MAX_SOUND_DELAY_MSEC_ENCODE_BITS	(13)

// Subtract one to leave room for the sign bit
#define MAX_SOUND_DELAY_MSEC				(1<<(MAX_SOUND_DELAY_MSEC_ENCODE_BITS-1))    // 4096 msec or about 4 seconds

//-----------------------------------------------------------------------------
// common pitch values
//-----------------------------------------------------------------------------
#define	PITCH_NORM		100			// non-pitch shifted
#define PITCH_LOW		95			// other values are possible - 0-255, where 255 is very high
#define PITCH_HIGH		120

#define DEFAULT_SOUND_PACKET_VOLUME 1.0f
#define DEFAULT_SOUND_PACKET_PITCH	100
#define DEFAULT_SOUND_PACKET_DELAY	0.0f

// Handy defines for EmitSound
#define SOUND_FROM_UI_PANEL			-2		// Sound being played inside a UI panel on the client
#define SOUND_FROM_LOCAL_PLAYER		-1
#define SOUND_FROM_WORLD			0

// These are used to feed a soundlevel to the sound system and have it use
// goldsrc-type attenuation. We should use this as little as possible and 
// phase it out as soon as possible.

// Take a regular sndlevel and convert it to compatibility mode.
#define SNDLEVEL_TO_COMPATIBILITY_MODE( x )		((soundlevel_t)(int)( (x) + 256 ))

// Take a compatibility-mode sndlevel and get the REAL sndlevel out of it.
#define SNDLEVEL_FROM_COMPATIBILITY_MODE( x )	((soundlevel_t)(int)( (x) - 256 ))

// Tells if the given sndlevel is marked as compatibility mode.
#define SNDLEVEL_IS_COMPATIBILITY_MODE( x )		( (x) >= 256 )

//-----------------------------------------------------------------------------
// Client-server neutral effects interface
//-----------------------------------------------------------------------------

	typedef void* FileNameHandle_t;

	struct SndInfo_t
	{
		// Sound Guid
		int			m_nGuid;
		FileNameHandle_t m_filenameHandle;		// filesystem filename handle - call IFilesystem to conver this to a string
		int			m_nSoundSource;
		int			m_nChannel;
		// If a sound is being played through a speaker entity (e.g., on a monitor,), this is the
		//  entity upon which to show the lips moving, if the sound has sentence data
		int			m_nSpeakerEntity;
		float		m_flVolume;
		float		m_flLastSpatializedVolume;
		// Radius of this sound effect (spatialization is different within the radius)
		float		m_flRadius;
		int			m_nPitch;
		Vector		*m_pOrigin;
		Vector		*m_pDirection;

		// if true, assume sound source can move and update according to entity
		bool		m_bUpdatePositions;
		// true if playing linked sentence
		bool		m_bIsSentence;
		// if true, bypass all dsp processing for this sound (ie: music)	
		bool		m_bDryMix;
		// true if sound is playing through in-game speaker entity.
		bool		m_bSpeaker;
		// for snd_show, networked sounds get colored differently than local sounds
		bool		m_bFromServer;
	};

	//-----------------------------------------------------------------------------
	// Hearing info
	//-----------------------------------------------------------------------------
	struct AudioState_t
	{
		Vector m_Origin;
		QAngle m_Angles;
		bool m_bIsUnderwater;
	};

	namespace TABLE
	{
		namespace IEngineSound
		{
			enum
			{
				EmitSound1 = 5 ,
				EmitSound2 = 6
			};
		}
	}

	class IEngineSound
	{
	public:
		// Precache a particular sample
		virtual bool PrecacheSound( const char *pSample , bool bPreload = false , bool bIsUISound = false ) = 0;
		virtual bool IsSoundPrecached( const char *pSample ) = 0;
		virtual void PrefetchSound( const char *pSample ) = 0;
		virtual bool IsLoopingSound( const char *pSample ) = 0;

		// Just loads the file header and checks for duration (not hooked up for .mp3's yet)
		// Is accessible to server and client though
		virtual float GetSoundDuration( const char *pSample ) = 0;

		// Pitch of 100 is no pitch shift.  Pitch > 100 up to 255 is a higher pitch, pitch < 100
		// down to 1 is a lower pitch.   150 to 70 is the realistic range.
		// EmitSound with pitch != 100 should be used sparingly, as it's not quite as
		// fast (the pitchshift mixer is not native coded).

		// NOTE: setting iEntIndex to -1 will cause the sound to be emitted from the local
		// player (client-side only)
		virtual int EmitSound1( IRecipientFilter& filter , int iEntIndex , int iChannel , const char *pSoundEntry , unsigned int nSoundEntryHash , const char *pSample ,
								float flVolume , soundlevel_t iSoundlevel , int nSeed , int iFlags = 0 , int iPitch = PITCH_NORM ,
								const Vector *pOrigin = NULL , const Vector *pDirection = NULL , CUtlVector< Vector >* pUtlVecOrigins = NULL , bool bUpdatePositions = true , float soundtime = 0.0f , int speakerentity = -1 ) = 0;

		virtual int EmitSound2( IRecipientFilter& filter , int iEntIndex , int iChannel , const char *pSoundEntry , unsigned int nSoundEntryHash , const char *pSample ,
								float flVolume , float flAttenuation , int nSeed , int iFlags = 0 , int iPitch = PITCH_NORM ,
								const Vector *pOrigin = NULL , const Vector *pDirection = NULL , CUtlVector< Vector >* pUtlVecOrigins = NULL , bool bUpdatePositions = true , float soundtime = 0.0f , int speakerentity = -1 ) = 0;

		virtual void EmitSentenceByIndex( IRecipientFilter& filter , int iEntIndex , int iChannel , int iSentenceIndex ,
										  float flVolume , soundlevel_t iSoundlevel , int nSeed , int iFlags = 0 , int iPitch = PITCH_NORM ,
										  const Vector *pOrigin = NULL , const Vector *pDirection = NULL , CUtlVector< Vector >* pUtlVecOrigins = NULL , bool bUpdatePositions = true , float soundtime = 0.0f , int speakerentity = -1 ) = 0;

		virtual void StopSound( int iEntIndex , int iChannel , const char *pSample , unsigned int nSoundEntryHash ) = 0;
	};
}
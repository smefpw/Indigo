#pragma once

#include "Definitions.hpp"

#include "CInput.hpp"
#include "IClientMode.hpp"
#include "CGlobalVarsBase.hpp"
#include "ClientClass.hpp"
#include "ButtonCode.h"

namespace SDK
{
	class bf_write;
	class CEngineSprite;

	namespace TABLE
	{
		namespace IBaseClientDLL
		{
			enum
			{
				FrameStageNotify = 36
			};
		}
	}

	class IBaseClientDLL {
	public:
		virtual int				Connect( CreateInterfaceFn appSystemFactory , CGlobalVarsBase *pGlobals ) = 0;
		virtual int				Disconnect( void ) = 0;
		virtual int				Init( CreateInterfaceFn appSystemFactory , CGlobalVarsBase *pGlobals ) = 0;
		virtual void			PostInit() = 0;
		virtual void			Shutdown( void ) = 0;
		virtual void			LevelInitPreEntity( char const* pMapName ) = 0;
		virtual void			LevelInitPostEntity() = 0;
		virtual void			LevelShutdown( void ) = 0;
		virtual ClientClass		*GetAllClasses( void ) = 0;
		virtual int				HudVidInit( void ) = 0;
		virtual void			HudProcessInput( bool bActive ) = 0;
		virtual void			HudUpdate( bool bActive ) = 0;
		virtual void			HudReset( void ) = 0;
		virtual void			HudText( const char * message ) = 0;
		virtual void			ShouldDrawDropdownConsole( void ) = 0;
		virtual void			IN_ActivateMouse( void ) = 0;
		virtual void			IN_DeactivateMouse( void ) = 0;
		virtual void			IN_Accumulate( void ) = 0;
		virtual void			IN_ClearStates( void ) = 0;
		virtual bool			IN_IsKeyDown( const char *name , bool& isdown ) = 0;
		virtual int				IN_KeyEvent( int eventcode , ButtonCode_t keynum , const char *pszCurrentBinding ) = 0;
		virtual void			CreateMove( int sequence_number , float input_sample_frametime , bool active ) = 0;
		virtual void			ExtraMouseSample( float frametime , bool active ) = 0;
		virtual bool			WriteUsercmdDeltaToBuffer( int nSlot , bf_write *buf , int from , int to , bool isnewcommand ) = 0;
		virtual void			EncodeUserCmdToBuffer( int nSlot , bf_write& buf , int slot ) = 0;
		virtual void			DecodeUserCmdFromBuffer( int nSlot , bf_read& buf , int slot ) = 0;
		virtual void			View_Render( vrect_t *rect ) = 0;
		virtual void			RenderView( const CViewSetup &view , int nClearFlags , int whatToDraw ) = 0;
		virtual void			View_Fade( ScreenFade_t *pSF ) = 0;
		virtual void			SetCrosshairAngle( const QAngle& angle ) = 0;
		virtual void			InitSprite( CEngineSprite *pSprite , const char *loadname ) = 0;
		virtual void			ShutdownSprite( CEngineSprite *pSprite ) = 0;
		virtual int				GetSpriteSize( void ) const = 0;
		virtual void			VoiceStatus( int entindex , int iSsSlot , qboolean bTalking ) = 0;
		virtual int				PlayerAudible( int audible ) = 0;
		virtual void			InstallStringTableCallback( char const *tableName ) = 0;
		virtual void			FrameStageNotify( ClientFrameStage_t curStage ) = 0;
		virtual bool			DispatchUserMessage( int msg_type , int unk1 , int unk2 , bf_read &msg_data ) = 0;
	};
}
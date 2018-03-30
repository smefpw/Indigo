#pragma once

#include "../../Engine/Engine.h"
#include "../../SDK/Spoofed.h"

class CMisc
{
public:
//[swap_lines]
	void OnRender();
	void OnCreateMove( CUserCmd* pCmd );
	void FrameStageNotify(ClientFrameStage_t Stage);
#define POSTPROCESS_VALUE
	void OnDrawModelExecute();
	void OnPlaySound( const char* pszSoundName );

	void OnOverrideView( CViewSetup* pSetup );
	void OnGetViewModelFOV( float& fov );

	vector<int> GetObservervators( int playerId );
	void FakeLag(bool & bSendPacket);
	void OnRenderSpectatorList();
//[/swap_lines]
};
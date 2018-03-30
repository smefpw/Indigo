#pragma once

#include "../../Engine/Engine.h"

namespace Engine
{
	class CPlayer;
}

using namespace Engine;

class CRadar
{
public:
//[swap_lines]
	void RadarRange( float* x , float* y , float range );
	void CalcRadarPoint( Vector vOrigin , int& screenx , int& screeny );

	void OnRenderPlayer();
	void OnRenderSounds();
	void OnRender();
//[/swap_lines]
};
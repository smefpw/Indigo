#pragma once
#include "../../Engine/Engine.h"

using namespace Engine;

class AntiAim
{
private:
	int ChokedTicks;
public:
	void LegitAA(CUserCmd* cmd, bool bSendPacket);
};
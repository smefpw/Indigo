#pragma once

#include "Engine.h"

using GetBool_t = bool(__thiscall*)(void*);

namespace Engine
{
	namespace Hook
	{
		bool Initialize();
		void Shutdown();
	}
}
#pragma once
#include "Engine.h"

namespace Engine
{
	struct EventLog_t 
	{
		float deathtime;
		string text;
		float alpha = 1.f;
	};
	class EventLogs
	{
	public:
		void AddToLog(const char* text, ...);
		void DrawLogs();
	private:
		vector<EventLog_t> logs;
	};
	extern EventLogs* EventLog;
}
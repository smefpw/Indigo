#pragma once
#include "Engine.h"

namespace Engine
{
	struct EventLog_t 
	{
		float deathtime;
		string text;
	};
	class EventLogs
	{
	public:
		void AddToLog(const char* text);
		void DrawLogs();
	private:
		vector<EventLog_t> logs;
	};
	extern EventLogs* EventLog;
}
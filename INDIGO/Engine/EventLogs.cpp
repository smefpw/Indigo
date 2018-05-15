#include "EventLogs.h"
using namespace Client;
#define LOG_DURATION 3.f
#define STAY_ON_SCREEN_DURATION 0.5f

namespace Engine
{
	void EventLogs::AddToLog(const char* text, ...)
	{
		EventLog_t log;

		// variadic functions cause ur gay
		char buffer[8192];
		va_list list;
		va_start(list, text);
		_vsnprintf(buffer, sizeof(buffer) - 1, text, list);
		va_end(list);
		buffer[sizeof(buffer) - 1] = 0;

		// Construct our log.
		log.text = buffer;
		log.deathtime = Interfaces::GlobalVars()->curtime + LOG_DURATION;

		// Push our log.
		PrintToConsole(buffer);
		logs.emplace_back(log);
	}

	void EventLogs::DrawLogs()
	{
		if (logs.size() > 15) // Remove the first log if there are more than 15 logs.
			logs.erase(logs.begin() + 1);

		for (size_t i = 0; i < logs.size(); i++) 
		{
			float v22 = (logs[i].deathtime - Interfaces::GlobalVars()->curtime) + STAY_ON_SCREEN_DURATION;

			if (v22 < STAY_ON_SCREEN_DURATION) { // Remove that log if death time has reached.
				logs.erase(logs.begin() + i);
				continue;
			}

			float v23 = 255.f * (v22 / LOG_DURATION);
			g_pRender->TextToConsole(5, 2 + (12 * i), false, true, Color(255, 255, 255, (int)clampMinMax(v23, 0.f, 255.f)), logs[i].text.c_str());
		}
	}
	EventLogs* EventLog = new EventLogs();
}
#include "EventLogs.h"
using namespace Client;
#define LOG_DURATION 5.f
#define STAY_ON_SCREEN_DURATION 3.f

namespace Engine
{
	void EventLogs::AddToLog(const char* text, ...)
	{
		EventLog_t log;

		char buffer[8192];
		va_list list;
		va_start(list, text);
		_vsnprintf(buffer, sizeof(buffer) - 1, text, list);
		va_end(list);
		buffer[sizeof(buffer) - 1] = 0;

		log.text = buffer;
		log.deathtime = Interfaces::GlobalVars()->curtime + LOG_DURATION;

		PrintToConsole(buffer);
		logs.emplace_back(log);
	}

	void EventLogs::DrawLogs()
	{
		if (logs.size() > 15)
			logs.erase(logs.begin() + 1);

		for (size_t i = 0; i < logs.size(); i++) 
		{
			float v22 = (logs[i].deathtime - Interfaces::GlobalVars()->curtime);

			if (v22 < STAY_ON_SCREEN_DURATION) 
				logs[i].alpha -= 0.1f * (1.f / (LOG_DURATION - STAY_ON_SCREEN_DURATION));

			if (v22 < 0 || logs[i].alpha <= 0) {
				logs.erase(logs.begin() + i);
				continue;
			}

			g_pRender->TextToConsole(5, 2 + (12 * i), false, true, Color(255, 255, 255, (int)clampMinMax((255.f * logs[i].alpha), 0.f, 255.f)), logs[i].text.c_str());
		}
	}
	EventLogs* EventLog = new EventLogs();
}
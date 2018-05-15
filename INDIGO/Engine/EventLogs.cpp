#include "EventLogs.h"
using namespace Client;
#define LOG_DURATION 3.f

namespace Engine
{
	void EventLogs::AddToLog(const char* text)
	{
		EventLog_t log;
		log.text = text;
		log.deathtime = Interfaces::GlobalVars()->curtime + LOG_DURATION;
		PrintToConsole(text);
		logs.emplace_back(log);
	}

	void EventLogs::DrawLogs()
	{
		if (logs.size() > 15)
			logs.erase(logs.begin() + 1);

		for (size_t i = 0; i < logs.size(); i++) 
		{
			float diff = logs[i].deathtime - Interfaces::GlobalVars()->curtime;
			if (diff < 0) {
				logs.erase(logs.begin() + i);
				continue;
			}

			float alpha = 0.8f - (diff / 0.8f);
			g_pRender->TextToConsole(5, 5 + (12 * i), false, true, Color(255, 255, 255, (int)(255.f * alpha)), logs[i].text.c_str());
		}

	}
	EventLogs* EventLog = new EventLogs();
}
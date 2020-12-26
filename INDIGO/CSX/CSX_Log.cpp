#include "CSX_Log.h"
#include <fstream>
#include "../Cheat/Client.h"

namespace CSX {
	string Log::LogFile = Client::LogFile;
	ofstream Log::m_Logfile;

	/* Write Log */
	void Log::Add(const char* format, ...) {
		char sMessage[256] = { 0 };
		va_list args;
		va_start(args, format);
		vsprintf_s(sMessage, format, args);
		m_Logfile.open(LogFile.c_str(), ios::out | ios::app);
		m_Logfile << sMessage << endl;
		m_Logfile.close();
		va_end( args );
	}

	void Log::Add(const string& Msg) {
		m_Logfile.open( LogFile.c_str(), ios::out | ios::app);
		m_Logfile << Msg << endl;
		m_Logfile.close();
	}

	/*Delete the log - to make things cleaner :)*/
	void Log::Delete() {
		std::remove(Client::LogFile.c_str());
	}
}
#pragma once
#include "CSX.h"

namespace CSX {
	class Log {
	public:
//[swap_lines]
		/* Write Log */
		static void Add(const char* format, ...);
		static void Add(const string& Msg);
		static void Delete();
		static string LogFile;
		static ofstream m_Logfile;
//[/swap_lines]
	};
}
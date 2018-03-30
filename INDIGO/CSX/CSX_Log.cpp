#include "CSX_Log.h"
#include <fstream>

//[enc_string_enable /]

namespace CSX
{
	string Log::LogFile = /*CSX::Utils::GetModuleBaseDir( GetModuleHandleA( 0 ) )*/*CSX::Utils::GetHackWorkingDirectory().c_str() + "\\log.txt";
	ofstream Log::m_Logfile;

	/* Write Log */
//[junk_enable /]
	void Log::Add( const char * format , ... )
	{
		char sMessage[256] = { 0 };
		va_list args;
		va_start( args , format );
		vsprintf_s( sMessage , format , args );
		m_Logfile.open( LogFile.c_str() , ios::out | ios::app );
		m_Logfile << sMessage << endl;
		m_Logfile.close();
		va_end( args );
	}

	void Log::Add( const string& Msg )
	{
		m_Logfile.open( LogFile.c_str() , ios::out | ios::app );
		m_Logfile << Msg << endl;
		m_Logfile.close();
	}
}
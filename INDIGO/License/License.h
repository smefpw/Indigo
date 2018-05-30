#pragma once

#include "../Main/Main.h"

//[enc_string_enable /]

#define HOST			"indigocsgo.ru"
#define PATH			"/WebPanel/"

#define HOST_GATE		"gate.php?serial="
#define HOST_KEY_GATE	"license-success-ok-"

#define HOST_CHECK		"check.php?serial="
#define HOST_KEY_CHECK	"D2DF62F3E61D4696-"

#define CHEAT_VERSION	"13"

class CLicense
{
private:
	string	GetUrlData( string url );
	string	StringToHex( const string input );
	string	GetHashText( const void * data , const size_t data_size );

	string	GetHwUID();
	DWORD	GetVolumeID();
	string	GetCompUserName( bool User );
	string	GetSerialKey();
	string	GetHashSerialKey();

public:

	string	GetSerial();
	string	GetSerial64();
	string  GetIP( string hostname );
	bool	CheckLicenseURL( string URL , string GATE , string KEY );
	bool	CheckLicense();
};

string base64_encode( char const* bytes_to_encode , unsigned int in_len );

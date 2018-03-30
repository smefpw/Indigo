#pragma once

#include "CSX.h"

namespace CSX
{
	namespace Cvar
	{
//[swap_lines]
		void	InitPath( const char* szPath );

		int		LoadCvar( char* szSection , char* szKey , int DefaultValue );
		string	LoadCvar( char* szSection , char* szKey , string DefaultValue );
		float	LoadCvar( char* szSection , char* szKey , float DefaultValue );

		void	SaveCvar( char* szSection , char* szKey , int Value );
		void	SaveCvar( char* szSection , char* szKey , float Value );
		void	SaveCvar( char* szSection , char* szKey , string Value );
//[/swap_lines]
	}
}
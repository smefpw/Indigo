#pragma once

#include "Engine.h"

namespace Engine
{
	class NetVarManager
	{
	public:
//[swap_lines]
		bool Init( ClientClass* pClientClass );

		DWORD GetOffset( const char* tableName , const char* propName );
		bool HookProp( const char* tableName , const char* propName , RecvVarProxyFn function );
//[/swap_lines]
	private:
//[swap_lines]
		DWORD dwGetProp( const char* tableName , const char* propName , RecvProp** prop = 0 );
		DWORD dwGetProp( RecvTable* recvTable , const char* propName , RecvProp** prop = 0 );
//[/swap_lines]
		RecvTable* GetTable( const char* tableName );
		vector<RecvTable*> m_tables;
	};

	extern NetVarManager g_NetVar;
}
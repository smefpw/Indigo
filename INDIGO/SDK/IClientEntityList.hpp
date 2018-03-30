#pragma once

#include "Definitions.hpp"

namespace SDK
{
	class IClientNetworkable;
	class IClientEntity;

	class IClientEntityList {
	public:
		virtual IClientNetworkable*   GetClientNetworkable( int entnum ) = 0;
		virtual IClientNetworkable*   GetClientNetworkableFromHandle( CBaseHandle hEnt ) = 0;
		virtual IClientUnknown*		  GetClientUnknownFromHandle( CBaseHandle hEnt ) = 0;
		virtual IClientEntity*        GetClientEntity( int entNum ) = 0;
		virtual IClientEntity*        GetClientEntityFromHandle( PVOID hEnt ) = 0;
		virtual int                   NumberOfEntities( bool bIncludeNonNetworkable ) = 0;
		virtual int                   GetHighestEntityIndex( void ) = 0;
		virtual void                  SetMaxEntities( int maxEnts ) = 0;
		virtual int                   GetMaxEntities() = 0;
	};
}
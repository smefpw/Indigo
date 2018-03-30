#pragma once

#include "Definitions.hpp"

namespace SDK
{
	class IClientUnknown;
	class ClientClass;
	class bf_read;
	
	enum DataUpdateType_t
	{
		DATA_UPDATE_CREATED = 0,
	//	DATA_UPDATE_ENTERED_PVS,
		DATA_UPDATE_DATATABLE_CHANGED,
	//	DATA_UPDATE_LEFT_PVS,
	//	DATA_UPDATE_DESTROYED,
	};

	class IClientNetworkable {
	public:
		virtual IClientUnknown*  GetIClientUnknown() = 0;
		virtual void             Release() = 0;
		virtual ClientClass*     GetClientClass() = 0;
		virtual void             NotifyShouldTransmit( int state ) = 0;
		virtual void             OnPreDataChanged( DataUpdateType_t updateType ) = 0;
		virtual void             OnDataChanged( DataUpdateType_t updateType ) = 0;
		virtual void             PreDataUpdate( DataUpdateType_t updateType ) = 0;
		virtual void             PostDataUpdate( DataUpdateType_t updateType ) = 0;
		virtual void             unknown( void ) = 0;
		virtual bool             IsDormant( void ) = 0;
		virtual int              EntIndex( void ) const = 0;
		virtual void             ReceiveMessage( int classID , bf_read& msg ) = 0;
		virtual void*            GetDataTableBasePtr() = 0;
		virtual void             SetDestroyedOnRecreateEntities( void ) = 0;
	};
}
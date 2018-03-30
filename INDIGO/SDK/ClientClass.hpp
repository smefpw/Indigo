#pragma once

#include "Definitions.hpp"

#include "Recv.hpp"
#include "IClientNetworkable.hpp"

namespace SDK
{
	class ClientClass;

	typedef IClientNetworkable*   ( *CreateClientClassFn )( int entnum , int serialNum );
	typedef IClientNetworkable*   ( *CreateEventFn )( );

	class ClientClass {
	public:
		const char* GetName() { return m_pNetworkName; }
		CreateClientClassFn      m_pCreateFn;
		CreateEventFn            m_pCreateEventFn;
		char*                    m_pNetworkName;
		RecvTable*               m_pRecvTable;
		ClientClass*             m_pNext;
		int                      m_ClassID;
	};
}
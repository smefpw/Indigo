#pragma once

#include "SDK.h"

namespace SDK
{
	namespace TABLE
	{
		namespace IGameEvent
		{
			enum
			{
				GetName = 1,
				GetInt = 6,
				GetFloat = 8,
				GetString = 9,
				SetString = 16
			};
		}

		namespace IGameEventManager2
		{
			enum
			{
				FireEventClientSide = 9
			};
		}
	}

	class IGameEvent
	{
	public:
		const char* GetName()
		{
			VirtualFn(const char*)(PVOID);
			return GetMethod< OriginalFn >(this, TABLE::IGameEvent::GetName)(this);
		}

		int GetInt(const char* szKeyName, int nDefault = 0)
		{
			VirtualFn(int)(PVOID, const char*, int);
			return GetMethod< OriginalFn >(this, TABLE::IGameEvent::GetInt)(this, szKeyName, nDefault);
		}

		float GetFloat(const char *szkeyName = NULL, float defaultValue = 0.0f)
		{
			VirtualFn(float)(PVOID, const char*, float);
			return GetMethod< OriginalFn >(this, TABLE::IGameEvent::GetFloat)(this, szkeyName, defaultValue);
		}

		const char* GetString(const char* szKeyName)
		{
			VirtualFn(const char*)(PVOID, const char*, int);
			return GetMethod< OriginalFn >(this, TABLE::IGameEvent::GetString)(this, szKeyName, 0);
		}

		void SetString(const char* szKeyName, const char* szValue)
		{
			VirtualFn(void)(PVOID, const char*, const char*);
			GetMethod< OriginalFn >(this, TABLE::IGameEvent::SetString)(this, szKeyName, szValue);
		}
	};

	class IGameEventListener2
	{
	public:
		virtual    ~IGameEventListener2(void) {};

		virtual void FireGameEvent(IGameEvent *event) = 0;
		virtual int  GetEventDebugID(void) = 0;
	};

	class IGameEventManager2
	{
	public:
		virtual	~IGameEventManager2(void) {};
		virtual int LoadEventsFromFile(const char *filename) = 0;
		virtual void  Reset() = 0;
		bool AddListener(IGameEventListener2 *listener, const char *name, bool bServerSide)
		{
			VirtualFn(bool)(PVOID, IGameEventListener2*, const char*, bool);
			return GetMethod< OriginalFn >(this, 3)(this, listener, name, bServerSide);
		}
		virtual bool FindListener(IGameEventListener2 *listener, const char *name) = 0;
		virtual void RemoveListener(IGameEventListener2 *listener) = 0;
		virtual IGameEvent *CreateEvent(const char *name, bool bForce = false, int *pCookie = NULL) = 0;
		virtual bool FireEvent(IGameEvent *event, bool bDontBroadcast = false) = 0;
		bool FireEventClientSide(IGameEvent* pEvent)
		{
			VirtualFn(bool)(PVOID, IGameEvent*);
			return GetMethod< OriginalFn >(this, TABLE::IGameEventManager2::FireEventClientSide)(this, pEvent);
		}
		virtual IGameEvent *DuplicateEvent(IGameEvent *event) = 0;
		virtual void FreeEvent(IGameEvent *event) = 0;
		virtual bool SerializeEvent(IGameEvent *event, bf_write *buf) = 0;
		virtual IGameEvent *UnserializeEvent(bf_read *buf) = 0;
	};
}
#pragma once

namespace SDK
{
		class ConVar
	{
	public:
		Color GetColor()
		{
			using OriginalFn = Color(__thiscall *)(void *);
			return GetMethod<OriginalFn>(this, 10)(this);
		}

		const char* GetString()
		{
			using OriginalFn = const char *(__thiscall *)(void *);
			return GetMethod<OriginalFn>(this, 11)(this);
		}

		float GetFloat()
		{
			using OriginalFn = float(__thiscall *)(void *);
			return GetMethod<OriginalFn>(this, 12)(this);
		}

		int GetInt()
		{
			using OriginalFn = int(__thiscall *)(void *);
			return GetMethod<OriginalFn>(this, 13)(this);
		}

		void SetValue(const char *value)
		{
			using OriginalFn = void(__thiscall *)(void *, const char *);
			GetMethod<OriginalFn>(this, 14)(this, value);
		}

		void SetValue(float value)
		{
			using OriginalFn = void(__thiscall *)(void *, float);
			GetMethod<OriginalFn>(this, 15)(this, value);
		}

		void SetValue(int value)
		{
			using OriginalFn = void(__thiscall *)(void *, int);
			GetMethod<OriginalFn>(this, 16)(this, value);
		}

		void SetValue(Color value)
		{
			using OriginalFn = void(__thiscall *)(void *, Color);
			GetMethod<OriginalFn>(this, 17)(this, value);
		}

		ConVar* FindVar(const char *var_name)
		{
			typedef ConVar*(__thiscall* OriginalFn)(void*, const char*);
			return GetMethod<OriginalFn>(this, 15)(this, var_name);
		}

		void* RegisterConCommand(ConVar *pCommandBase)
		{
			typedef ConVar*(__thiscall* OriginalFn)(void*, ConVar*);
			return GetMethod<OriginalFn>(this, 9)(this, pCommandBase);
		}

		void* UnregisterConCommand(ConVar *pCommandBase)
		{
			typedef ConVar*(__thiscall* OriginalFn)(void*, ConVar*);
			return GetMethod<OriginalFn>(this, 10)(this, pCommandBase);
		}

		char pad_0x0000[0x4]; //0x0000
		ConVar *pNext; //0x0004
		int32_t bRegistered; //0x0008
		char *pszName; //0x000C
		char *pszHelpString; //0x0010
		int32_t nFlags; //0x0014
		char pad_0x0018[0x4]; //0x0018
		ConVar *pParent; //0x001C
		char *pszDefaultValue; //0x0020
		char *strString; //0x0024
		int32_t StringLength; //0x0028
		float fValue; //0x002C
		int32_t nValue; //0x0030
		int32_t bHasMin; //0x0034
		float fMinVal; //0x0038
		int32_t bHasMax; //0x003C
		float fMaxVal; //0x0040
		void *fnChangeCallback; //0x0044
	};
}

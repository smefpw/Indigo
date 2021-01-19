#include "CSX_Cvar.h"

#define ENCRYPTION_KEY 0xA67D7
//[junk_enable /]
namespace CSX {
	namespace Cvar {
		string IniFile = "";
		size_t IniFileLen = 0;

		void IniFileED() {
			for (size_t i = 0; i < IniFileLen; i++) {
				IniFile[i] ^= ENCRYPTION_KEY;
			}
		}

		int InitPath(const char* szPath) {
			try {
				IniFile = szPath;
				IniFileLen = IniFile.size();
				IniFileED();
				return 0; //success!
			}
			catch (...) {
				return 1; //fail!
			}
		}

		int LoadCvar(char* szSection, char* szKey, int DefaultValue) {
			IniFileED();
			char IntValue[16] = { 0 };
			GetPrivateProfileStringA(szSection, szKey, to_string(DefaultValue).c_str(), IntValue, sizeof(IntValue), IniFile.c_str());
			IniFileED();
			return atoi(IntValue);
		}

		string LoadCvar(char* szSection, char* szKey, string DefaultValue) {
			IniFileED();
			char cTempString[16] = { 0 };
			GetPrivateProfileStringA(szSection, szKey, DefaultValue.c_str(), cTempString, sizeof(cTempString), IniFile.c_str());
			IniFileED();
			return string(cTempString);
		}

		float LoadCvar(char* szSection, char* szKey, float DefaultValue) {
			IniFileED();
			char FloatValue[16] = { 0 };
			GetPrivateProfileStringA(szSection, szKey, to_string(DefaultValue).c_str(), FloatValue, sizeof(FloatValue), IniFile.c_str());
			IniFileED();
			return (float)atof(FloatValue);
		}

		void SaveCvar(char* szSection, char* szKey, int Value) {
			string IntValue = to_string(Value);
			IniFileED();
			WritePrivateProfileStringA(szSection, szKey, IntValue.c_str(), IniFile.c_str());
			IniFileED();
		}

		void SaveCvar(char* szSection, char* szKey, float Value) {
			string FloatValue = to_string(Value);
			IniFileED();
			WritePrivateProfileStringA(szSection, szKey, FloatValue.c_str(), IniFile.c_str());
			IniFileED();
		}

		void SaveCvar(char* szSection, char* szKey, string Value) {
			IniFileED();
			WritePrivateProfileStringA(szSection, szKey, Value.c_str(), IniFile.c_str());
			IniFileED();
		}
	}
}
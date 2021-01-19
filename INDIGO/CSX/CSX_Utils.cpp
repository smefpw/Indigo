#include "CSX_Utils.h"
#include <locale>
#include <codecvt>
#include <sstream>

static const DWORD dwModuleDelay = 100;
//[junk_enable /]
namespace CSX {
	namespace Utils {
		/* Wait dwMsec Load Module */
		bool IsModuleLoad(PCHAR szModule, DWORD dwMsec) {
			HMODULE hModule = GetModuleHandleA(szModule);
			if (!hModule) {
				DWORD dwMsecFind = 0;
				while (!hModule) {
					if (dwMsecFind == dwMsec) {
						return false;
					}

					hModule = GetModuleHandleA(szModule);
					HANDLE hEvent = CreateEventA(0, true, false, 0);
					WaitForSingleObject(hEvent, dwModuleDelay);
					CloseHandle(hEvent);
					dwMsecFind += dwModuleDelay;
				}
			}
			return true;
		}

		/* Get Current Process Path */
		string GetCurrentProcessPath() {
			string ProcessPath = "";
			char szFileName[MAX_PATH] = { 0 };
			if (GetModuleFileNameA(0, szFileName, MAX_PATH)) {
				ProcessPath = szFileName;
			}
			return ProcessPath;
		}

		/* Get Current Process Name */
		string GetCurrentProcessName() {
			string ProcessName = GetCurrentProcessPath();
			if (!ProcessName.empty()) {
				ProcessName = ProcessName.erase(0, ProcessName.find_last_of("\\/") + 1);
				return ProcessName;
			}
			return ""; //maybe add else here?
		}

		/* Get Module File Path */
		string GetModuleFilePath(HMODULE hModule) {
			string ModuleName = "";
			char szFileName[MAX_PATH] = { 0 };

			if (GetModuleFileNameA(hModule, szFileName, MAX_PATH)) {
				ModuleName = szFileName;
			}
			return ModuleName;
		}

		/* Get Module Dir */
		string GetModuleBaseDir(HMODULE hModule) {
			string ModulePath = GetModuleFilePath(hModule);
			return ModulePath.substr(0, ModulePath.find_last_of("\\/"));
		}

		/* Random Int Range */
		int RandomIntRange(int min, int max) {
			static bool first = true;
			if (first) {
				srand(GetTickCount());
				first = false;
			}
			return min + rand() % (max - min);
		}

		/* Get hwProfile GUID */
		string GetHwProfileGUID() {
			HW_PROFILE_INFO hwProfileInfo = { 0 };
			if (GetCurrentHwProfileA(&hwProfileInfo) != NULL) {
				return hwProfileInfo.szHwProfileGuid;
			}
			else {
				return "null";
			}
			return hwProfileInfo.szHwProfileGuid;
		}

		/* Return False If Read Ptr Error */
		bool IsBadReadPtr(PVOID pPointer) {
			MEMORY_BASIC_INFORMATION mbi = { 0 };
			if (VirtualQuery(pPointer, &mbi, sizeof(mbi))) {
				DWORD mask = (PAGE_READONLY | PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY);

				bool ret = !(mbi.Protect & mask);

				if (mbi.Protect & (PAGE_GUARD | PAGE_NOACCESS))
					ret = true;

				return ret;
			}
			return true;
		}

		/* Unicode To Utf8 Convert */
		BSTR ConvertStringToBSTR_UTF8(const char* szBuff) {
			if (!szBuff) {
				return NULL;
			}
			DWORD cwch;
			BSTR wsOut(NULL);

			if (cwch = MultiByteToWideChar(CP_UTF8, 0, szBuff, -1, NULL, 0)) {
				cwch--;
				wsOut = SysAllocStringLen( NULL , cwch );
				if (wsOut) {
					if (!MultiByteToWideChar(CP_UTF8, 0, szBuff, -1, wsOut, cwch)) {
						if (ERROR_INSUFFICIENT_BUFFER == ::GetLastError()) {
							return wsOut;
						}
						SysFreeString(wsOut);
						wsOut = NULL;
					}
				}
			};
			return wsOut;
		}

		std::string WstringToString(std::wstring wstr) {
			wstring_convert<codecvt_utf8<wchar_t>, wchar_t> converter;
			try {
				return converter.to_bytes(wstr);
			}
			catch (std::range_error) {
				stringstream s;
				s << wstr.c_str();
				return s.str();
			}
		}

		std::wstring StringToWstring(std::string str) {
			wstring_convert<codecvt_utf8<wchar_t>, wchar_t> converter;

			try {
				return converter.from_bytes(str);
			}
			catch (range_error) {
				wostringstream s;
				s << str.c_str();
				return s.str();
			}
		}

		std::string GetHackWorkingDirectory() {
			HKEY rKey;
			TCHAR Path[256] = {0};
			DWORD RegetPath=sizeof(Path);
			RegOpenKeyEx(HKEY_CURRENT_USER, "SOFTWARE\\x2", NULL, KEY_QUERY_VALUE, &rKey);
			RegQueryValueEx(rKey,"path", NULL, NULL,(LPBYTE)&Path, &RegetPath);
			return Path;
		};
	}
}
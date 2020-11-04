#include <Windows.h>
#include <Psapi.h>
#include <string>
#include <regex>
#include "HookFunctions.h"

BOOL WINAPI DllMain(HINSTANCE hInst, DWORD reason, LPVOID)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		HMODULE baseModule = GetModuleHandle(NULL);
		TCHAR baseModuleName[MAX_PATH];
		GetModuleFileName(baseModule, baseModuleName, sizeof(baseModuleName));
		int indexOfLastPathNode = StrEndsWith(baseModuleName, sizeof(baseModuleName), '\\') + 1;
		
		TCHAR exeName[MAX_PATH];
		auto lenght = std::wcslen(baseModuleName);
		wcscpy_s(exeName, lenght, baseModuleName + indexOfLastPathNode);
		StrToLower(exeName);

		std::wregex pattern(L"deadspace2?(.+)\\.exe");
		if (std::regex_search(exeName, pattern))
		{
			MODULEINFO info;
			GetModuleInformation(GetCurrentProcess(), baseModule, &info, sizeof(info));

			const char* LookForStr = "XCENTKOWSK_C9C50BC_08B592A_v3";
			const char* ReplaceStr = "XPATEL1026_CB19946_663E415_v3";
			auto lenghtOfStr = strlen(LookForStr);
			
			for (unsigned int i = 0; i < info.SizeOfImage-lenghtOfStr; i++)
			{
				if(ScanAndOverride((intptr_t)baseModule + i, lenghtOfStr, LookForStr, ReplaceStr))
					return TRUE;
			}
			MessageBox(NULL, L"Nothing found, so nothing was overriden!", L"Warning", MB_ICONWARNING | MB_OK);
		}
		else
		{
			std::wstring messageBoxText = L"Unrecognized exe name: " + (std::wstring)exeName + L"\nFull path: " + (std::wstring)baseModuleName;
			MessageBox(NULL, messageBoxText.c_str(), L"Warning", MB_ICONWARNING | MB_OK);
		}
	}
	else if (reason == DLL_PROCESS_DETACH) {
	}
	return TRUE;
}

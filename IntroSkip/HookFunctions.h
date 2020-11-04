#pragma once
#include <Windows.h>
#include <string>

static bool OverrideCharArray(intptr_t addressOfText, const char* textToOverrideWith, const size_t OriginalLenght)
{
	if (std::strlen(textToOverrideWith) > OriginalLenght)
	{
		MessageBox(NULL, L"Char to override with is longer than original char", L"Error", MB_OK | MB_ICONERROR);
		return false;
	}

	DWORD curProtectionFlag;
	VirtualProtect((void*)addressOfText, OriginalLenght, PAGE_EXECUTE_READWRITE, &curProtectionFlag);
	memcpy((void*)addressOfText, textToOverrideWith, OriginalLenght);
	DWORD temp;
	VirtualProtect((void*)addressOfText, OriginalLenght, curProtectionFlag, &temp);

	return true;
}

static bool ScanAndOverride(intptr_t address, int strLenght, const char* textToFind, const char* textToOverrideWith)
{
	char* readFromMemory = (char*)address;
	for (auto i = 0; i < strLenght; i++)
	{
		if (readFromMemory == 0 || readFromMemory[i] != textToFind[i])
			return false;
	}

	OverrideCharArray(address, textToOverrideWith, strLenght);

	return true;
}

static bool OverrideByte(intptr_t addressOfByteToOverride, byte ByteToOverrideWith)
{
	DWORD curProtectionFlag;
	VirtualProtect((void*)addressOfByteToOverride, ByteToOverrideWith, PAGE_EXECUTE_READWRITE, &curProtectionFlag);
	*(byte*)addressOfByteToOverride = ByteToOverrideWith;
	DWORD temp;
	VirtualProtect((void*)addressOfByteToOverride, ByteToOverrideWith, curProtectionFlag, &temp);

	return true;
}

static int StrEndsWith(WCHAR* chrArray, int lenght, char character)
{
	int pos = -1;
	for (int i = 0; i < lenght; i++)
	{
		if (chrArray[i] == character)
			pos = i;
	}

	return pos;
}

static void StrToLower(WCHAR* chrArray)
{
	int lenght = std::wcslen(chrArray);
	for (int i = 0; i < lenght; i++)
	{
		chrArray[i] = ::tolower(chrArray[i]);
	}
}
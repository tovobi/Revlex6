#include "stdafx.h"
#include "MemMagic.h"




UINT_PTR  MemMagic::GetProcessBaseAddress(DWORD processID, HANDLE *processHandle)
{
	DWORD_PTR   baseAddress = 0;
	HMODULE     *moduleArray;
	LPBYTE      moduleArrayBytes;
	DWORD       bytesRequired;

	if (*processHandle)
	{
		if (EnumProcessModulesEx(*processHandle, NULL, 0, &bytesRequired, 0x02))
		{
			if (bytesRequired)
			{
				moduleArrayBytes = (LPBYTE)LocalAlloc(LPTR, bytesRequired);

				if (moduleArrayBytes)
				{
					unsigned int moduleCount;

					moduleCount = bytesRequired / sizeof(HMODULE);
					moduleArray = (HMODULE *)moduleArrayBytes;

					if (EnumProcessModulesEx(*processHandle, moduleArray, bytesRequired, &bytesRequired, 0x02))
					{
						baseAddress = (DWORD_PTR)moduleArray[0];
					}
					LocalFree(moduleArrayBytes);
				}
			}
		}
	}
	return baseAddress;
}


MemMagic::MemMagic(const LPCWSTR &windowName)
{
	this->windowHandle = FindWindow(nullptr, windowName);
}
/*MemMagic::MemMagic()
{
}
*/
bool MemMagic::Append()
{
	DWORD PID;
	GetWindowThreadProcessId(this->windowHandle, &PID);
	this->processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);
	if (processHandle == 0) {
		return false;
	}
	this->baseAddress = GetProcessBaseAddress(PID, &processHandle);
	return  true;
}
unsigned int MemMagic::GetBaseAddress()
{
	return this->baseAddress;
}
byte MemMagic::MemReadByte(const unsigned int &address)
{
	byte value = 0;
	ReadProcessMemory(processHandle, (void*)address, &value, sizeof(value), 0);
	return value;
}
unsigned int MemMagic::MemReadUInt(const unsigned int &address)
{
	unsigned int value = 0;
	ReadProcessMemory(processHandle, (void*)address, &value, sizeof(value), 0);
	return value;
}
unsigned long long MemMagic::MemReadULong(const unsigned int &address)
{
	unsigned long long value = 0ULL;
	ReadProcessMemory(processHandle, (void*)address, &value, sizeof(value), 0);
	return value;
}
float MemMagic::MemReadFloat(const unsigned int &address)
{
	float value = 0.0f;
	ReadProcessMemory(processHandle, (void*)address, &value, sizeof(value), 0);
	return value;
}
std::string MemMagic::MemReadString(const unsigned int &address)
{
	std::string name;
	char value[64];
	ReadProcessMemory(processHandle, (void*)address, &value, sizeof(value), 0); //7 = 8 bytes insgesamt
	for (char outChar : value)
	{
		if (outChar != '\0')
		{
			name += outChar;
		}
		else
		{
			break;
		}
	}
	return name;
}
void MemMagic::CloseMagicHandle()
{
	CloseHandle(processHandle);
}



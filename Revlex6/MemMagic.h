#pragma once
#include "stdafx.h"
#include <iostream>
#include <string>
#include <vector>
#include <windows.h>
#include <psapi.h>


class MemMagic
{
private:
	UINT_PTR	baseAddress;

	UINT_PTR GetProcessBaseAddress(DWORD processID, HANDLE *processHandle);


public:
	//MemMagic();
	HWND		windowHandle;
	HANDLE		processHandle;
	MemMagic(const LPCWSTR &windowName);
	bool Append();
	unsigned int GetBaseAddress();
	byte MemReadByte(const unsigned int &address);
	unsigned int MemReadUInt(const unsigned int &address);
	unsigned long long MemReadULong(const unsigned int &address);
	float MemReadFloat(const unsigned int &address);
	std::string MemReadString(const unsigned int &address);
	void CloseMagicHandle();
};


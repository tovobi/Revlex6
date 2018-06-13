#pragma once
#include "stdafx.h"
#include <string>
#include <vector>
#include "WowHelpers.h"
#include "WowVector3d.h"

class InjectAsm
{
public:
	InjectAsm(const WowHelpers &wowHelper);
	InjectAsm();
	~InjectAsm();
	bool buildCodeCave(const DWORD &hookAddress);
	bool buildActionButtonCave(const DWORD &hookAddress);
	bool buildPetActionCave(const DWORD &hookAddress);
	bool setActionButton(const byte &actionBtn);
	bool setPetAction(const byte &actionBtn);
	bool changeEndScene(const DWORD &hookAddress);
	bool setTarget(const unsigned long long &guid);
	bool setSpell(const std::string &spellName);	
	bool setSpellLocation(const WowVector3d &loc);
	bool setDoAction(const byte &d);
	bool setSpellPlacementAction(const byte &d);
	bool setUseItemPtr(const DWORD &p);
	bool initialized = false;
protected:
	HANDLE hProc;
	unsigned int targetGuid1;
	unsigned int targetGuid2;
	DWORD codeCave = 0x7FEDAC;
	DWORD actionButtonCodeCave = 0x7FEF40;
	DWORD petActionCodeCave = 0x8304E6;
	DWORD useItemAsm = 0x7FEE94;
	DWORD placeSpellAsm = 0x7FEED8;
	byte doSpellCast = 0;
	DWORD itemObjPtr = 0;
	BOOL SetDebugPrivileges();


};


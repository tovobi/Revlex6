#pragma once
#include "stdafx.h"
#include <iostream>
#include <sstream>
#include <string>
#include <Windows.h>
#include "Auras.h"
#include <vector>
#include "MemMagic.h"
#include "WowPointers.h"
#include "wowObject.h"
#include "WowObjectLight.h"
#include <chrono>
#include <cpplinq.hpp>
#include "config.h"
#include "Spells.h"
#include <iterator>
#include "vector3dLog.h"
#include "Logging.h"
#include <map>

class WowHelpers
{
public:
	MemMagic wowMem;
	WowObject localPlayer;
	WowObject target;
	WowObject lastTarget;
	Vector3dLog foreCastObj;
	Logging myLog;
	//std::map<unsigned long,std::string> recentSpellList;
	struct recentSpell {
		unsigned long addedAtTime;
		std::string spellName;
		unsigned long releaseTime;
	};
	std::vector<recentSpell> recentSpellList;

	

	//WowObject forCastTarget; 
	/*unsigned long targetLastVectorCheck = 0.0;*/
	UINT_PTR base;
	/*unsigned long forCastTime = 0.0;*/
	unsigned int firstObject = 0;
	unsigned int staticClientConnection = 0;
	unsigned int clientConnection = 0;
	unsigned long blockScanObj = 150;
	unsigned long lastScanObj = 0;
	unsigned long blockScanObjLight = 75;
	unsigned long lastScanObjLight = 0;
	unsigned long blockScanAddRecentSpell = 150;
	unsigned long lastScanAddRecentSpell = 0;
	unsigned long blockScanRemoveRecentSpell = 150;
	unsigned long lastScanRemoveRecentSpell = 0;
	unsigned long spellBookUpdateIntervall = 900000;
	unsigned long lastSpellBookUpdate = 0;
	unsigned long uiUpdateIntervall = 1000;
	unsigned long lastUiUpdate = 0;
	bool textCaretActive = false;
	char petFollow = 0;
	char petAwareness = 0;
	unsigned int firstObjectOffset = WowPointers::ObjectManager::FIRSTOBJECTOFFSET;
	std::vector<bool> playerAllianceFaction;
	std::vector<bool> playerHordeFaction;
	std::vector<bool> allianceHostileFaction;
	std::vector<bool> hordeHostileFaction;
	std::vector<WowObject> cachedUnitlist;
	std::vector<Spells> spellsInSpellBook;
	unsigned long currentTime;

	WowHelpers(LPCWSTR);
	~WowHelpers();



	bool GetStaticClientConnection();
	bool GetBasicWowData();
	static std::string decToHexStr(const unsigned long long &s1);
	static std::wstring s2ws(const std::string& s);
	static void buildPackedGuid(unsigned __int64 guid, std::vector<BYTE> &result);
	static void splitGuid(const unsigned __int64 &guid, unsigned int &guid1, unsigned int &guid2);
	static std::vector<byte> ToBytePattern(const unsigned long long &a);
	static std::vector<byte> ToBytePattern(const DWORD &a);
	static std::vector<byte> ToBytePattern(const unsigned int &a);
	//WowVector3d calculateVector3dForecast(const unsigned long &foreCastTimeinMs = 1500);
	//void collectTargetVector3d();
	//WowVector3d lastTargetLoc;
	WowObject getItemObj(const int &id);
	std::vector<WowObject> getNearEnemies(const double &radius, const unsigned int &maxNumberToScan);
	std::vector<WowObject> getNearEnemiesMultiDamageTapped(const unsigned int &health, const std::string &spell, const unsigned long long targetGuid, const double &radius = 15, const unsigned int &maxNumberToScan = 5);
	std::vector<WowObject> getNearEnemiesMultiDamage(const unsigned int &health, const std::string &spell, const unsigned long long targetGuid, const double &radius = 15, const unsigned int &maxNumberToScan = 5);
	std::vector<WowObject> getNearEnemiesPetTaunt(const unsigned int &health, const std::string &spell, const unsigned long long targetGuid, const double &radius = 30, const unsigned int &maxNumberToScan = 15);
	std::vector<WowObject> getNearFriendlyPlayers(const double &radius, const unsigned int &maxNumberToScan);
	bool isEliteMob(WowObject target);
	void configSave();
	void configLoad();
	bool isHostile(const WowObject &unit);
	bool hasHostileTarget(const WowObject &humanPlayer, const WowObject &unit);
	bool hasFriendlyTarget(const WowObject &humanPlayer, const WowObject &unit);
	bool isHostile(const WowObjectLight &unit);
	bool hasHostileTarget(const WowObject &humanPlayer, const WowObjectLight &unit);
	bool hasFriendlyTarget(const WowObject &humanPlayer, const WowObjectLight &unit);
	bool hasTarget(const WowObject &unit);
	std::string mobNameFromBaseAddr(const unsigned int &baseAddr);
	std::string itemNameFromBaseAddr(const unsigned int &baseAddr);
	std::string itemNameFromBaseAddr2(const unsigned int &baseAddr);
	unsigned int itemTypeFromBaseAddr(const unsigned int &baseAddr);
	std::string playerNameFromGuid(const unsigned long long &guid);
	static std::string getDateTime(void);
	double nfmod(const double &a, const double &b);
	double GetAtan2ToUnit(const WowObject &src, const WowObject &dest);
	double getFacingToUnit(const WowObject &dest);
	void unbanMultiDmgObj();
	static unsigned long getTime();
	WowObject getTargetPlayer(const WowObject &unit);
	WowObject getTargetNpc(const WowObject &unit);
	std::string getSpellName(const unsigned int &id);
	std::vector<Auras> getUnitBuffs(const unsigned int &objBaseAddress);
	std::vector<Auras> getUnitDebuffs(const unsigned int &objBaseAddress);
	//bool unitHasBuff(const WowObject &unit, const unsigned int &id);
	//bool unitHasBuff(const WowObject &unit, const std::string &name);
	bool unitHasBuff2(const WowObject &unit, const unsigned int &id);
	bool unitHasBuff2(const WowObject &unit, const std::string &name);
	bool unitHasBuff2(const WowObjectLight &unit, const unsigned int &id);
	bool unitHasBuff2(const WowObjectLight &unit, const std::string &name);
	//bool unitHasDebuff(const WowObject &unit, const unsigned int &id);
	//bool unitHasDebuff(const WowObject &unit, const std::string &name);
	bool unitHasDebuff2(const WowObject &unit, const unsigned int &id);
	bool unitHasDebuff2(const WowObject &unit, const std::string &name);
	bool unitHasDebuff2(const WowObjectLight &unit, const unsigned int &id);
	bool unitHasDebuff2(const WowObjectLight &unit, const std::string &name);
	WowObjectLight WowObjectToWowObjectLight(const WowObject &obj);
	WowObject WowObjectLightToWowObject(const WowObjectLight &obj);
	unsigned int getStacksOfDebuff(WowObject unit, std::string aura);
	bool hasPvpEnabled(unsigned int unitFlags);
	bool isInCombat(unsigned int unitFlags);
	std::string getErrMsg();
	void getPlayerSpells();
	void getPetSpells();
	std::string *playerFastSpellList = new std::string[30000];
	bool spellsAndRank();
	std::map<std::string, unsigned int> allPlayerSpellsAndRank;
	unsigned int getSpellIdFromSpellName(const std::string &name);
	std::string getSpellNameFromSpellId(const unsigned int &id);
	bool spellExist(int spellId);
	bool spellExist(std::string spellName);
	void printSpells(std::vector<Spells> spellList);
	std::vector<SpellsInHistory> getPlayerSpellsOnCooldown();
	std::vector<SpellsInHistory> getPlayerSpellHistory();
	std::map<unsigned long long,unsigned long> multiDamageBanlist;
	float getSpellCooldown(std::string spellName);
	float getSpellCooldown(unsigned int spellID);
	
	bool globalCooldown();
	void scanObjLight();
	void scanObj();



};


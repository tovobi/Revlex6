#pragma once
#include <string>
#include <vector>
#include "WowHelpers.h"
#include "InjectAsm.h"
#include "RvxKeys.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <algorithm>
#include "WowVector3d.h"
#include "Logging.h"

using namespace std;

class PlayerRotation
{
public:
	PlayerRotation(WowHelpers &_wowHelperObj, InjectAsm &_wowInject);
	~PlayerRotation();
	float cdOffset = 0.1f;
	void baseRotation(InjectAsm &wowInject);

	void castSpellLocation(const std::string &spellName, const unsigned long &foreCastTime);
	void useItemLocation(const DWORD &objPtr, const unsigned long &travelTimeInMsPerM, unsigned long castTimeInMs = 0.0);
	void useItem(const DWORD &objPtr);
	void clickButton(byte btn);
	void petAction(byte action);
	void castSpell(const std::string &spellName, const WowObject &target, const unsigned long &blockTime = 0);
	void castSpell(const std::string &spellName, const WowObjectLight &target, const unsigned long &blockTime = 0);

	void checkForHostileFaction(WowObjectLight _target);
	void checkForHostileFaction(WowObject _target);
	void checkForHostileFactionPart2(WowObject targetTarget);
	bool gotRelevantTargetFromList(std::vector<WowObject> _targetList);
	bool isCoequalUnit(float unitHealthPercent);
	bool multiTargetRota(unsigned int hPercent, std::string debuff, std::string spell, unsigned int distance, unsigned int maxTargets, bool mustTapped, bool downrank = false);
	std::vector<WowObject> multiDmgTargets;
	std::string downRankSpells(std::string spell, WowObject target, unsigned int  groupFactor, unsigned int hFactor);


protected:
	WowHelpers &wowHelperObj;
	InjectAsm &wowInject;

};


#include "stdafx.h"
#include "PlayerRotation.h"


PlayerRotation::PlayerRotation(WowHelpers &_wowHelperObj, InjectAsm &_wowInject)
	:wowHelperObj(_wowHelperObj),
	wowInject(_wowInject)
{
}


PlayerRotation::~PlayerRotation()
{
}

bool PlayerRotation::isCoequalUnit(float unitHealthPercent)
{
	return (
		(
		(wowHelperObj.localPlayer.target.healthPercent > unitHealthPercent)
		&&
		(wowHelperObj.localPlayer.target.level >= wowHelperObj.localPlayer.level - 6)
			)
		||
		wowHelperObj.localPlayer.target.type == (short)WowPointers::WoWObjectTypes::OT_PLAYER
		);

	//return (
	//	(((float)wowHelperObj.localPlayer.target.health / (float)wowHelperObj.localPlayer.target.maxHealth * 100.0f) > unitHealth || wowHelperObj.localPlayer.target.health > wowHelperObj.localPlayer.maxHealth * factor)
	//	&&
	//	(wowHelperObj.localPlayer.target.level >= wowHelperObj.localPlayer.level - 6 || wowHelperObj.localPlayer.target.health > wowHelperObj.localPlayer.maxHealth * factor)
	//	|| 
	//	wowHelperObj.localPlayer.target.type == (short)WowPointers::WoWObjectTypes::OT_PLAYER)
	//	;
}


void PlayerRotation::baseRotation(InjectAsm &wowInject)
{
	wowHelperObj.scanObj();
	WowObject &me = wowHelperObj.localPlayer;
	me.target = wowHelperObj.WowObjectToWowObjectLight(wowHelperObj.getTargetPlayer(me));

}




std::string PlayerRotation::downRankSpells(std::string spell, WowObject target, unsigned int groupFactor, unsigned int hPercent)
{
	float calcRank = 1;
	std::string calcSpell = spell;
	// rechnet den passenden Rank des Spells raus, falls das Target zu wenig Health hat
	std::map<std::string, unsigned int>::iterator it = wowHelperObj.allPlayerSpellsAndRank.find(spell);
	if (it != wowHelperObj.allPlayerSpellsAndRank.end() && it->second > 1 &&  (target.healthPercent < hPercent || target.level < wowHelperObj.localPlayer.level-6))
	{
		cout << dec << "h: " << target.healthPercent << " -> h*2" << target.healthPercent << "  -  hfact" << hPercent << endl;
		// nimmt den Factor aus Targethealth und hfactor (meistens playerhealth*0.2) und multipliziert ihn mit dem spellrank, damit wir genau den passenden Damage hinbekommen
		calcRank = std::fmin((float)(target.healthPercent / hPercent),1.0f);
		calcRank = calcRank * (((float)target.level / (float)wowHelperObj.localPlayer.level) + ((1.0f - ((float)target.level / (float)wowHelperObj.localPlayer.level))/2.0f));
		calcRank = round(calcRank * it->second);
		cout << "rank: " << calcRank << "   (" << std::fmin((float)(target.healthPercent / hPercent), 1.0f) << ")" << "   (" << (((float)target.level / (float)wowHelperObj.localPlayer.level) + ((1.0f - ((float)target.level / (float)wowHelperObj.localPlayer.level)) / 2.0f)) <<")"<< endl;
		calcSpell.append("(Rank ");
		calcSpell.append(to_string((int)calcRank));
		calcSpell.append(")");
		cout << "spell: " << calcSpell << endl;
	}
	return calcSpell;
}

bool PlayerRotation::multiTargetRota(unsigned int hPercent, std::string debuff, std::string spell, unsigned int distance, unsigned int maxTargets, bool mustTapped, bool downrank)
{


	// sammelt erstmal alle potentielle Targets ein, abhängig von tapped/nicht tapped
	if (mustTapped)
	{
		multiDmgTargets = wowHelperObj.getNearEnemiesMultiDamageTapped(hPercent, debuff, distance, maxTargets);
	}
	else
	{
		multiDmgTargets = wowHelperObj.getNearEnemiesMultiDamage(hPercent, debuff, distance, maxTargets);
	}
	if (multiDmgTargets.size() > 0)
	{
		// groupfactor:  Elite-Mob + 5 groupmembers = 5, kein elitemob oder kein groupmember = 1
		unsigned int groupFactor = (wowHelperObj.isEliteMob(multiDmgTargets[0]) * wowHelperObj.getNearFriendlyPlayers(30,5).size()) + 1;
		cout << "Elite: " << wowHelperObj.isEliteMob(multiDmgTargets[0]) << endl;
		cout << "Party#: " << wowHelperObj.getNearFriendlyPlayers(30, 5).size() << endl;
		cout << "groupFactor: " << groupFactor << endl;
		if (downrank)
		{
			spell = downRankSpells(spell, multiDmgTargets[0], groupFactor, hPercent);
		}

		// casted den Spell aufs erste target aus der gesammelten Liste
		castSpell(spell, multiDmgTargets[0]);
		cout << "castSpell("<< spell <<", " << hex << multiDmgTargets[0].name << ")" << endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		// Wenn LOS-Error kommt, dann wird das Target für 5sec auf eine Banliste gesetzt
		if (wowHelperObj.getErrMsg() == "Target not in line of sight")
		{
			wowHelperObj.multiDamageBanlist.insert({ { multiDmgTargets[0].guid },{ WowHelpers::getTime() } });
			// Um die Errormessage im Speicher mit etwas anderen zu füllen, kommt dieser irrsinnige Selfcast
			castSpell(spell, wowHelperObj.localPlayer);
		}
		return true;
	}
	return false;
}



void PlayerRotation::castSpellLocation(const std::string &spellName, const unsigned long &foreCastTime)
{
	WowVector3d foreCastLoc = wowHelperObj.target.calculateVector3dForecast(foreCastTime, wowHelperObj.myLog, "target");
	wowInject.setTarget(wowHelperObj.localPlayer.guid); // Nicht relevant für groundSpells, das hat nichts zu tun mit forecasttarget
	wowInject.setSpell(spellName);
	wowInject.setSpellLocation(foreCastLoc);
	wowInject.setSpellPlacementAction(0x3);
	wowInject.setDoAction(0x1);

}
void PlayerRotation::useItemLocation(const DWORD &objPtr, const unsigned long &travelTimeInMsPerM, unsigned long castTimeInMs)
{

	if (wowHelperObj.localPlayer.targetGuid == 0)
	{
		// Wenn kein Target ausgewählt, dann errechne die mögliche Location des Targets was innerhalb der letzten 3 sec noch Target war
		// Beachte: die LocLog-Tabelle wird nicht mehr befüllt. Das ist auch gut, denn wir berechnen den target.vector3d anhand vo lastVectorCheck und den letzten Tabellen-Daten.
		// Ein neuer Eintrag in die LocLog-Tabelle mit der Fake-Vorrausberechnung hätte unpräzise Vectordaten  für weitere Berechnungen eines "None"-Targets
		unsigned long predictNoneTargetLocTimeDelta = WowHelpers::getTime() - wowHelperObj.target.predictLocObj.targetLastVectorCheck;
		wowHelperObj.myLog.ss << "predictNoneTargetLocTimeDelta: " << dec << predictNoneTargetLocTimeDelta << "\tgetTime(): " << WowHelpers::getTime() << "\ttargetLastVectorCheck: " << wowHelperObj.target.predictLocObj.targetLastVectorCheck << endl;
		if (predictNoneTargetLocTimeDelta < 3000)
		{
			wowHelperObj.myLog.ss << "ehemals richtiges Target" << endl;
			wowHelperObj.target.vector3d = wowHelperObj.localPlayer.calculateVector3dForecast(predictNoneTargetLocTimeDelta, wowHelperObj.myLog, "target");

		}
		// Wenn wir 3s kein Target hatten, dann wird der eigene Char als Target genommen.
		else
		{
			wowHelperObj.myLog.ss << "Kein Target? localPlayer.vector3d!" << endl;
			wowHelperObj.target.vector3d = wowHelperObj.localPlayer.vector3d;
			wowHelperObj.target.predictLocObj = wowHelperObj.localPlayer.predictLocObj;
		}
	}
	// Wenn selbst als Target gewählt, dann werden die vectordaten angeglichen
	else if (wowHelperObj.localPlayer.targetGuid == wowHelperObj.localPlayer.guid)
	{
		wowHelperObj.target.vector3d = wowHelperObj.localPlayer.vector3d;
	}
	WowVector3d predictTargetLoc;
	double distanceLaunchToImpact;
	castTimeInMs += 0.001;
	WowVector3d predictLaunchTimeVectorPlayer = wowHelperObj.localPlayer.calculateVector3dForecast(castTimeInMs, wowHelperObj.myLog, "predictLaunchTimeVectorPlayer");
	WowVector3d predictLaunchTimeVectorTarget = wowHelperObj.target.calculateVector3dForecast(castTimeInMs, wowHelperObj.myLog, "predictLaunchTimeVectorTarget");
	//Logging::sout << "x1:" << wowHelperObj.localPlayer.vector3d.x << "  x2:" << wowHelperObj.target.vector3d.x << "\ty1:" << wowHelperObj.localPlayer.vector3d.y << "  y2:" << wowHelperObj.target.vector3d.y << endl;
	
	// Das predictTargetLoc ist die Location, welche auf Basis der Location zum Zeitpunkt des Launches und den locPerMsec des original Targets errechnet wird.
	// Letztendlich ist es eine Summe  von Vorrausberechnung der Loc nach dem Cast und Vorrausberechnung der Loc nach dem Einschlag
	predictTargetLoc = WowObject::standaloneCalculateVector3dForecast(2000, wowHelperObj.myLog, predictLaunchTimeVectorTarget, wowHelperObj.target.predictLocObj,"predictTargetLoc");

	//predictTargetLoc = wowHelperObj.target.calculateVector3dForecast(10, "target"); // zu Begin erstmal irgendein Richtwert an GesamtTravelzeit angeben.
	
	//Logging::sout <<"? ImpactLoc: " << predictTargetLoc.y << "\t" << predictTargetLoc.x << "\t" << predictTargetLoc.z << endl;
	// Jetzt über mehrere Iterationen immer näher der Vorrausberechnung kommen.
	for (int i = 0; i < 15; i++)
	{
		distanceLaunchToImpact = predictLaunchTimeVectorPlayer.distance(predictTargetLoc);
		//wowHelperObj.myLog.ss << i << " ------------------" << endl;
		//wowHelperObj.myLog.ss << "dist launch to impact:\t" << distanceLaunchToImpact << endl;
		//wowHelperObj.myLog.ss << "dist launch to player now:\t" << wowHelperObj.localPlayer.vector3d.distance(predictLaunchTimeVectorPlayer) << endl;
		//predictTargetLoc = wowHelperObj.target.calculateVector3dForecast(distanceLaunchToImpact * (double)travelTimeInMsPerM, "target");
		predictTargetLoc = WowObject::standaloneCalculateVector3dForecast(distanceLaunchToImpact * (double)travelTimeInMsPerM, wowHelperObj.myLog, predictLaunchTimeVectorTarget, wowHelperObj.target.predictLocObj, "predictTargetLoc_loop");
		//wowHelperObj.myLog.ss << " ImpactLoc: x:" << predictTargetLoc.x << "\ty:" << predictTargetLoc.y << "\ttravelTime: " << (distanceLaunchToImpact * (double)travelTimeInMsPerM) << "ms"<<  endl;

	}





	wowInject.setTarget(wowHelperObj.localPlayer.guid); // Nicht relevant für groundSpells, das hat nichts zu tun mit forecasttarget
	wowInject.setUseItemPtr(objPtr);
	wowInject.setSpellLocation(predictTargetLoc);
	wowInject.setSpellPlacementAction(0x3);
	wowInject.setDoAction(0x2);

	wowHelperObj.myLog.cout(); 
}

void PlayerRotation::useItem(const DWORD &objPtr)
{
	wowInject.setUseItemPtr(objPtr);
	wowInject.setSpellPlacementAction(0x0);
	wowInject.setDoAction(0x2);

}
void PlayerRotation::petAction(byte action)
{
	wowInject.setPetAction(action - 1);
	wowInject.setDoAction(0x5);
}

void PlayerRotation::clickButton(byte btn)
{
	wowInject.setActionButton(btn);
	wowInject.setDoAction(0x4);
}


void PlayerRotation::castSpell(const std::string &spellName, const WowObject &target, const unsigned long &blockTime)
{
	checkForHostileFaction(target);
	wowInject.setTarget(target.guid); 
	wowInject.setSpell(spellName);
	wowInject.setSpellPlacementAction(0x0);
	wowInject.setDoAction(0x1);
}
void PlayerRotation::castSpell(const std::string &spellName, const WowObjectLight &target, const unsigned long &blockTime)
{
	checkForHostileFaction(wowHelperObj.WowObjectLightToWowObject(target));
	wowInject.setTarget(target.guid);
	wowInject.setSpell(spellName);
	wowInject.setSpellPlacementAction(0x0);
	wowInject.setDoAction(0x1);
}
void PlayerRotation::checkForHostileFaction(WowObjectLight _target)
{
	WowObject targetTarget;
	//Wenn kein target vorgegeben wird, dann wird das target von player genommen
	if (_target.guid == 0)
	{
		targetTarget = wowHelperObj.getTargetNpc(wowHelperObj.WowObjectLightToWowObject(wowHelperObj.localPlayer.target));
	}
	else
	{
		targetTarget = wowHelperObj.getTargetNpc(wowHelperObj.WowObjectLightToWowObject(_target));
	}
	checkForHostileFactionPart2(targetTarget);
}
void PlayerRotation::checkForHostileFaction(WowObject _target)
{
	WowObject targetTarget;
	//Wenn kein target vorgegeben wird, dann wird das target von player genommen
	if (_target.guid == 0)
	{
		targetTarget = wowHelperObj.getTargetNpc(wowHelperObj.WowObjectLightToWowObject(wowHelperObj.localPlayer.target));
	}
	else
	{
		targetTarget = wowHelperObj.getTargetNpc(_target);
	}
	checkForHostileFactionPart2(targetTarget);
}

void PlayerRotation::checkForHostileFactionPart2(WowObject targetTarget)
{
	//cout << "Factioncheck: target.unitflags: " << (wowHelperObj.localPlayer.target.unitFlags << 12 >> 31) << "   targetTarget.guid: " << hex << targetTarget.guid << "   localPlayer.guid: " << wowHelperObj.localPlayer.guid << "   targetTarget.health: " << dec << targetTarget.health << "  hostile: " << wowHelperObj.isHostile(wowHelperObj.localPlayer.target) << endl;
	if (wowHelperObj.localPlayer.target.unitFlags << 12 >> 31 == 1 && targetTarget.guid == wowHelperObj.localPlayer.guid)
	{
		
		if (!wowHelperObj.isHostile(wowHelperObj.localPlayer.target))
		{
			cout << "Marked [" << wowHelperObj.localPlayer.target.factionTemplate << "] as hostile (" << wowHelperObj.localPlayer.factionTemplate << ")" << endl;
			if (wowHelperObj.playerAllianceFaction[wowHelperObj.localPlayer.factionTemplate])
			{
				wowHelperObj.allianceHostileFaction[wowHelperObj.localPlayer.target.factionTemplate] = true;
			}
			else if (wowHelperObj.playerHordeFaction[wowHelperObj.localPlayer.factionTemplate])
			{
				wowHelperObj.hordeHostileFaction[wowHelperObj.localPlayer.target.factionTemplate] = true;
			}
			cout << "save config" << endl;	
			wowHelperObj.configSave();				

		}
	}
}

bool PlayerRotation::gotRelevantTargetFromList(std::vector<WowObject> _targetList)
{
	return true;
}

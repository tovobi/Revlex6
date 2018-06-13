#include "stdafx.h"
#include "WowObjectLight.h"




WowObjectLight::WowObjectLight()
	:vector3d(0.0, 0.0, 0.0)
{

}
void WowObjectLight::Clear()
{
	// general properties
	guid = 0;
	xPos = 0;
	yPos = 0;
	zPos = 0;
	wowClass = "None";
	distance = 0;
	factionTemplate = 0;
	factionOffset = 0;
	vector3d.set(0.0, 0.0, 0.0);
	rotation = 0;
	objBaseAddress = 0;
	unitFieldsAddress = 0;
	type = 0;
	dodged = 0;
	guidOfAutoAttackTarget;
	autoShoot = 0;
	playerClass = "None";
	isMoving = false;
	isFighting = false;
	isInCombat = false;
	isFleeing = false;
	isStunned = false;
	cantMove = false;
	isConfused = false;
	hasBreakableCc = false;
	//List<Auras> buffList = null;
	//List<Auras> debuffList = null;
	channelSpell = 0;
	castSpell = 0;
	playerIsFacingTo = 0;
	movementFlags = 0;
	unitFlags = 0;
	npcFlags = 0;
	dynamicFlags = 0;
	name = "";
	isHostile = false; //we havent found a way yet to determine the hostile, thats just for a quick and dirty way for the calculation of # of targets
	tempBuffStacks = 0;
	tempNextSpell = "";
	tempTargetPrio = 0;

	// more specialised properties (player or mob)
	health = 0;
	maxHealth = 0;
	healthPercent = 100;
	mana = 0; // mana, rage and energy will all fall under energy.
	maxMana = 0;
	manaPercent = 100;
	rage = 0; // mana, rage and energy will all fall under energy.
	energy = 0;
	level = 0;
	buffList.clear();
	debuffList.clear();
	stacks = 0;
	itemOwner = 0;
	itemId = 0;

	gameObjectType = 0;
	//WowObject next = null;	
}
void WowObjectLight::DisposeObject()
{
	delete this;
}

void WowObjectLight::collectLocData()
{
	//WowVector3d sLoc(0, 0, 0);
	//unsigned long sTime = 0.0;
	//WowVector3d aLoc(0, 0, 0);
	//unsigned long aTime = 0.0;
	//WowVector3d nextLoc(0, 0, 0); // der wahrscheinlich nächste berechnte Punkt vom target
	//WowVector3d locPerMsec(0, 0, 0); // die meter pro millisec vom target
	predictLocObj.sTime = 0.0;
	predictLocObj.aTime = 0.0;
	predictLocObj.sLoc.set(0, 0, 0);
	predictLocObj.aLoc.set(0, 0, 0);


	// Bedenke, bei "map" niemals eine custom class als "key"
	lastVector3d.insert({ { (unsigned long)GetTickCount64() },{ vector3d } });
	std::map<unsigned long, WowVector3d>::iterator start = lastVector3d.begin();
	std::map<unsigned long, WowVector3d>::iterator end = lastVector3d.end();
	//cout << "insert" << "currentTime: " << currentTime << "   xyz: " << unit.vector3d.x << ", " << unit.vector3d.y << ", " << unit.vector3d.z << endl;

	if (std::distance(start, end) > predictLocObj.logLength)
	{
		lastVector3d.erase(start);
	}
	start = lastVector3d.begin();
	end = lastVector3d.end();

	for (auto it = start; it != end; ++it)
	{
		// index des aktuellen it
		auto currentIt = std::distance(start, it) + 1;
		// Ausnahmeregelung für den ersten Durchgang bei der durchnschnittlichen Zeitmessung
		if (it != start)
		{
			predictLocObj.sTime += it->first - predictLocObj.targetLastVectorCheck;
			predictLocObj.aTime = predictLocObj.sTime / currentIt;
		}
		// Summe aller Elemente pro Koordinate
		predictLocObj.sLoc.x += it->second.x;
		predictLocObj.sLoc.y += it->second.y;
		predictLocObj.sLoc.z += it->second.z;

		predictLocObj.targetLastVectorCheck = it->first;
		// durchschnitt aller bisher durchgelaufenen werte
		predictLocObj.aLoc.x = predictLocObj.sLoc.x / currentIt;
		predictLocObj.aLoc.y = predictLocObj.sLoc.y / currentIt;
		predictLocObj.aLoc.z = predictLocObj.sLoc.z / currentIt;

		// letzte location beim Durchlauf durch das objekt <map>lastVector3d
		predictLocObj.lastLoc.x = it->second.x;
		predictLocObj.lastLoc.y = it->second.y;
		predictLocObj.lastLoc.z = it->second.z;

		//std::cout << "###"<< dec << distance(start, it) << ":   time: " << it->first << "   xyz: " << it->second.x << ", " << it->second.y << ", " << it->second.z << endl;

	}
	// berechnung von meter pro millisec vom target
	predictLocObj.locPerMsec.x = (predictLocObj.lastLoc.x - start->second.x) / predictLocObj.logLength / predictLocObj.aTime;
	predictLocObj.locPerMsec.y = (predictLocObj.lastLoc.y - start->second.y) / predictLocObj.logLength / predictLocObj.aTime;
	predictLocObj.locPerMsec.z = (predictLocObj.lastLoc.z - start->second.z) / predictLocObj.logLength / predictLocObj.aTime;
	//std::cout << std::endl;
	//std::cout << dec << "atime: " << foreCastObj.aTime << "   " << foreCastObj.aLoc.x << ", " << foreCastObj.aLoc.y << ", " << foreCastObj.aLoc.z << endl;

}

WowVector3d WowObjectLight::calculateVector3dForecast(const unsigned long &foreCastTimeinMs)
{
	//foreCastObj.forCastTime = foreCastTimeinMs;
	// der wahrscheinlich nächste berechnte Punkt vom target
	predictLocObj.nextLoc.x = predictLocObj.lastLoc.x + (predictLocObj.locPerMsec.x * foreCastTimeinMs);
	predictLocObj.nextLoc.y = predictLocObj.lastLoc.y + (predictLocObj.locPerMsec.y * foreCastTimeinMs);
	predictLocObj.nextLoc.z = predictLocObj.lastLoc.z + (predictLocObj.locPerMsec.z * foreCastTimeinMs);


	////std::cout << std::dec << "dist: " << target.distance << "\t forecast: " << predictLocObj.forCastTime << " ms" << std::endl;
	//std::cout << std::dec << "fcloc: " << predictLocObj.nextLoc.x << " : " << predictLocObj.nextLoc.y << "\t current.loc: " << vector3d.x << " : " << vector3d.y << std::endl;

	//std::cout << std::dec << "perMS: " << predictLocObj.locPerMsec.x << ", " << predictLocObj.locPerMsec.y << ", " << predictLocObj.locPerMsec.z << std::endl;

	return predictLocObj.nextLoc;
}

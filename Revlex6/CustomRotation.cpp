#include "stdafx.h"
#include "CustomRotation.h"
#include <iostream>
#include <bitset>

using namespace std;


CustomRotation::CustomRotation(WowHelpers &_wowHelperObj, InjectAsm &_wowInject) :PlayerRotation(_wowHelperObj,_wowInject)
{
}


CustomRotation::~CustomRotation()
{
}


bool CustomRotation::autoRotation(InjectAsm &wowInject, std::vector<RvxKeys> &rvxKey)
{
	WowObject &me = wowHelperObj.localPlayer;
	WowObject &target = wowHelperObj.getTargetPlayer(me);

	// Full automated Routine
	if (fullAutoRoutine)
	{
		//petAction(10);
		if (lastAutoRoutine + blockAutoRoutine < wowHelperObj.getTime())
		{
			lastAutoRoutine = wowHelperObj.getTime();
			// Place for Actions:

			//cout << "aware: " << (int)wowHelperObj.petAwareness << "\tfollow: " << (int)wowHelperObj.petFollow << endl;

			// Taunt/Torment of Pet
			wowHelperObj.unbanMultiDmgObj();
			std::vector<WowObject> multiTauntTargets = wowHelperObj.getNearEnemiesPetTaunt(10, "", me.guid, 20, 8);
			for (WowObject x : multiTauntTargets)
			{
				cout << "x: dist: " << x.distance << "   " << x.name << endl;
			}
			//if (multiTauntTargets.size())
			//{
			//	cout << " targets: " << multiTauntTargets.size() <<  "   cd: " << wowHelperObj.getSpellCooldown(wowHelperObj.getSpellNameFromSpellId(11775)) << "   inCombat: " << multiTauntTargets[0].isInCombat << " " << me.isInCombat << endl;
			//}
			if (multiTauntTargets.size() && multiTauntTargets[0].isInCombat && me.isInCombat && wowHelperObj.petAwareness != 0)
			{
				
				//if (lastAutoSpell == 1)
				//{
				//	cout << "petAction(8)" << endl;
				//	petAction(8);
				//	lastAutoSpell = 0; // reset autospell
				//}
				//else
				////lastAutoSpell ist eine Art Queue
				//if (lastAutoSpell == 0)
				//{
				//	cout << "cast torment" << endl;
				//	castSpell("Torment", multiTauntTargets[0]);
				//	lastAutoSpell = 1;
				//}

				//if (wowHelperObj.petAwareness == 1)
				//{
				//	cout << "petAction(8)" << endl;
				//	petAction(8);
				//	
				//}
				cout << "cast torment " << multiTauntTargets[0].distance << endl;
				castSpell("Torment", multiTauntTargets[0]);
				// rennt nur zum multiTauntTarget und schlägt es mit spell 1 wenn es auch target vom player ist. sonst würde er imme rnur das Target vom Player schlagen wollen.
				if (lastPetAttack + blockPetAttack < wowHelperObj.getTime() && multiTauntTargets[0].guid == me.targetGuid)
				{
					cout << "petAction(1) Attack!" << endl;
					petAction(1);
					lastPetAttack = wowHelperObj.getTime();
				}
				
			}
			//else
			//{
			//	if (wowHelperObj.petAwareness == 2)
			//	{
			//		cout << "!!!!!!!!!!petAction(9)" << endl;
			//		petAction(9);

			//	}
			//}


			//else
			////Attack of Pet at the beginning of the fight
			//if (((me.isInCombat && target.isInCombat) || me.castSpell > 0) && target.distance < 20 && target.guid != me.guid )
			//{
			//	cout << "click Button petAttack" << endl;
			//	petAction(1);
			//}


			return true;

		}
	}
	return false;
}

bool CustomRotation::mainRotation(InjectAsm &wowInject, std::vector<RvxKeys> &rvxKey)
{
	WowObject &me = wowHelperObj.localPlayer;
	WowObject &target = wowHelperObj.getTargetPlayer(me);



	// toggle full auto routines

	if (rvxKey[0x43].getKeyState() > 1 && rvxKey[0xA0].getKeyState() > 0 && rvxKey[0xA2].getKeyState() > 0)
	{
		cout << "<C + Shift + Ctrl>" << endl;
		if (fullAutoRoutine)
		{
			fullAutoRoutine = false;
			cout << "auto routine OFF" << endl;
		}
		else
		{
			fullAutoRoutine = true;
			cout << "auto routine ON" << endl;
		}
	}
	//// Single DMG rota
	//if (rvxKey[0x45].getKeyState() > 1 && me.manaPercent > 10)
	//{
	//	castSpell("Shadow Bolt", me.target);
	//	std::string errorMsg = wowHelperObj.wowMem.MemReadString(WowPointers::StaticAddresses::LASTSCREENMESSAGE);
	//	cout << errorMsg << endl;
	//}

	// -F- Single DMG rota
	if (rvxKey[0x46].getKeyState() > 1)
	{

		cout << "<F>" << endl;	
		//---Priest
		if (me.playerClass == "Priest")
		{
			cout << "me.mp: " << dec << me.manaPercent << "\ttar.hp: " << target.healthPercent << "\tcombat: " << me.isInCombat << "\tpwd: " << wowHelperObj.unitHasDebuff2(target, "Shadow Word: Pain") << endl;
			if (me.manaPercent > 20 && (target.healthPercent > 80 || (!me.isInCombat && target.healthPercent > 60)))
			{
				//if (me.castSpell != wowHelperObj.getSpellIdFromSpellName("Smite"))
				//{
				//	castSpell("Smite", target);
				//}
				castSpell("Mind Blast", target);
				return true;
			}
			if (!wowHelperObj.unitHasDebuff2(target, "Shadow Word: Pain") && me.manaPercent > 10 && target.healthPercent > 40)
			{
				castSpell("Shadow Word: Pain", target);
				return true;
			}
			if (me.autoShoot <= 1)
			{
				castSpell("Shoot", target);
			}
			return true;
		}
		if (me.playerClass == "Warlock")
		{
			if (me.autoShoot <= 1)
			{
				castSpell("Shoot", target);
			}
			return true;
		}
	}
	// -5- Self Buff
	if (rvxKey[0x35].getKeyState() > 1)
	{
		cout << "<5>" << endl;
		if (me.playerClass == "Priest")
		{
			if (wowHelperObj.unitHasBuff2(me, "Power Word: Fortitude") && me.guid != target.guid && me.targetGuid != 0 && !wowHelperObj.unitHasBuff2(target, "Power Word: Fortitude") && me.manaPercent > 30)
			{
				castSpell("Power Word: Fortitude", target);
				return true;
			}
			if (!wowHelperObj.unitHasBuff2(me, "Power Word: Fortitude") && me.manaPercent > 50)
			{
				castSpell("Power Word: Fortitude", me);
				return true;
			}
		}
		if (me.playerClass == "Warlock")
		{
			petAction(10);
		}
	}

	// -6- test
	if (rvxKey[0x36].getKeyState() > 1)
	{
		cout << "<6>" << endl;
		if (me.playerClass == "Priest")
		{

		}
		if (me.playerClass == "Warlock")
		{
			useItemLocation(wowHelperObj.getItemObj(4390).objBaseAddress, Constants::times::GRENADETRAVEL_MS_PER_M, Constants::times::GRENADECAST);
		}
	}

	// -7- test
	if (rvxKey[0x37].getKeyState() > 1)
	{
		cout << "<7>" << endl;
		if (me.playerClass == "Priest")
		{

		}
		if (me.playerClass == "Warlock")
		{
			castSpellLocation("Blizzard", 1000);
		}
	}

	// -E- Self Heal
	if (rvxKey[0x45].getKeyState() > 1)
	{
		cout << "<E> " << me.healthPercent << endl;
		if (me.playerClass == "Priest")
		{
			if (!wowHelperObj.unitHasBuff2(me, "Power Word: Shield") && me.manaPercent > 30 && me.healthPercent < 20)
			{
				castSpell("Power Word: Shield", me);
				return true;
			}
			if (!wowHelperObj.unitHasBuff2(me, "Renew") && me.healthPercent < 80)
			{
				castSpell("Renew", me);
				return true;
			}
			//if (wowHelperObj.hasFriendlyTarget(me, target) && target.healthPercent < 60)
			//{
			//	castSpell("Lesser Heal", target);
			//	return true;
			//}
			if (wowHelperObj.unitHasBuff2(me, "Renew") && me.healthPercent < 97 && !me.isMoving)
			{
				castSpell("Lesser Heal", me);
				return true;
			}
		}
		if (me.playerClass == "Warlock")
		{
			clickButton(61);
		}
	}


	//// -B- Single DMG rota
	//if (rvxKey[0x42].getKeyState() > 1)
	//{
	//	cout << "tar: " << hex << me.target.guid << endl;
	//	if (wowHelperObj.unitHasBuff2(me, "Shadow Trance") && me.manaPercent > 10)
	//	{
	//		castSpell("Shadow Bolt", me.target);
	//		return true;
	//	}
	//	if (me.target.healthPercent < 10 && wowHelperObj.getSpellCooldown("Shadowburn") < cdOffset)
	//	{
	//		castSpell("Shadowburn", me.target);
	//	}
	//	if (isCoequalUnit(50) && !wowHelperObj.unitHasDebuff2(me.target, "Corruption") && me.manaPercent > 10)
	//	{
	//		castSpell("Corruption", me.target);
	//		return true;
	//	}
	//	if (isCoequalUnit(20) && !wowHelperObj.unitHasDebuff2(me.target, "Immolate") && !me.isMoving && me.manaPercent > 10)
	//	{
	//		castSpell("Immolate", me.target);
	//		return true;
	//	}
	//	if (isCoequalUnit(80) && !wowHelperObj.unitHasDebuff2(me.target, "Curse of Agony") && me.manaPercent > 10)
	//	{
	//		castSpell("Curse of Agony", me.target);
	//		return true;
	//	}
	//	if (isCoequalUnit(95) && !wowHelperObj.unitHasDebuff2(me.target, "Siphon Life") && me.manaPercent > 40 && me.healthPercent < 75)
	//	{
	//		castSpell("Siphon Life", me.target);
	//		return true;
	//	}
	//	if (isCoequalUnit(50) && !wowHelperObj.unitHasDebuff2(me.target, "Drain Life") && me.healthPercent < 75 && me.manaPercent > 10)
	//	{
	//		castSpell("Drain Life", me.target);
	//		return true;
	//	}
	//	if (me.autoShoot <= 1)
	//	{
	//		castSpell("Shoot", me.target);
	//	}
	//	return true;
	//}

	//// test rota
	//if (rvxKey[0x46].getKeyState() > 1)
	//{
	//	//wowHelperObj.myLog.ss << "******** <F> ********" << endl;
	//	cout << "******** <F> ********" << endl;
	//	//checkForHostileFaction(me.target);
	//	//cout << "id: " << hex << me.target.guid << "   dynflags: " << me.target.dynamicFlags << "  " << me.target.npcFlags << "  unitflags: " << me.target.unitFlags << "   fac: " << dec << me.target.factionTemplate << "   h: " << me.target.health << endl;
	//	//castSpell("Iron Grenade", me.target);
	//	//wowInject.setSpellLocation(wowHelperObj.calculateVector3dForecast(5000));
	//	
	//	useItemLocation(wowHelperObj.getItemObj(4390).objBaseAddress, Constants::times::GRENADETRAVEL_MS_PER_M, Constants::times::GRENADECAST);

	//}

	//// test rota2
	//if (rvxKey[0x47].getKeyState() > 1)
	//{
	//	//useItem(wowHelperObj.getItemObj(9312).objBaseAddress);
	//	castSpellLocation("Blizzard", 1000);

	//}

	//// multi DMG rota
	//if (rvxKey[0x43].getKeyState() > 1)
	//{
	//	wowHelperObj.unbanMultiDmgObj();
	//	multiDmgTargets = wowHelperObj.getNearEnemiesMultiDamage(20, "", 36, 8);
	//	std::vector<WowObject> multiDmgTargetsTapped = wowHelperObj.getNearEnemiesMultiDamageTapped(20, "", 36, 8);
	//	cout << "multiDmgTargets: " << dec << multiDmgTargets.size() << " " << endl;
	//	cout << "multiDmgTargetsTapped: " << dec << multiDmgTargetsTapped.size() << " " << endl;
	//	if (wowHelperObj.unitHasBuff2(me,"Shadow Trance") && multiDmgTargets.size() && me.manaPercent > 10)
	//	{
	//		castSpell("Shadow Bolt", multiDmgTargets[0]);
	//		return true;
	//	}

	//	if (multiTargetRota(20, "Corruption", "Corruption", 36, 8, false, true) && me.manaPercent > 10)
	//	{
	//		return true;
	//	}

	//	if (multiTargetRota(20, "Curse of Agony", "Curse of Agony", 36, 8, false, true) && me.manaPercent > 10)
	//	{
	//		return true;
	//	}

	//	if (me.autoShoot <= 1 && multiDmgTargets.size())
	//	{
	//		castSpell("Shoot", multiDmgTargets[0]);
	//	}
	//	return true;
	//}
	return false;
}

//////////////AAALLLLT???????
//////////////multiDmgTargets = wowHelperObj.getNearEnemiesMultiDamageTapped(me.maxHealth*0.2f, "Corruption", 36, 8);
//////////////if (multiDmgTargets.size() > 0)
//////////////{
//////////////	cout << "multiDmgTargets.size(): " << multiDmgTargets.size() << "\t multiDamageBanlist.size(): " << wowHelperObj.multiDamageBanlist.size() << endl;
//////////////	cout << "corruption multiDmgTargets[0]" << ": " << dec << "   debuff: " << wowHelperObj.unitHasDebuff2(multiDmgTargets[0], "Corruption") << "    type: " << multiDmgTargets[0].type << "    h: " << multiDmgTargets[0].health << "    me.h: " << me.maxHealth*0.2 << "  id: " << hex << multiDmgTargets[0].guid << endl;
//////////////
//////////////	castSpell("Corruption", multiDmgTargets[0]);
//////////////	std::this_thread::sleep_for(std::chrono::milliseconds(10));
//////////////	cout << "Orig: " << wowHelperObj.getErrMsg() << endl;
//////////////	if (wowHelperObj.getErrMsg() == "Target not in line of sight")
//////////////	{
//////////////		wowHelperObj.multiDamageBanlist.insert({ { multiDmgTargets[0].guid },{ WowHelpers::getTime() } });
//////////////		castSpell("Corruption", me);
//////////////		cout << "Onme: " << wowHelperObj.getErrMsg() << endl;
//////////////	}
//////////////	return true;
//////////////}
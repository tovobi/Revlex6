#include "stdafx.h"
#include "WowHelpers.h"
#include "MemMagic.h"
#include <typeinfo>
#include <math.h>
#include <vector>




#ifndef M_PI 
#define M_PI    3.14159265358979323846f 
#endif

#pragma warning(disable:4996) //disable errors for depracted functions

using namespace std;
using namespace cpplinq;





WowHelpers::WowHelpers(LPCWSTR windowName) : wowMem(windowName)
{

}


WowHelpers::~WowHelpers()
{
}



//string -> wstring //why it gives random values on console-app?
std::wstring WowHelpers::s2ws(const std::string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}
void WowHelpers::buildPackedGuid(unsigned __int64 guid, std::vector<BYTE> &result)
{
	result.clear();
	result.reserve(sizeof(unsigned __int64));

	if (!guid)
		return;

	result.push_back(0);

	for (int i = 0; guid != 0; ++i)
	{
		if ((guid & 0xFF) != 0)
		{
			result[0] |= (BYTE)(1 << i);
			result.push_back((BYTE)(guid & 0xFF));
		}

		guid >>= 8;
	}
}
void WowHelpers::splitGuid(const unsigned __int64 &guid, unsigned int &guid1, unsigned int &guid2)
{
	guid1 = (unsigned int)guid;
	guid2 = (unsigned int)(guid >> 32);
}

std::vector<byte> WowHelpers::ToBytePattern(const DWORD &a)
{
	byte *tempByteArray = (byte*)&a; //converts the int/long-value to a array of bytes
	std::vector<byte> finalVector(tempByteArray, tempByteArray + sizeof tempByteArray / sizeof tempByteArray[0]); // converts the array of bytes into a vector
	finalVector.erase(std::remove(finalVector.begin(), finalVector.end(), 0), finalVector.end()); // deletes all "0" from vector
	finalVector.shrink_to_fit();
	return finalVector;
}
std::vector<byte> WowHelpers::ToBytePattern(const unsigned long long &a)
{
	byte *tempByteArray = (byte*)&a; //converts the int/long-value to a array of bytes
	std::vector<byte> finalVector(tempByteArray, tempByteArray + sizeof tempByteArray / sizeof tempByteArray[0]); // converts the array of bytes into a vector
	//finalVector.erase(std::remove(finalVector.begin(), finalVector.end(), 0), finalVector.end()); // deletes all "0" from vector
	//finalVector.shrink_to_fit();
	return finalVector;
}
std::vector<byte> WowHelpers::ToBytePattern(const unsigned int &a)
{
	byte *tempByteArray = (byte*)&a; //converts the int/long-value to a array of bytes
	std::vector<byte> finalVector(tempByteArray, tempByteArray + sizeof tempByteArray / sizeof tempByteArray[0]); // converts the array of bytes into a vector
	//finalVector.erase(std::remove(finalVector.begin(), finalVector.end(), 0), finalVector.end()); // deletes all "0" from vector
	//finalVector.shrink_to_fit();
	return finalVector;
}

std::string WowHelpers::decToHexStr(const unsigned long long &s1)
{
	std::stringstream ss;
	ss << std::hex << s1;
	return ss.str();
}

bool WowHelpers::GetStaticClientConnection()
{
	base = wowMem.GetBaseAddress();
	staticClientConnection = base + WowPointers::ObjectManager::CURMGRPOINTER;
	return (staticClientConnection != 0) ? true : false;
}
bool WowHelpers::GetBasicWowData()
{
	try
	{
		clientConnection = wowMem.MemReadUInt((staticClientConnection));
		firstObject = wowMem.MemReadUInt((clientConnection + firstObjectOffset));
		localPlayer.guid = wowMem.MemReadULong(base + WowPointers::StaticAddresses::LOCALPLAYERGUID);
		textCaretActive = wowMem.MemReadByte(WowPointers::StaticAddresses::TEXTCARETACTIVE);
		petFollow = wowMem.MemReadByte(WowPointers::StaticAddresses::PETFOLLOW);
		petAwareness = wowMem.MemReadByte(WowPointers::StaticAddresses::PETAWARENESS);
		// //lastPlayerGuidUpdate = GetTime();
		//ScanObj();
	}
	catch (int e)
	{
		std::cout << "GetBasicWowData: " << e << std::endl;
		return false;
	}

	// if the local guid is zero it means that something failed.
	//std::cout << std::endl;
	//std::cout << "clientConnection: " << std::hex << clientConnection << std::endl;
	//std::cout << "firstObject: " << std::hex << firstObject << std::endl;
	//std::cout << "localPlayer.guid: " << std::dec << localPlayer.guid << std::endl;
	return (localPlayer.guid != 0) ? true : false;
}

std::string WowHelpers::mobNameFromBaseAddr(const unsigned int &baseAddr)
{
	unsigned int objectBase = baseAddr;
	std::string tmp = "";
	try
	{

		//tmp = ReadASCIIString((wowMem.ReadUInt((wowMem.ReadUInt((ObjectBase + (uint)Pointers.UnitName.UnitName1)) + (uint)Pointers.UnitName.UnitName2))));
		tmp = wowMem.MemReadString((wowMem.MemReadUInt((wowMem.MemReadUInt((objectBase + (unsigned int)WowPointers::UnitName::UNITNAME1)) + (unsigned int)WowPointers::UnitName::UNITNAME2))));
		return tmp;
	}
	catch (int e)
	{
		std::cout << "MobNameFromBaseAddr: " << e << std::endl;
		return "unnamed";
	}
}
std::string WowHelpers::itemNameFromBaseAddr(const unsigned int &baseAddr)
{
	unsigned int objectBase = baseAddr;
	std::string tmp = "";
	try
	{

		//tmp = ReadASCIIString((wowMem.ReadUInt((wowMem.ReadUInt((ObjectBase + (uint)Pointers.UnitName.UnitName1)) + (uint)Pointers.UnitName.UnitName2))));
		tmp = wowMem.MemReadString((wowMem.MemReadUInt((wowMem.MemReadUInt((objectBase + (unsigned int)WowPointers::UnitName::OBJECTNAME1)) + (unsigned int)WowPointers::UnitName::OBJECTNAME2))));
		return tmp;
	}
	catch (int e)
	{
		std::cout << "itemNameFromBaseAddr: " << e << std::endl;
		return "unnamed";
	}
}
std::string WowHelpers::itemNameFromBaseAddr2(const unsigned int &baseAddr)
{
	unsigned int objectBase = baseAddr;
	std::string tmp = "";
	try
	{

		//tmp = ReadASCIIString((wowMem.ReadUInt((wowMem.ReadUInt((ObjectBase + (uint)Pointers.UnitName.UnitName1)) + (uint)Pointers.UnitName.UnitName2))));
		tmp = wowMem.MemReadString((wowMem.MemReadUInt((wowMem.MemReadUInt((objectBase + (unsigned int)WowPointers::UnitName::OBJECTNAME1)) + (unsigned int)WowPointers::UnitName::OBJECTNAME2))));
		return tmp;
	}
	catch (int e)
	{
		std::cout << "itemNameFromBaseAddr: " << e << std::endl;
		return "unnamed";
	}
}
unsigned int WowHelpers::itemTypeFromBaseAddr(const unsigned int &baseAddr)
{
	unsigned int objectBase = baseAddr;
	unsigned int tmp;
	try
	{
		tmp = wowMem.MemReadUInt((objectBase + (unsigned int)WowPointers::UnitName::ITEMTYPE));
		return tmp;
	}
	catch (int e)
	{
		std::cout << "itemTypeFromBaseAddr: " << e << std::endl;
		return 0;
	}
}
std::string WowHelpers::playerNameFromGuid(const unsigned long long &guid)
{
	std::string tmp = "";
	try
	{
		unsigned long long nameStorePtr = base + WowPointers::UnitName::PLAYERNAMECACHEPOINTER; // Player name database
		unsigned long long base_, testGUID;
		base_ = wowMem.MemReadUInt((unsigned int)nameStorePtr);
		testGUID = wowMem.MemReadULong(((unsigned int)base_ + WowPointers::UnitName::PLAYERNAMEGUIDOFFSET));
		while (testGUID != guid)
		{
			//read next
			base_ = wowMem.MemReadUInt(((unsigned int)base_));
			testGUID = wowMem.MemReadULong(((unsigned int)base_ + WowPointers::UnitName::PLAYERNAMEGUIDOFFSET));
		}

		// Hopefully found the guid in the name list...
		// I don't know how to check for not found
		tmp = wowMem.MemReadString((unsigned int)base_ + WowPointers::UnitName::PLAYERNAMESTRINGOFFSET);
		return tmp;

	}
	catch (int e)
	{
		std::cout << "Error: playerNameFromGuid(): " << e << std::endl;
		return "unnamed";
	}
}
std::string WowHelpers::getDateTime(void)
{
	time_t rawtime;
	struct tm * timeinfo;
	char buffer[80];
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	strftime(buffer, sizeof(buffer), "%H:%M:%S", timeinfo);
	return buffer;
}
double WowHelpers::nfmod(const double &a, const double &b)
{
	return a - b * floor(a / b);
}
double WowHelpers::GetAtan2ToUnit(const WowObject &src, const WowObject &dest)
{
	return nfmod(atan2((src.xPos - dest.xPos), (src.yPos - dest.yPos)), 2 * M_PI);
}
double WowHelpers::getFacingToUnit(const WowObject &dest)
{
	// wtf sometimes it returns a "-" when its under 0.5, anyway doesnt matter because the crititacl point is > 1.5
	//return Math.PI - Math.Abs(LocalPlayer.Rotation - (nfmod(Math.Atan2((LocalPlayer.XPos - dest.XPos), (LocalPlayer.YPos - dest.YPos)), 2 * Math.PI)));
	return abs(M_PI - abs(localPlayer.rotation - (nfmod(atan2((localPlayer.xPos - dest.xPos), (localPlayer.yPos - dest.yPos)), 2 * M_PI))));
}

unsigned long WowHelpers::getTime()
{
	return GetTickCount64();
}

std::string WowHelpers::getSpellName(const unsigned int &id)
{
	try
	{
		unsigned int descriptor = wowMem.MemReadUInt(0x00C0D780 + 0x8);
		unsigned int entryPtr = wowMem.MemReadUInt(descriptor + id * 4);
		unsigned int namePtr = wowMem.MemReadUInt(entryPtr + 0x1E0);
		return wowMem.MemReadString(namePtr);
		//return wowMem.ReadASCIIString(namePtr, 512);
	}
	catch (int e)
	{
		std::cout << "unknown Spell: " << e << std::endl;
	}
}

std::vector<Auras> WowHelpers::getUnitBuffs(const unsigned int &objBaseAddress)
{
	std::vector<Auras> objAuras;
	for (int i = 0; i <= 31; i++)
	{
		//Alternative: int id = *(int*)(*(uint*)(addr + 8) + auraPos * 4)   //the inner *(uint*) is a pointer to the descriptors
		try
		{
			unsigned int buffid = wowMem.MemReadUInt((objBaseAddress + 0xB58 + i * 4));// for i = 0..47
			byte stacks = wowMem.MemReadByte((unsigned int)(objBaseAddress + 0x1e2c + i + 0x108));
			if (buffid != 0)
			{
				objAuras.push_back(Auras(getSpellName(buffid), buffid, 1, (unsigned int)stacks + 1));
			}
		}
		catch (int e)
		{
			std::cout << "GetUnitBuffs: " << e << std::endl;
		}

	}
	return objAuras;
}
std::vector<Auras> WowHelpers::getUnitDebuffs(const unsigned int &objBaseAddress)
{
	std::vector<Auras> objAuras;
	for (int i = 32; i <= 47; i++)
	{
		//Alternative: int id = *(int*)(*(uint*)(addr + 8) + auraPos * 4)   //the inner *(uint*) is a pointer to the descriptors
		try
		{
			unsigned int buffid = wowMem.MemReadUInt((objBaseAddress + 0xB58 + i * 4));// for i = 0..47
			byte stacks = wowMem.MemReadByte((unsigned int)(objBaseAddress + 0xF84 + i + 0xA8));
			if (buffid != 0)
			{
				objAuras.push_back(Auras(getSpellName(buffid), buffid, 1, (unsigned int)stacks + 1));
			}
		}
		catch (int e)
		{
			std::cout << "GetUnitDebuffs: " << e << std::endl;
		}

	}
	return objAuras;
}

//bool WowHelpers::unitHasBuff(const WowObject &unit, const unsigned int &id)
//{
//	for (Auras a : unit.buffList)
//	{
//		if (a.id == id)
//		{
//			return true;
//		}
//	}
//	return false;
//}
//bool WowHelpers::unitHasBuff(const WowObject &unit, const std::string &name)
//{
//	for (Auras a : unit.buffList)
//	{
//		if (a.name == name)
//		{
//			return true;
//		}
//	}
//	return false;
//}

// get Buffs
bool WowHelpers::unitHasBuff2(const WowObject &unit, const unsigned int &id)
{
	for (Auras a : getUnitBuffs(unit.objBaseAddress))
	{
		if (a.id == id)	{ return true; }	
	}
	return false;
}
bool WowHelpers::unitHasBuff2(const WowObject &unit, const std::string &name)
{
	for (Auras a : getUnitBuffs(unit.objBaseAddress))
	{
		if (a.name == name)	{ return true; }
	}
	return false;
}
bool WowHelpers::unitHasBuff2(const WowObjectLight &unit, const unsigned int &id)
{
	for (Auras a : getUnitBuffs(unit.objBaseAddress))
	{
		if (a.id == id) { return true; }
	}
	return false;
}
bool WowHelpers::unitHasBuff2(const WowObjectLight &unit, const std::string &name)
{
	for (Auras a : getUnitBuffs(unit.objBaseAddress))
	{
		if (a.name == name) { return true; }
	}
	return false;
}

//bool WowHelpers::unitHasDebuff(const WowObject &unit, const unsigned int &id)
//{
//	for (Auras a : unit.debuffList)
//	{
//		if (a.id == id)
//		{
//			return true;
//		}
//	}
//	return false;
//}
//bool WowHelpers::unitHasDebuff(const WowObject &unit, const std::string &name)
//{
//	for (Auras a : unit.debuffList)
//	{
//		if (a.name == name)
//		{
//			return true;
//		}
//	}
//	return false;
//}

// get Debuffs
bool WowHelpers::unitHasDebuff2(const WowObject &unit, const unsigned int &id)
{
	for (Auras a : getUnitDebuffs(unit.objBaseAddress))
	{
		if (a.id == id) { return true; }
	}
	return false;
}
bool WowHelpers::unitHasDebuff2(const WowObject &unit, const std::string &name)
{
	for (Auras a : getUnitDebuffs(unit.objBaseAddress))
	{
		if (a.name == name) { return true; }
	}
	return false;
}
bool WowHelpers::unitHasDebuff2(const WowObjectLight &unit, const unsigned int &id)
{
	for (Auras a : getUnitDebuffs(unit.objBaseAddress))
	{
		if (a.id == id) { return true; }
	}
	return false;
}
bool WowHelpers::unitHasDebuff2(const WowObjectLight &unit, const std::string &name)
{

	for (Auras a : getUnitDebuffs(unit.objBaseAddress))
	{
		if (a.name == name) { return true; }
	}
	return false;
}

void WowHelpers::configSave()
{
	std::vector<int> a;
	std::vector<int> h;
	for (int i = 0; i < 2000; i++)
	{
		if (hordeHostileFaction[i])
		{
			h.push_back(i);
		}
		if (allianceHostileFaction[i])
		{
			a.push_back(i);
		}
	}

	Config::cfgSave(0, 0, h, a);
}
void WowHelpers::configLoad()
{
	Config::cfgLoad();
	cout << "dddd" << endl;

	std::vector<int> hostileFactionAllianceCfg = Config::getHostileFactionAlliance();
	std::vector<int> hostileFactionHordeCfg = Config::getHostileFactionHorde();
	cout << "dddd2" << endl;

	// fill hostile factions
	playerAllianceFaction.resize(120);
	playerAllianceFaction[1] = true;
	playerAllianceFaction[3] = true;
	playerAllianceFaction[4] = true;
	playerAllianceFaction[8] = true;
	playerAllianceFaction[115] = true;

	playerHordeFaction.resize(120);
	playerHordeFaction[2] = true;
	playerHordeFaction[5] = true;
	playerHordeFaction[6] = true;
	playerHordeFaction[9] = true;
	playerHordeFaction[116] = true;

	allianceHostileFaction.resize(2000);
	allianceHostileFaction[2] = true;
	allianceHostileFaction[5] = true;
	allianceHostileFaction[6] = true;
	allianceHostileFaction[9] = true;
	allianceHostileFaction[116] = true;
	cout << "ffff" << endl;
	for (int x : hostileFactionAllianceCfg)
	{
		
		cout << x << endl;
		allianceHostileFaction[x] = true;
	}

	hordeHostileFaction.resize(2000);
	hordeHostileFaction[1] = true;
	hordeHostileFaction[3] = true;
	hordeHostileFaction[4] = true;
	hordeHostileFaction[8] = true;
	hordeHostileFaction[115] = true;
	cout << "ffff2" << endl;
	for (int x : hostileFactionHordeCfg)
	{
		cout << x << endl;
		hordeHostileFaction[x] = true;
	}



}



bool WowHelpers::isHostile(const WowObjectLight &unit)
{
	return playerAllianceFaction[localPlayer.factionTemplate] && allianceHostileFaction[unit.factionTemplate] || playerHordeFaction[localPlayer.factionTemplate] && hordeHostileFaction[unit.factionTemplate];
}
bool WowHelpers::isHostile(const WowObject &unit)
{
	return playerAllianceFaction[localPlayer.factionTemplate] && allianceHostileFaction[unit.factionTemplate] || playerHordeFaction[localPlayer.factionTemplate] && hordeHostileFaction[unit.factionTemplate];
}
bool WowHelpers::hasHostileTarget(const WowObject &humanPlayer, const  WowObjectLight &unit)
{
	return playerAllianceFaction[humanPlayer.factionTemplate] && allianceHostileFaction[unit.factionTemplate] || playerHordeFaction[humanPlayer.factionTemplate] && hordeHostileFaction[unit.factionTemplate];	
}
bool WowHelpers::hasHostileTarget(const WowObject &humanPlayer, const  WowObject &unit)
{
	return playerAllianceFaction[humanPlayer.factionTemplate] && allianceHostileFaction[unit.factionTemplate] || playerHordeFaction[humanPlayer.factionTemplate] && hordeHostileFaction[unit.factionTemplate];
}
bool WowHelpers::hasFriendlyTarget(const WowObject &humanPlayer, const WowObjectLight &unit)
{
	return playerAllianceFaction[humanPlayer.factionTemplate] && !allianceHostileFaction[unit.factionTemplate] || playerHordeFaction[humanPlayer.factionTemplate] && !hordeHostileFaction[unit.factionTemplate];
}
bool WowHelpers::hasFriendlyTarget(const WowObject &humanPlayer, const WowObject &unit)
{
	return playerAllianceFaction[humanPlayer.factionTemplate] && !allianceHostileFaction[unit.factionTemplate] || playerHordeFaction[humanPlayer.factionTemplate] && !hordeHostileFaction[unit.factionTemplate];
}
bool WowHelpers::hasTarget(const WowObject &unit)
{
	return unit.targetGuid != 0;
}

WowObject WowHelpers::getItemObj(const int &id)
{
	WowObject obj = from(cachedUnitlist)
		>> where([&](WowObject const & o) {return o.itemId == id; })
		>> first_or_default()
		;
	return obj;
}


 std::vector<WowObject> WowHelpers::getNearEnemies(const double &radius = 15, const unsigned int &maxNumberToScan = 5)
{
	 std::vector<WowObject> IsTargetOfList = from(cachedUnitlist)
		 >> where([&](WowObject const & o) {return (isHostile(o) && o.distance <= radius && o.health > 0 && ((o.type == (short)WowPointers::WoWObjectTypes::OT_PLAYER && hasPvpEnabled(o.unitFlags)) || o.type == (short)WowPointers::WoWObjectTypes::OT_UNIT)); })
		 >> orderby([&](WowObject const & o) {return (o.health); })
		 >> take(maxNumberToScan)
		 >> to_vector()
		 ;
	return IsTargetOfList;
}

 void WowHelpers::unbanMultiDmgObj()
 {
	 // erstellt ein vector mit dem iterator-typ, wir können die gebannten Adressen nicht direkt löschen, da sonst beim shcleifendurchlauf der iterator verzerrt wird und es zum crash kommt
	 std::vector<std::_Tree_const_iterator<class std::_Tree_val<struct std::_Tree_simple_types<struct std::pair<unsigned __int64 const, unsigned long> > > >> toDel;
	 for (auto it = multiDamageBanlist.cbegin(); it != multiDamageBanlist.cend(); ++it)
	 {

		 if (it->second < currentTime - 5000)
		 {
			 //cout << "unban: " << it->first << endl;
			 toDel.push_back(it);			 
		 }
	 }
	 // und nun können wir hier die gebannten targets wieder entfernen
	 for (auto itv : toDel)
	 {
		 multiDamageBanlist.erase(itv);
	 }
 }

 std::vector<WowObject> WowHelpers::getNearEnemiesMultiDamageTapped(const unsigned int &hPercent, const std::string &spell, const unsigned long long targetGuid, const double &radius, const unsigned int &maxNumberToScan)
 {
	unsigned int scannedObj = 0;
	std::vector<WowObject> isNotBanned;
	for (WowObject obj : cachedUnitlist)
	{
		if (scannedObj >= maxNumberToScan)
		{
			return isNotBanned;
		}
		//cout << hex << "dyn: " << obj.dynamicFlags << dec << "   h: " << obj.health << "  debuff: " << unitHasDebuff2(obj, spell) << "  hostile: " << isHostile(obj) << "   dist: " << obj.distance << "  name: " << obj.name << "  type: " << obj.type << "   fact: " << obj.factionTemplate  << endl;
		// const unsigned int TAPPEDBYME = 0XC; funktioniert auf meinem servern nicht, dort sind die werte anders
		if ((obj.healthPercent > hPercent && (!unitHasDebuff2(obj,spell) || spell == "")) && (obj.targetGuid == targetGuid || targetGuid == 0) && isHostile(obj) && obj.distance <= radius && obj.type == (short)WowPointers::WoWObjectTypes::OT_UNIT && obj.dynamicFlags == 0xC)
		//if ((obj.health > health && (!unitHasDebuff2(obj, spell) || spell == "")) && isHostile(obj) && obj.distance <= radius && ((obj.type == (short)WowPointers::WoWObjectTypes::OT_PLAYER && hasPvpEnabled(obj.unitFlags)) || obj.type == (short)WowPointers::WoWObjectTypes::OT_UNIT))
		{
			bool banned = false;
			for (auto it = multiDamageBanlist.cbegin(); it != multiDamageBanlist.cend(); ++it)
			{
				if (it->first == obj.guid)
				{
					banned = true;
					break;
				}
			}
			if (!banned)
			{
				cout << "add obj" << obj.name << endl;
				isNotBanned.push_back(obj);
				scannedObj++;
			}
			cout << endl;
		}
	}
	return isNotBanned;
}

std::vector<WowObject> WowHelpers::getNearEnemiesMultiDamage(const unsigned int &hPercent, const std::string &spell, const unsigned long long targetGuid, const double &radius, const unsigned int &maxNumberToScan)
{
	unsigned int scannedObj = 0;
	std::vector<WowObject> isNotBanned;
	for (WowObject obj : cachedUnitlist)
	{
		if (scannedObj >= maxNumberToScan)
		{
			return isNotBanned;
		}
		//cout << hex << "dyn: " << obj.dynamicFlags << dec <<"   h: " << obj.health << "  debuff: " << unitHasDebuff2(obj, spell) << "  hostile: " << isHostile(obj) << "   dist: " << obj.distance << "  name: " << obj.name << "  type: " << obj.type << "   fact: " << obj.factionTemplate  << endl;
		// const unsigned int TAPPEDBYOTHERS = 0X4; dynamic flags funktionieren auf meinem Server nicht, aber "!= 0x4" dürfte ja dennoch funzen
		if ((obj.healthPercent > hPercent && (!unitHasDebuff2(obj, spell) || spell == "")) && (obj.targetGuid == targetGuid || targetGuid == 0) && isHostile(obj) && obj.distance <= radius && obj.type == (short)WowPointers::WoWObjectTypes::OT_UNIT && obj.dynamicFlags != 0x4)
		//if ((obj.health > health && (!unitHasDebuff2(obj, spell) || spell == "")) && isHostile(obj) && obj.distance <= radius && ((obj.type == (short)WowPointers::WoWObjectTypes::OT_PLAYER && hasPvpEnabled(obj.unitFlags)) || obj.type == (short)WowPointers::WoWObjectTypes::OT_UNIT))
		{
			bool banned = false;
			for (auto it = multiDamageBanlist.cbegin(); it != multiDamageBanlist.cend(); ++it)
			{
				if (it->first == obj.guid)
				{
					banned = true;
					break;
				}
			}
			if (!banned)
			{
				cout << "add obj" << obj.name << endl;
				isNotBanned.push_back(obj);
				scannedObj++;
			}
		}
	}
	return isNotBanned;
}

std::vector<WowObject> WowHelpers::getNearEnemiesPetTaunt(const unsigned int &hPercent, const std::string &spell, const unsigned long long targetGuid, const double &radius, const unsigned int &maxNumberToScan)
{
	unsigned int scannedObj = 0;
	std::vector<WowObject> isNotBanned;
	for (WowObject obj : cachedUnitlist)
	{
		if (scannedObj >= maxNumberToScan)
		{
			return isNotBanned;
		}
		//cout << hex << "dyn: " << obj.dynamicFlags << dec <<"   h: " << obj.health << "  debuff: " << unitHasDebuff2(obj, spell) << "  hostile: " << isHostile(obj) << "   dist: " << obj.distance << "  name: " << obj.name << "  type: " << obj.type << "   fact: " << obj.factionTemplate  << endl;
		// const unsigned int TAPPEDBYOTHERS = 0X4; dynamic flags funktionieren auf meinem Server nicht, aber "!= 0x4" dürfte ja dennoch funzen
		if ((obj.healthPercent > hPercent && (!unitHasDebuff2(obj, spell) || spell == "")) && (obj.targetGuid == targetGuid || targetGuid == 0) && isHostile(obj) && obj.distance <= radius && obj.type == (short)WowPointers::WoWObjectTypes::OT_UNIT && obj.dynamicFlags != 0x4)
			//if ((obj.health > health && (!unitHasDebuff2(obj, spell) || spell == "")) && isHostile(obj) && obj.distance <= radius && ((obj.type == (short)WowPointers::WoWObjectTypes::OT_PLAYER && hasPvpEnabled(obj.unitFlags)) || obj.type == (short)WowPointers::WoWObjectTypes::OT_UNIT))
		{
			bool banned = false;
			for (auto it = multiDamageBanlist.cbegin(); it != multiDamageBanlist.cend(); ++it)
			{
				if (it->first == obj.guid)
				{
					banned = true;
					break;
				}
			}
			if (!banned)
			{
				cout << "add obj" << obj.name << endl;
				isNotBanned.push_back(obj);
				scannedObj++;
			}
			sort(isNotBanned.begin(), isNotBanned.end(), [](const WowObject &lhs, const WowObject &rhs)
			{
				return lhs.distance < rhs.distance;
			});
		}
	}
	return isNotBanned;
}

bool WowHelpers::isEliteMob(WowObject target)
{
	return target.unitFlags << 25 >> 31 == 1;
}

std::vector<WowObject> WowHelpers::getNearFriendlyPlayers(const double &radius = 15, const unsigned int &maxNumberToScan = 5)
{
	std::vector<WowObject> friendList = from(cachedUnitlist)
		>> where([&](WowObject const & o) {return (!isHostile(o) && o.distance <= radius && o.type == (short)WowPointers::WoWObjectTypes::OT_PLAYER && o.guid != localPlayer.guid); })
		>> take(maxNumberToScan)
		>> to_vector()
		;
	return friendList;
}

 WowObject WowHelpers::getTargetPlayer(const WowObject &unit)
 {
	 WowObject isTargetOf = from(cachedUnitlist)
		 >> where([&](WowObject const & o) {return (o.type == (short)WowPointers::WoWObjectTypes::OT_PLAYER || o.type == (short)WowPointers::WoWObjectTypes::OT_UNIT) && o.guid == unit.targetGuid; })
		 >> first_or_default()
		 ;
	 if (isTargetOf.guid != 0)
	 {
		 return isTargetOf;
	 }
	 return WowObject();
 }
 WowObject WowHelpers::getTargetNpc(const WowObject &unit)
 {
	 WowObject isTargetOf = from(cachedUnitlist)
		 >> where([&](WowObject const & o) {return (o.type == (short)WowPointers::WoWObjectTypes::OT_UNIT || o.type == (short)WowPointers::WoWObjectTypes::OT_PLAYER) && o.guid == unit.targetGuid; })
		 >> first_or_default()
	 ;
	 if (isTargetOf.guid != 0)
	 {
		 return isTargetOf;
	 }
	 return WowObject();
 }


unsigned int WowHelpers::getStacksOfDebuff(WowObject unit, std::string aura)
{
	//Scans all Targets for "Sunder Armor"-Debuff and assign the stacks to the respective WowObject, if no Auras-Object, it returns stacks a new empty Object of Auras via Null-Coalesce Operator

	for (Auras result : getUnitDebuffs(unit.objBaseAddress))
	{
		if (result.name == aura)
		{
			return result.stacks;
		}
	}
	return 0;
	//c#: return (GetUnitDebuffs(unit).FirstOrDefault(c = > c.Name == aura) ? ? tempEmptyAura).Stacks;
}

bool WowHelpers::hasPvpEnabled(unsigned int unitFlags)
{
	if (unitFlags << 19 >> 31 == 1)
	{
		return true;
	}
}
bool WowHelpers::isInCombat(unsigned int unitFlags)
{
	if (unitFlags << 12 >> 31 == 1)
	{
		return true;
	}
}

std::string WowHelpers::getErrMsg()
{
	return wowMem.MemReadString(WowPointers::StaticAddresses::LASTSCREENMESSAGE);
}

void WowHelpers::getPlayerSpells()
{
	cout << "Scan spell book for spells" << endl;
	int spellCounter = 0;
	spellsInSpellBook.clear();
	while (true)
	{
		try
		{
			auto currentSpellId = wowMem.MemReadUInt(WowPointers::ObjectManager::CURPLAYERSPELLPTR + (spellCounter * 4));
			if (currentSpellId == 0) break;
			auto entryPtr = wowMem.MemReadUInt(wowMem.MemReadUInt(0x00C0D780 + 8) + currentSpellId * 4);

			unsigned int rank = 1;
			auto entrySpellId = wowMem.MemReadUInt(entryPtr);
			auto namePtr = wowMem.MemReadUInt(entryPtr + 0x1E0);
			auto name = wowMem.MemReadString(namePtr); // Will default to ascii
			for (Spells o : spellsInSpellBook)
			{
				if (o.name == name)
				{
					rank++;
				}
			}
			playerFastSpellList[currentSpellId] = name;
			Spells spell(name, 0, entrySpellId, rank);
			spellsInSpellBook.push_back(spell);
		}
		catch (int e)
		{
			cerr << "GetPlayerSpells: " << e << endl;
		}
		spellCounter++;
	}
	// add spells from pet
	getPetSpells();
	//// manually add attack
	//Spells spell2("Attack", 0, 6603,1);
	//spellsInSpellBook.push_back(spell2);
	printSpells(spellsInSpellBook);
}

void WowHelpers::getPetSpells()
{
	cout << "Scan spell pet book for spells" << endl;
	int spellCounter = 0;
	while (true)
	{		
		try
		{
			auto currentSpellId = wowMem.MemReadUInt(WowPointers::ObjectManager::CURPETSPELLPTR + (spellCounter * 4));
			if (currentSpellId == 0) break;
			auto entryPtr = wowMem.MemReadUInt(wowMem.MemReadUInt(0x00C0D780 + 8) + currentSpellId * 4);

			unsigned int rank = 1;
			auto entrySpellId = wowMem.MemReadUInt(entryPtr);
			auto namePtr = wowMem.MemReadUInt(entryPtr + 0x1E0);
			auto name = wowMem.MemReadString(namePtr); // Will default to ascii
			for (Spells o : spellsInSpellBook)
			{
				if (o.name == name)
				{
					rank++;
				}
			}
			playerFastSpellList[currentSpellId] = name;
			Spells spell(name, 0, entrySpellId, rank);
			spellsInSpellBook.push_back(spell);
		}
		catch (int e)
		{
			cerr << "GetPetSpells: " << e << endl;
		}
		spellCounter++;
	}
}

bool  WowHelpers::spellsAndRank()
{
	allPlayerSpellsAndRank.clear();
	for (Spells o : spellsInSpellBook)
	{
		if (o.rank > 1)
		{
			std::map<std::string, unsigned int>::iterator it = allPlayerSpellsAndRank.find(o.name);
			if (it != allPlayerSpellsAndRank.end())
			{
				it->second = o.rank;
			}
		}
		else
		{
			allPlayerSpellsAndRank.insert(make_pair(o.name, o.rank));
		}
	}

	std::map<std::string, unsigned int>::iterator it2;
	for (it2 = allPlayerSpellsAndRank.begin(); it2 != allPlayerSpellsAndRank.end(); it2++)
	{
		std::cout << it2->first  // string (key)
			<< ':'
			<< it2->second   // string's value 
			<< std::endl;
	}
	return true;

}




unsigned int WowHelpers::getSpellIdFromSpellName(const std::string &name)
{

	Spells result = from(spellsInSpellBook)
		>> where([&](Spells const & o) {return (o.name == name); })
		>> orderby_descending([&](Spells const & o) {return o.id; })
		>> first_or_default()
		;
	if (result.id > 0)
	{
		return result.id;
	}
	else
	{
		return 0;
	}
}

std::string WowHelpers::getSpellNameFromSpellId(const unsigned int &id)
{
	for (Spells x : spellsInSpellBook)
	{
		if (x.id == id)
		{
			return x.name;
		}
	}
	return "";
}

bool WowHelpers::spellExist(int spellId)
{
	for (Spells result : spellsInSpellBook)
	{
		if (result.id == spellId)
		{
			return true;
		}		
	}
	return false;
}
bool WowHelpers::spellExist(std::string spellName)
{
	for (Spells result : spellsInSpellBook)
	{
		if (result.name == spellName)
		{
			return true;
		}		
	}
	return false;
}


void  WowHelpers::printSpells(std::vector<Spells> spellList)
 {
	 for (Spells x : spellList)
	 {
		 cout << dec << "ID: " << x.id << " \t" << x.name << endl;
	 }
 }

std::vector<SpellsInHistory> WowHelpers::getPlayerSpellsOnCooldown()
 {
	cout << "Spellhistory" << endl;
	//Log.Print("--GetPlayerSpellsInHistory");
	std::vector<SpellsInHistory> spellListOnCd;
	//Current time in ms
	try
	{
		auto currentTime = wowMem.MemReadFloat(WowPointers::StaticAddresses::TIMESTAMP) * 1000;

		//Get first list object
		auto currentListObject = wowMem.MemReadUInt(WowPointers::SpellHistory::SPELLHISTORYBASE + WowPointers::SpellHistory::FIRSTREC);

		unsigned int spellId = 0;
		float cooldown = 0;
		unsigned int startTime = 0;//get record with latest starttime
		while ((currentListObject != 0) && ((currentListObject & 1) == 0))
		{
			spellId = wowMem.MemReadUInt(currentListObject + (unsigned int)WowPointers::SpellHistory::SPELLID);
			//cout << "Spell: " << dec << spellId << endl;
			auto currentStartTime = wowMem.MemReadUInt(currentListObject + WowPointers::SpellHistory::STARTTIME);
			if (currentStartTime > startTime) //get CD for the latest start time record
			{
				startTime = currentStartTime;
				// there exists 2 offsets with different values for CD, we check both.
				auto spellCDx20 = wowMem.MemReadUInt(currentListObject + WowPointers::SpellHistory::SPELLCOOLDOWNX20);
				auto spellCDx14 = wowMem.MemReadUInt(currentListObject + WowPointers::SpellHistory::SPELLCOOLDOWNX14);
				
				cooldown = max(roundf((startTime + spellCDx20 - currentTime) / 100) / 10, roundf((startTime + spellCDx14 - currentTime) / 100) / 10);
				if (cooldown > 0)
				{
					spellId = wowMem.MemReadUInt(currentListObject + WowPointers::SpellHistory::SPELLID);
					SpellsInHistory spellOnCd(spellId, cooldown,currentStartTime);
					spellListOnCd.push_back(spellOnCd);
				}
			}
			//Get next list object
			currentListObject = wowMem.MemReadUInt(currentListObject + WowPointers::SpellHistory::NEXTREC);
		}
	}
	catch (int e)
	{
		cerr << "GetPlayerSpellsInHistory: " << e << endl;
	}
	return spellListOnCd;
 }

std::vector<SpellsInHistory> WowHelpers::getPlayerSpellHistory()
{
	//Log.Print("--GetPlayerSpellsInHistory");
	std::vector<SpellsInHistory> spellList;
	//Current time in ms
	try
	{
		auto currentTime = wowMem.MemReadFloat(WowPointers::StaticAddresses::TIMESTAMP) * 1000;

		//Get first list object
		auto currentListObject = wowMem.MemReadUInt(WowPointers::SpellHistory::SPELLHISTORYBASE + WowPointers::SpellHistory::FIRSTREC);

		unsigned int spellId = 0;
		float cooldown = 0;
		unsigned int startTime = 0;//get record with latest starttime
		while ((currentListObject != 0) && ((currentListObject & 1) == 0))
		{
			auto currentStartTime = wowMem.MemReadUInt(currentListObject + WowPointers::SpellHistory::STARTTIME);
			if (currentStartTime > startTime) //get CD for the latest start time record
			{
				startTime = currentStartTime;
				// there exists 2 offsets with different values for CD, we check both.
				auto spellCDx20 = wowMem.MemReadUInt(currentListObject + WowPointers::SpellHistory::SPELLCOOLDOWNX20);
				auto spellCDx14 = wowMem.MemReadUInt(currentListObject + WowPointers::SpellHistory::SPELLCOOLDOWNX14);
				cooldown = max(roundf((startTime + spellCDx20 - currentTime) / 100) / 10, roundf((startTime + spellCDx14 - currentTime) / 100) / 10);
				spellId = wowMem.MemReadUInt(currentListObject + WowPointers::SpellHistory::SPELLID);
				SpellsInHistory spellOnCd(spellId, cooldown, currentStartTime);
				spellList.push_back(spellOnCd);
			}
			//Get next list object
			currentListObject = wowMem.MemReadUInt(currentListObject + WowPointers::SpellHistory::NEXTREC);
		}
	}
	catch (int e)
	{
		cerr << "GetPlayerSpellsInHistory: " << e << endl;
	}
	return spellList;
}

 float WowHelpers::getSpellCooldown(string spellName)
 {
	 return getSpellCooldown(getSpellIdFromSpellName(spellName));
 }
 float WowHelpers::getSpellCooldown(unsigned int spellID)
 {
	 //Current time in ms
	 float currentTime = 0.0f;
	 unsigned int currentListObject = 0;
	 float cooldown = 0;
	 unsigned int startTime = 0;//get record with latest starttime

	 try
	 {
		 currentTime = wowMem.MemReadFloat((unsigned int)WowPointers::StaticAddresses::TIMESTAMP) * 1000;
		 //Get first list object
		 currentListObject = wowMem.MemReadUInt(WowPointers::SpellHistory::SPELLHISTORYBASE + WowPointers::SpellHistory::FIRSTREC);

		 while ((currentListObject != 0) && ((currentListObject & 1) == 0))
		 {
			 if (wowMem.MemReadUInt(currentListObject + WowPointers::SpellHistory::SPELLID) == spellID)//filter by ID here
			 {
				 auto currentStartTime = wowMem.MemReadUInt(currentListObject + WowPointers::SpellHistory::STARTTIME);
				 if (currentStartTime > startTime) //get CD for the latest start time record
				 {
					 startTime = currentStartTime;
					 // there exists 2 offsets with different values for CD, we check both.
					 auto spellCDx20 = wowMem.MemReadUInt(currentListObject + WowPointers::SpellHistory::SPELLCOOLDOWNX20);
					 auto spellCDx14 = wowMem.MemReadUInt(currentListObject + WowPointers::SpellHistory::SPELLCOOLDOWNX14);
					 cooldown =  max(roundf((startTime + spellCDx20 - currentTime) / 100)/10, roundf((startTime + spellCDx14 - currentTime) / 100)/10);
				 }
			 }
			 //Get next list object
			 currentListObject = wowMem.MemReadUInt(currentListObject + WowPointers::SpellHistory::NEXTREC);
		 }
	 }
	 catch (int e)
	 {
		 cerr << "GetSpellCooldown error#: " << e << endl;		 
	 }
	 return (cooldown < 0) ? 0 : cooldown;
 }

bool WowHelpers::globalCooldown()
{
	float currentTime = 0.0f;
	unsigned int currentListObject = 0;
	try
	{
		//Current time in ms
		currentTime = wowMem.MemReadFloat(WowPointers::StaticAddresses::TIMESTAMP) * 1000;
		//Get first list object
		currentListObject = wowMem.MemReadUInt(WowPointers::SpellHistory::SPELLHISTORYBASE + WowPointers::SpellHistory::FIRSTREC);

		while ((currentListObject != 0) && ((currentListObject & 1) == 0))
		{
			//auto bytes = wowMem.ReadBytes(currentListObject, 0x100);
			//Start time of the spell cooldown in ms
			auto startTime = wowMem.MemReadUInt(currentListObject + WowPointers::SpellHistory::STARTTIME);

			//Absolute gcd of the spell in ms
			auto globalCooldown = wowMem.MemReadUInt(currentListObject + WowPointers::SpellHistory::GLOBALCOOLDOWN);

			//Spell on gcd?
			if ((startTime + globalCooldown) > currentTime)
			{
				return true;
			}

			//Get next list object
			currentListObject = wowMem.MemReadUInt(currentListObject + WowPointers::SpellHistory::NEXTREC);
		}
		return false;
	}
	catch (int e)
	{
		cerr << "var GlobalCooldown error#: " << e << endl;
		return false;
	}

}

 WowObjectLight WowHelpers::WowObjectToWowObjectLight(const WowObject &obj)
 {
	 WowObjectLight tmpObj;
	 tmpObj.guid = obj.guid;
	 tmpObj.xPos = obj.xPos;
	 tmpObj.yPos = obj.yPos;
	 tmpObj.zPos = obj.zPos;
	 tmpObj.wowClass = obj.wowClass;
	 tmpObj.distance = obj.distance;
	 tmpObj.factionTemplate = obj.factionTemplate;
	 tmpObj.vector3d = obj.vector3d;
	 tmpObj.lastVector3d = obj.lastVector3d;
	 tmpObj.predictLocObj = obj.predictLocObj;
	 tmpObj.rotation = obj.rotation;
	 tmpObj.objBaseAddress = obj.objBaseAddress;
	 tmpObj.unitFieldsAddress = obj.unitFieldsAddress;
	 tmpObj.type = obj.type;
	 tmpObj.dodged = obj.dodged;

	 tmpObj.isMoving = obj.isMoving;
	 tmpObj.isInCombat = obj.isInCombat;
	 tmpObj.isFighting = obj.isFighting;
	 tmpObj.isFleeing = obj.isFleeing;
	 tmpObj.isStunned = obj.isStunned;
	 tmpObj.cantMove = obj.cantMove;
	 tmpObj.isConfused = obj.isConfused;
	 tmpObj.hasBreakableCc = obj.hasBreakableCc;
	 tmpObj.buffList = obj.buffList;
	 tmpObj.debuffList = obj.debuffList;
	 tmpObj.channelSpell = obj.channelSpell;
	 tmpObj.castSpell = obj.castSpell;
	 tmpObj.playerIsFacingTo = obj.playerIsFacingTo;
	 tmpObj.movementFlags = obj.movementFlags;
	 tmpObj.dynamicFlags = obj.dynamicFlags;
	 tmpObj.unitFlags = obj.unitFlags;
	 tmpObj.npcFlags = obj.npcFlags;
	 tmpObj.isHostile = obj.isHostile;
	 tmpObj.tempBuffStacks = obj.tempBuffStacks;
	 tmpObj.tempNextSpell = obj.tempNextSpell;
	 tmpObj.tempTargetPrio = obj.tempTargetPrio;

	 tmpObj.health = obj.health;
	 tmpObj.maxHealth = obj.maxHealth;
	 tmpObj.healthPercent = obj.healthPercent;
	 tmpObj.mana = obj.mana;
	 tmpObj.maxMana = obj.maxMana;
	 tmpObj.manaPercent = obj.manaPercent;
	 tmpObj.rage = obj.rage;
	 tmpObj.energy = obj.energy;
	 tmpObj.level = obj.level;

	 tmpObj.stacks = obj.stacks;
	 tmpObj.itemOwner = obj.itemOwner;
	 tmpObj.itemId = obj.itemId;
	 tmpObj.gameObjectType = obj.gameObjectType;
	 return tmpObj;
 }
 WowObject WowHelpers::WowObjectLightToWowObject(const WowObjectLight &obj)
 {
	 WowObject tmpObj;
	 tmpObj.guid = obj.guid;
	 tmpObj.xPos = obj.xPos;
	 tmpObj.yPos = obj.yPos;
	 tmpObj.zPos = obj.zPos;
	 tmpObj.wowClass = obj.wowClass;
	 tmpObj.distance = obj.distance;
	 tmpObj.factionTemplate = obj.factionTemplate;
	 tmpObj.lastVector3d = obj.lastVector3d;
	 tmpObj.predictLocObj = obj.predictLocObj;
	 tmpObj.rotation = obj.rotation;
	 tmpObj.objBaseAddress = obj.objBaseAddress;
	 tmpObj.unitFieldsAddress = obj.unitFieldsAddress;
	 tmpObj.type = obj.type;
	 tmpObj.dodged = obj.dodged;

	 tmpObj.targetGuid = wowMem.MemReadULong((tmpObj.unitFieldsAddress + Descriptors::WowUnitFields::TARGET));
	 tmpObj.isMoving = obj.isMoving;
	 tmpObj.isInCombat = obj.isInCombat;
	 tmpObj.isFighting = obj.isFighting;
	 tmpObj.isFleeing = obj.isFleeing;
	 tmpObj.isStunned = obj.isStunned;
	 tmpObj.cantMove = obj.cantMove;
	 tmpObj.isConfused = obj.isConfused;
	 tmpObj.hasBreakableCc = obj.hasBreakableCc;
	 tmpObj.buffList = obj.buffList;
	 tmpObj.debuffList = obj.debuffList;
	 tmpObj.channelSpell = obj.channelSpell;
	 tmpObj.castSpell = obj.castSpell;
	 tmpObj.playerIsFacingTo = obj.playerIsFacingTo;
	 tmpObj.movementFlags = obj.movementFlags;
	 tmpObj.dynamicFlags = obj.dynamicFlags;
	 tmpObj.unitFlags = obj.unitFlags;
	 tmpObj.npcFlags = obj.npcFlags;
	 tmpObj.isHostile = obj.isHostile;
	 tmpObj.tempBuffStacks = obj.tempBuffStacks;
	 tmpObj.tempNextSpell = obj.tempNextSpell;
	 tmpObj.tempTargetPrio = obj.tempTargetPrio;

	 tmpObj.health = obj.health;
	 tmpObj.maxHealth = obj.maxHealth;
	 tmpObj.healthPercent = obj.healthPercent;
	 tmpObj.mana = obj.mana;
	 tmpObj.maxMana = obj.maxMana;
	 tmpObj.manaPercent = obj.manaPercent;
	 tmpObj.rage = obj.rage;
	 tmpObj.energy = obj.energy;
	 tmpObj.level = obj.level;
	 tmpObj.stacks = obj.stacks;
	 tmpObj.itemOwner = obj.itemOwner;
	 tmpObj.itemId = obj.itemId;
	 tmpObj.gameObjectType = obj.gameObjectType;
	 return tmpObj;
 }

 


void WowHelpers::scanObjLight()
{
	lastTarget = target;
	std::vector<WowObject> cachedUnitlistLight;
	//if (lastScanObjLight + blockScanObjLight > getTime())
	//{
	//	cout << lastScanObjLight + blockScanObjLight << " >>> " << getTime() << endl;
	//}
	if (lastScanObjLight + blockScanObjLight < getTime())
	{
		//cout << lastScanObjLight + blockScanObjLight << " <   " << getTime() << endl;
		bool foundLocalPlayer = false;
		bool foundTarget = false;
		lastScanObjLight = getTime();
		try
		{
			localPlayer.targetGuid = wowMem.MemReadULong(base + WowPointers::StaticAddresses::CURRENTTARGETGUID);
			target.guid = localPlayer.targetGuid;
		}
		catch (int e)
		{
			std::cout << "Error read CURRENTTARGETGUID" << e << std::endl;
		}


		cachedUnitlistLight.clear();
		// set our counter variable to 0 so we can begin counting the objects
		int totalWowObjects = 0;
		WowObject currentObject;
		// set our current object as the first in the object manager
		currentObject.objBaseAddress = firstObject;
		while ((currentObject.objBaseAddress & 1) == 0)
		{

			// type independent informations
			try
			{
				totalWowObjects++;
				currentObject.guid = wowMem.MemReadULong((currentObject.objBaseAddress + WowPointers::WowObjectData::GUID));
				//DWORD x = currentObject.objBaseAddress + WowPointers::WowObjectData::GUID;
			}
			catch (int e)
			{
				std::cout << "Error read currentObject.objBaseAddress + WowPointers::WowObjectData::GUID" << e << std::endl;
			}
			if (currentObject.guid == target.guid)
			{
				try
				{
					target.objBaseAddress = currentObject.objBaseAddress;
					target.xPos = wowMem.MemReadFloat((currentObject.objBaseAddress + WowPointers::WowObjectData::X));
					target.yPos = wowMem.MemReadFloat((currentObject.objBaseAddress + WowPointers::WowObjectData::Y));
					target.zPos = wowMem.MemReadFloat((currentObject.objBaseAddress + WowPointers::WowObjectData::Z));
					target.vector3d.set(target.xPos, target.yPos, target.zPos);


				}
				catch (int e)
				{

					std::cout << "ScanObjLight() -> switch/case: " << e << std::endl;
				}

				// target gefunden, und raus!
				foundTarget = true;
			}
			if (currentObject.guid == localPlayer.guid)
			{
				try
				{
					localPlayer.objBaseAddress = currentObject.objBaseAddress;
					localPlayer.xPos = wowMem.MemReadFloat((currentObject.objBaseAddress + WowPointers::WowObjectData::X));
					localPlayer.yPos = wowMem.MemReadFloat((currentObject.objBaseAddress + WowPointers::WowObjectData::Y));
					localPlayer.zPos = wowMem.MemReadFloat((currentObject.objBaseAddress + WowPointers::WowObjectData::Z));
					localPlayer.vector3d.set(localPlayer.xPos, localPlayer.yPos, localPlayer.zPos);

				}
				catch (int e)
				{

					std::cout << "ScanObjLight() -> switch/case: " << e << std::endl;
				}

				// target gefunden, und raus!
				foundLocalPlayer = true;
			}
			// if localplayer and target was found on objectlist, then quit the loop
			if (foundLocalPlayer && (foundTarget || localPlayer.targetGuid == 0))
			{
				//cout << endl;
				//cout << "[[[[meptr: " << hex << &localPlayer << endl;
				localPlayer.collectLocData(myLog, "player");
				//if (localPlayer.targetGuid == 0)
				//{
				//	//unsigned long predictNoneTargetLocTimeDelta = getTime() - target.predictLocObj.targetLastVectorCheck;

				//	target.predictLocObj = localPlayer.predictLocObj;
				//	target.distance = 0;
				//	target.guid = localPlayer.guid;
				//	target.vector3d = localPlayer.vector3d;
				//	target.xPos = localPlayer.xPos;
				//	target.yPos = localPlayer.yPos;
				//	target.zPos = localPlayer.zPos;
				//	target.guid = target.guid + 100;
				//}
				if (localPlayer.targetGuid != 0)
				{
					target.distance = round((localPlayer.vector3d.distance(target.vector3d)) * 100) / 100;
					//cout << "[[[[tarptr: " << hex << &target << endl;
					target.collectLocData(myLog,"target");
				}
				break;
			}
			
			// set the current object as the next object in the object manager
			WowObject tmpObject = currentObject;
			currentObject.Clear();
			try
			{
				currentObject.objBaseAddress = wowMem.MemReadUInt((tmpObject.objBaseAddress + WowPointers::ObjectManager::NEXTOBJECTOFFSET));
			}
			catch (int e)
			{
				std::cout << "Error: ScanObjLight() -> NextObjectOffset: " << e << std::endl;
			}
		}
		// Bei targetwechsel wird die ganze tabelle mit den vector3d gelöscht
		if (target.guid != lastTarget.guid)
		{
			target.lastVector3d.clear();
			target.predictLocObj.targetLastVectorCheck = 0.0;
		}
		myLog.cout();


		//cout << "base: " << hex << target.objBaseAddress << " " << target.guid << "   " << dec << target.xPos << " " << target.yPos << " " << target.zPos << endl;
		//cout << "-------------------" << endl;



		/*WowVector3d tempVector = calculateVector3dForecast(1500);*/
		
		//std::cout << "forecast:    x: " << tempVector.x << "\ty: " << tempVector.y << endl;
		//WowObject distLocationTarget = target;
		//if (localPlayer.targetGuid == 0)
		//{
		//	distLocationTarget = localPlayer;
		//}
		//target.distance = round((localPlayer.vector3d.distance(distLocationTarget.vector3d)) * 100) / 100;
		//std::cout << "distTarget: " << distLocationTarget.distance << "\t distForecast: " << localPlayer.vector3d.distance(tempVector) << endl;

	}




}

void WowHelpers::scanObj()
{
	lastTarget = target;
	if (lastScanObj + blockScanObj < getTime())
	{
		lastScanObj = getTime();

		cachedUnitlist.clear();
		// set our counter variable to 0 so we can begin counting the objects
		int totalWowObjects = 0;
		WowObject currentObject;
		// set our current object as the first in the object manager
		currentObject.objBaseAddress = firstObject;
		try
		{
			localPlayer.targetGuid = wowMem.MemReadULong(base + WowPointers::StaticAddresses::CURRENTTARGETGUID);
			target.guid = localPlayer.targetGuid;
		}
		catch (int e)
		{
			std::cout << "Error read CURRENTTARGETGUID" << e << std::endl;
		}
		while ((currentObject.objBaseAddress & 1) == 0)
		{
			//cout << hex << currentObject.objBaseAddress << endl;
			// type independent informations
			try
			{
				totalWowObjects++;
				currentObject.guid = wowMem.MemReadULong((currentObject.objBaseAddress + WowPointers::WowObjectData::GUID));
				//DWORD x = currentObject.objBaseAddress + WowPointers::WowObjectData::GUID;
			}
			catch (int e)
			{
				std::cout << "Error read currentObject.objBaseAddress + WowPointers::WowObjectData::GUID" << e << std::endl;
			}
			if (currentObject.guid != 0)
			{
				try
				{
					//Log.Print("List: " + currentObject.Guid);
					currentObject.type = (short)(wowMem.MemReadUInt((currentObject.objBaseAddress + WowPointers::WowObjectData::TYPE)));
					switch (currentObject.type)
					{

					case (short)WowPointers::WoWObjectTypes::OT_UNIT: // an npc
						currentObject.castSpell = wowMem.MemReadUInt((currentObject.objBaseAddress + WowPointers::WowObjectData::CASTSPELL));
						currentObject.unitFieldsAddress = wowMem.MemReadUInt((currentObject.objBaseAddress + WowPointers::WowObjectData::DATAPTR));
						currentObject.channelSpell = wowMem.MemReadUInt((currentObject.unitFieldsAddress + Descriptors::WowUnitFields::CHANNELSPELL));
						currentObject.health = wowMem.MemReadUInt((currentObject.unitFieldsAddress + Descriptors::WowUnitFields::HEALTH));
						currentObject.maxHealth = wowMem.MemReadUInt((currentObject.unitFieldsAddress + Descriptors::WowUnitFields::MAXHEALTH));
						currentObject.healthPercent = (unsigned int)floor(((double)currentObject.health / (double)currentObject.maxHealth * 100));
						currentObject.mana = wowMem.MemReadUInt((currentObject.unitFieldsAddress + Descriptors::WowUnitFields::MANA));
						currentObject.maxMana = wowMem.MemReadUInt((currentObject.unitFieldsAddress + Descriptors::WowUnitFields::MAXMANA));
						currentObject.manaPercent = (unsigned int)floor(((double)currentObject.mana / (double)currentObject.maxMana * 100));
						// //CurrentObject.SummonedBy = wowMem.ReadUInt64((CurrentObject.UnitFieldsAddress + (uint)Descriptors.WoWUnitFields.SummonedBy));
						currentObject.factionTemplate = wowMem.MemReadUInt((currentObject.unitFieldsAddress + Descriptors::WowUnitFields::FACTIONTEMPLATE));
						//currentObject.factionOffset = wowMem.MemReadUInt((currentObject.unitFieldsAddress + Descriptors::WowUnitFields::FACTIONOFFSET));
						currentObject.level = wowMem.MemReadUInt((currentObject.unitFieldsAddress + Descriptors::WowUnitFields::LEVEL));
						//CurrentObject.Dodged = wowMem.ReadUInt((CurrentObject.UnitFieldsAddress + (uint)Descriptors.WoWUnitFields.Dodged));
						currentObject.xPos = wowMem.MemReadFloat((currentObject.objBaseAddress + WowPointers::WowObjectData::X));
						currentObject.yPos = wowMem.MemReadFloat((currentObject.objBaseAddress + WowPointers::WowObjectData::Y));
						currentObject.zPos = wowMem.MemReadFloat((currentObject.objBaseAddress + WowPointers::WowObjectData::Z));						
						// //Pointers.PlayerClass.TryGetValue(wowMem.ReadByte((CurrentObject.ObjBaseAddress + (uint)Pointers.WowObject.PlayerClass)), out CurrentObject.Class);
						currentObject.rotation = wowMem.MemReadFloat((currentObject.objBaseAddress + WowPointers::WowObjectData::FACING));
						//CurrentObject.PlayerIsFacingTo = GetFacingToUnit(CurrentObject);
						currentObject.vector3d.set(currentObject.xPos, currentObject.yPos, currentObject.zPos);
						//currentObject.distance = round((localPlayer.vector3d.distance(currentObject.vector3d))*100)/100;
						currentObject.unitFlags = wowMem.MemReadUInt((currentObject.unitFieldsAddress + Descriptors::WowUnitFields::FLAGS));
						currentObject.npcFlags = wowMem.MemReadUInt((currentObject.unitFieldsAddress + Descriptors::WowUnitFields::UNITNPCFLAGS));
						currentObject.dynamicFlags = wowMem.MemReadUInt(currentObject.unitFieldsAddress + Descriptors::WowUnitFields::DYNAMICFLAGS);
						currentObject.isInCombat = isInCombat(currentObject.unitFlags);
						// //CurrentObject.IsHostile = HostileFaction[CurrentObject.FactionTemplate];
						//currentObject.isHostile = wowMem.MemReadUInt((currentObject.unitFieldsAddress + Descriptors::WowUnitFields::UNITNPCFLAGS));
						currentObject.name = mobNameFromBaseAddr(currentObject.objBaseAddress);
						// //DecodeUnitFlags(CurrentObject);
						// //CurrentObject.Target = new WowObject();
						currentObject.targetGuid = wowMem.MemReadULong((currentObject.unitFieldsAddress + Descriptors::WowUnitFields::TARGET));

						cachedUnitlist.push_back(currentObject);
						break;

					case (short)WowPointers::WoWObjectTypes::OT_PLAYER: // a player
						currentObject.castSpell = wowMem.MemReadUInt((currentObject.objBaseAddress + WowPointers::WowObjectData::CASTSPELL));
						currentObject.guidOfAutoAttackTarget = wowMem.MemReadULong((currentObject.objBaseAddress + WowPointers::WowObjectData::GUIDOFAUTOATTACKTARGET));
						currentObject.autoShoot = wowMem.MemReadByte((currentObject.objBaseAddress + WowPointers::WowObjectData::AUTOSHOOT));
						currentObject.unitFieldsAddress = wowMem.MemReadUInt((currentObject.objBaseAddress + WowPointers::WowObjectData::DATAPTR));
						currentObject.channelSpell = wowMem.MemReadUInt((currentObject.unitFieldsAddress + Descriptors::WowUnitFields::CHANNELSPELL));
						currentObject.health = wowMem.MemReadUInt((currentObject.unitFieldsAddress + Descriptors::WowUnitFields::HEALTH));
						currentObject.maxHealth = wowMem.MemReadUInt((currentObject.unitFieldsAddress + Descriptors::WowUnitFields::MAXHEALTH));
						currentObject.healthPercent = (unsigned int)floor(((double)currentObject.health / (double)currentObject.maxHealth * 100));
						currentObject.mana = wowMem.MemReadUInt((currentObject.unitFieldsAddress + Descriptors::WowUnitFields::MANA));
						currentObject.maxMana = wowMem.MemReadUInt((currentObject.unitFieldsAddress + Descriptors::WowUnitFields::MAXMANA));
						currentObject.manaPercent = (unsigned int)floor(((double)currentObject.mana / (double)currentObject.maxMana * 100));
						currentObject.rage = (unsigned int)floor((double)wowMem.MemReadUInt((currentObject.unitFieldsAddress + Descriptors::WowUnitFields::RAGE)) / 10);
						currentObject.energy = wowMem.MemReadUInt((currentObject.unitFieldsAddress + Descriptors::WowUnitFields::ENERGY));
						currentObject.factionTemplate = wowMem.MemReadUInt((currentObject.unitFieldsAddress + Descriptors::WowUnitFields::FACTIONTEMPLATE));
						//currentObject.factionOffset = wowMem.MemReadUInt((currentObject.unitFieldsAddress + Descriptors::WowUnitFields::FACTIONOFFSET));
						currentObject.level = wowMem.MemReadUInt((currentObject.unitFieldsAddress + Descriptors::WowUnitFields::LEVEL));
						//currentObject.Dodged = wowMem.MemReadUInt((currentObject.unitFieldsAddress + Descriptors::WowUnitFields::Dodged));
						currentObject.xPos = wowMem.MemReadFloat((currentObject.objBaseAddress + WowPointers::WowObjectData::X));
						currentObject.yPos = wowMem.MemReadFloat((currentObject.objBaseAddress + WowPointers::WowObjectData::Y));
						currentObject.zPos = wowMem.MemReadFloat((currentObject.objBaseAddress + WowPointers::WowObjectData::Z));
						currentObject.playerClass = Constants::PlayerClass::CLASSES[(unsigned int)(wowMem.MemReadByte((currentObject.objBaseAddress + WowPointers::WowObjectData::PLAYERCLASS)))];
						// // Pointers.PlayerClass.TryGetValue(wowMem.ReadByte((currentObject.objBaseAddress + WowPointers::WowObjectData::PLAYERCLASS)), out currentObject.Class);
						currentObject.rotation = wowMem.MemReadFloat((currentObject.objBaseAddress + WowPointers::WowObjectData::FACING));
						currentObject.movementFlags = wowMem.MemReadUInt((currentObject.unitFieldsAddress + Descriptors::WowUnitFields::MOVEMENTFLAGS));
						//currentObject.PlayerIsFacingTo = GetFacingToUnit(currentObject);
						currentObject.vector3d.set(currentObject.xPos, currentObject.yPos, currentObject.zPos);
						//currentObject.Distance = 0;
						currentObject.unitFlags = wowMem.MemReadUInt((currentObject.unitFieldsAddress + Descriptors::WowUnitFields::FLAGS));
						currentObject.dynamicFlags = wowMem.MemReadUInt(currentObject.unitFieldsAddress + Descriptors::WowUnitFields::DYNAMICFLAGS);
						currentObject.isInCombat = isInCombat(currentObject.unitFlags);
						// // currentObject.isHostile = IsHostile(currentObject);
						currentObject.name = playerNameFromGuid(currentObject.guid);
						// //DecodeUnitFlags(currentObject);
						// //currentObject.target = new WowObject();
						currentObject.targetGuid = wowMem.MemReadULong((currentObject.unitFieldsAddress + Descriptors::WowUnitFields::TARGET));
						//std::cout << currentObject.name << "  ---- " << currentObject.targetGuid << std::endl;
						//std::cout << currentObject.name << "\t pos" << currentObject.vector3d.x << ":" << currentObject.vector3d.y << ":" << currentObject.vector3d.z << std::endl;
						cachedUnitlist.push_back(currentObject);
						break;

					case (short)WowPointers::WoWObjectTypes::OT_GAMEOBJ:
						currentObject.xPos = wowMem.MemReadFloat((currentObject.objBaseAddress + WowPointers::WowObjectData::GAMEOBJECTX));
						currentObject.yPos = wowMem.MemReadFloat((currentObject.objBaseAddress + WowPointers::WowObjectData::GAMEOBJECTY));
						currentObject.zPos = wowMem.MemReadFloat((currentObject.objBaseAddress + WowPointers::WowObjectData::GAMEOBJECTZ));
						//CurrentObject.PlayerIsFacingTo = GetFacingToUnit(CurrentObject);
						currentObject.vector3d.set(currentObject.xPos, currentObject.yPos, currentObject.zPos);
						//CurrentObject.Distance = Math.Round((LocalPlayer.vector3d.Distance(CurrentObject.vector3d)), 2);
						currentObject.unitFieldsAddress = wowMem.MemReadUInt((currentObject.objBaseAddress + WowPointers::WowObjectData::DATAPTR));
						currentObject.name = itemNameFromBaseAddr(currentObject.objBaseAddress);
						currentObject.gameObjectType = itemTypeFromBaseAddr(currentObject.objBaseAddress);
						cachedUnitlist.push_back(currentObject);
						break;

					case (short)WowPointers::WoWObjectTypes::OT_ITEM:
						currentObject.unitFieldsAddress = wowMem.MemReadUInt((currentObject.objBaseAddress + WowPointers::WowObjectData::DATAPTR));
						currentObject.itemId = wowMem.MemReadUInt((currentObject.unitFieldsAddress + Descriptors::ItemFields::itemId));
						currentObject.stacks = wowMem.MemReadUInt((currentObject.unitFieldsAddress + Descriptors::ItemFields::itemStackCount));
						//currentObject.itemOwner = wowMem.MemReadULong((currentObject.unitFieldsAddress + Descriptors::ItemFields::ITEM_FIELD_OWNER));
						cachedUnitlist.push_back(currentObject);
						break;
					}
					

				}
				catch (int e)
				{

					std::cout << "ScanObj() -> switch/case: " << e << std::endl;
				}
				//cout << "objBase: " << hex << currentObject.objBaseAddress << "\tid: "  << currentObject.guid << "\tname: " << currentObject.name << endl;
			}

			// set the current object as the next object in the object manager
			WowObject tmpObject = currentObject;
			currentObject.Clear();
			try
			{
				currentObject.objBaseAddress = wowMem.MemReadUInt((tmpObject.objBaseAddress + WowPointers::ObjectManager::NEXTOBJECTOFFSET));
			}
			catch (int e)
			{
				std::cout << "Error: ScanObj() -> NextObjectOffset: " << e << std::endl;
			}
		}
		// Bei targetwechsel wird die ganze tabelle mit den vector3d gelöscht
		if (target.guid != lastTarget.guid)
		{
			target.lastVector3d.clear();
			target.predictLocObj.targetLastVectorCheck = 0.0;
		}
		try
		{
			for (WowObject &listObj : cachedUnitlist)
			{
				//if (listObj.type == 1)
				//{

				//	cout << "objBase: " << hex << listObj.objBaseAddress << "\tid: " << dec << listObj.itemId << "   stacks: " << listObj.stacks <<  endl;
				//}

				// Wir brauchen eine temporäre Kopie vom predictLocObj und tmpLastVector3d vom Localplayer, sonst ist unser forecast/predictObj nach dem "localPlayer = listObj" leer, 
				Vector3dLog tmpPredictLocObj = localPlayer.predictLocObj;
				std::map<unsigned long, WowVector3d> tmpLastVector3d = localPlayer.lastVector3d;
				if (listObj.guid == localPlayer.guid)
				{
					if (localPlayer.xPos != listObj.xPos || localPlayer.yPos != listObj.yPos || localPlayer.zPos != listObj.zPos)
					{
						listObj.isMoving = true;
					}
					localPlayer = listObj; // Copy whole listObj to LocalPlayer
				}
				localPlayer.predictLocObj = tmpPredictLocObj;
				localPlayer.lastVector3d = tmpLastVector3d;

				// calculate distance for all Types (3,4,5 -> Player, Npc, Gameobj)
				listObj.distance = (round((localPlayer.vector3d.distance(listObj.vector3d)) * 100) / 100);
				// scan the auras of unit only i unit is near
				if (listObj.distance <= 30 && (listObj.type == (short)WowPointers::WoWObjectTypes::OT_UNIT || listObj.type == (short)WowPointers::WoWObjectTypes::OT_PLAYER))
				{

					listObj.dodged = wowMem.MemReadUInt((currentObject.unitFieldsAddress + Descriptors::WowUnitFields::DODGED));
					listObj.buffList = getUnitBuffs(listObj.objBaseAddress);
					listObj.debuffList = getUnitDebuffs(listObj.objBaseAddress);
					listObj.hasBreakableCc = from(listObj.debuffList)
						>> any([&](Auras const & o) {return  (o.name == "Polymorph" || o.name == "Sap" || o.name == "Blind"); })
						;				
				}
				else
				{
					listObj.dodged = 0;
					listObj.buffList.clear();
					listObj.debuffList.clear();
					listObj.hasBreakableCc = false;
				}
			}
		}
		catch (int e)
		{
			std::cout << "ScanObj() -> WowObject listObj in CachedUnitlist: " << e << std::endl;
		}
		
	}
}

/*
A GUID can be thought of as being composed of multiple pieces of data. Consider "0xAABCCCDDDDEEEEEE", where:

AA
unknown; Possible battlegroup identifier when used for players.
B
unit type, mask with 0x7 to get: 0 for players, 1 for world objects, 3 for NPCs, 4 for permanent pets, 5 for vehicles. Temporary pets (Treants, Spirit Wolves, Water Elemental, Mirror Images, and Ghouls) are considered NPCs (3), even if talents or glyphs prevent them from expiring.
NOTE: According to my personal analysis, I found that the number stands for players should be 8 instead of 0. I don't know if this is a newly update.
NOTE: On the server Alexstraza I have seen the 1st 3 numbers be 040, meaning you must mask with 0x7 and 0=player, not 8.
CORRECTION: If you have masked with 0x7 then the result CANNOT be 8. Masking 8 with 0x7 gives 0. It seems reasonable to conclude that the original poster is correct.
CCC
If the unit is a pet, CCCDDDD forms a unique ID for the pet based on creation order; if a world object, CCCDDDD is the object ID; otherwise unknown.
DDDD
If the unit is an NPC, this is the hexadecimal representation of the NPC id.
EEEEEE
If the unit is a player, this is a unique identifier based on creation order. Otherwise, this is a spawn counter based on spawn order.
(For players it can be 7 digits long, so that its DEEEEEE)


FÜr den Breakpoint 493540  bekommt ECX eine gekürzte GUID: AABEEEEE

*/

#include "stdafx.h"
#include "InjectAsm.h"



InjectAsm::InjectAsm(const WowHelpers &wowHelper)
{
	// Set privilegs for my app to manipulate memory
	SetDebugPrivileges();
	DWORD pId;
	GetWindowThreadProcessId(wowHelper.wowMem.windowHandle, &pId);
	hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pId);
	initialized = true;
	std::cout << "Constructor InjectAsm" << std::endl;
}
InjectAsm::InjectAsm()
{
	initialized = false;
	std::cout << "Default Constructor InjectAsm" << std::endl;
}


InjectAsm::~InjectAsm()
{
}
BOOL InjectAsm::SetDebugPrivileges()
{
	BOOL               bRET = FALSE;
	TOKEN_PRIVILEGES   tp;
	HANDLE             hToken;

	if (LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tp.Privileges[0].Luid))
	{
		if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken))
		{
			if (hToken != INVALID_HANDLE_VALUE)
			{
				tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
				tp.PrivilegeCount = 1;
				if (AdjustTokenPrivileges(hToken, FALSE, &tp, 0, 0, 0))
					bRET = TRUE;
				CloseHandle(hToken);
			}
		}
	}
	return bRET;
}

bool InjectAsm::setDoAction(const byte &d)
{
	// d can be 0 = do nothing, 1 = do castSpellByName, 2 = UseItem, 3 = place Spell, 4 = click Action Button, 5 = Pet Action (0,1,2 werden über den Funktionsaufruf gesetzt. 3 wird im Assembler gesetzt.
	SIZE_T bytes_read = 0;
	SIZE_T bytes_written = 0;
	DWORD doSpellCastAddress = 0x15F4E8;
	byte opCodesDoSpellCast;
	opCodesDoSpellCast = d;
	// Am Anfang der Codecave gibt es eine Bedingung mit anshcließenden Jump (JE), welcher ausgeführt wird, wenn ein Byte an einer Adresse auf 1 steht. Dieses Byte wird hier geschrieben.
	bool IsDoSpellCast = WriteProcessMemory(hProc, (void *)((int)doSpellCastAddress), &opCodesDoSpellCast, (size_t)1, &bytes_written);
	if (!IsDoSpellCast)
	{
		std::cerr << "Couldn't write process memory(doSpellCast): [" << GetLastError() << "]" << std::endl;
		return false;
	}
	else
	{
		//std::cout << "doSpellCast written \t(" << bytes_written << " of " << 1 << ")" << std::endl;
		return true;
	}
}

bool InjectAsm::setSpellPlacementAction(const byte &d)
{
	// d can be 0 = no Placement needed, 3 = placement needed, if d is 3 then parameter for "place spell" in setDoAction must be 3 as well. Because 15f4e8 will get the value of 15f4ec.
	SIZE_T bytes_read = 0;
	SIZE_T bytes_written = 0;
	DWORD spellPlacementAddress = 0x15F4EC;
	byte opCodeSpellPlacement;
	opCodeSpellPlacement = d;

	// Am Anfang der Codecave gibt es eine Bedingung mit anshcließenden Jump (JE), welcher ausgeführt wird, wenn ein Byte an einer Adresse auf 1 steht. Dieses Byte wird hier geschrieben.
	bool IsSpellPlacement = WriteProcessMemory(hProc, (void *)((int)spellPlacementAddress), &opCodeSpellPlacement, (size_t)1, &bytes_written);
	if (!IsSpellPlacement)
	{
		std::cerr << "Couldn't write process memory(setSpellPlacementAction): [" << GetLastError() << "]" << std::endl;
		return false;
	}
	else
	{
		//std::cout << "doSpellCast written \t(" << bytes_written << " of " << 1 << ")" << std::endl;
		return true;
	}
	//std::cout << "<space> setSpellPlacementAction" << std::endl;
	//std::cin.get();
}

bool InjectAsm::setSpellLocation(const WowVector3d &loc)
{

	float x = loc.x;
	float y = loc.y;
	float z = loc.z;
	// d can be 0 = do nothing, 1 = do castSpellByName, 2 = UseItem, 3 = place Spell (0,1,2 werden über den Funktionsaufruf gesetzt. 3 wird im Assembler gesetzt.
	SIZE_T bytes_read = 0;
	SIZE_T bytes_written = 0;
	std::vector<byte> opCodesLocation;
	DWORD locYAddress = 0x15F4F8;

	union { float a; unsigned char b[4]; } locFloatToByte;

	locFloatToByte.a = y;
	for (int i = 0; i < 4; i++)
	{
		opCodesLocation.push_back(locFloatToByte.b[i]);
	}
	locFloatToByte.a = x;
	for (int i = 0; i < 4; i++)
	{
		opCodesLocation.push_back(locFloatToByte.b[i]);
	}
	locFloatToByte.a = z;
	for (int i = 0; i < 4; i++)
	{
		opCodesLocation.push_back(locFloatToByte.b[i]);
	}

	//std::cout << "yxz: " << y << " " << x << " " << z << std::endl;
	//for (auto &x : opCodesLocation)
	//{
	//	std::cout << std::hex << (int)x << " ";
	//}
	//std::cout << std::endl;
	//std::cout << "<space> setSpellLocation" << std::endl;
	//std::cin.get();
	size_t sizeof_bytesOfLocation = opCodesLocation.capacity();
	bool IsSetLocation = WriteProcessMemory(hProc, (void *)((int)locYAddress), &opCodesLocation[0], sizeof_bytesOfLocation, &bytes_written);
	if (!IsSetLocation)
	{
		std::cerr << "Couldn't write process memory(setSpellLocation): [" << GetLastError() << "]" << std::endl;
		return false;
	}
	else
	{
		//std::cout << "setSpellLocation written \t(" << bytes_written << " of " << 1 << ")" << std::endl;
		return true;
	}
}



bool InjectAsm::setUseItemPtr(const DWORD &p)
{
	// at 0x15F4F0 we will place the pointer for the using ItemObject
	SIZE_T bytes_read = 0;
	SIZE_T bytes_written = 0;
	DWORD useItemPtrAddress = 0x15F4F0;
	std::vector<byte> opCodeUseItemPtr;


	std::vector<byte> bytesOfuseItemPtr = WowHelpers::ToBytePattern(p); //converts the int/long-value to a array of bytes
	bytesOfuseItemPtr.shrink_to_fit(); // remove unecessary allocations of empty elements
	
	for (auto &x : bytesOfuseItemPtr)
	{
		opCodeUseItemPtr.push_back(x);
	}
	//std::cout << "<space> setUseItemPtr" << std::endl;
	//std::cin.get();

	// Am Anfang der Codecave gibt es eine Bedingung mit anshcließenden Jump (JE), welcher ausgeführt wird, wenn ein Byte an einer Adresse auf 1 steht. Dieses Byte wird hier geschrieben.
	bool IsSpellPlacement = WriteProcessMemory(hProc, (void *)((int)useItemPtrAddress), &opCodeUseItemPtr[0], (size_t)4, &bytes_written);
	if (!IsSpellPlacement)
	{
		std::cerr << "Couldn't write process memory(setUseItemPtr): [" << GetLastError() << "]" << std::endl;
		return false;
	}
	else
	{
		//std::cout << "doSpellCast written \t(" << bytes_written << " of " << 1 << ")" << std::endl;
		return true;
	}
}
bool InjectAsm::setSpell(const std::string &spellName)
{
	SIZE_T bytes_read = 0;
	SIZE_T bytes_written = 0;
	DWORD spellNameAddress = 0x15F448;
	// ------------ create ASM: opcodes for Spellname ------------------------
	// ----------------------------------------------------------

	std::vector<char> opCodesSpellName(spellName.begin(), spellName.end());
	opCodesSpellName.push_back('\0');

	//std::cout << &spellName;
	//std::cout << "SpellName: ";
	//for (auto &x : opCodesSpellName)
	//{
	//	std::cout << std::hex << (int)x << " ";
	//}
	//std::cout << std::endl;
	size_t sizeof_bytesOfSpellName = opCodesSpellName.capacity();
	// ---------/ end create ASM: Spellname ------------------

	// Schreibt den Spellnamen in den Speicher
	bool IsAsmSpellNameWritten = WriteProcessMemory(hProc, (void *)((int)spellNameAddress), &opCodesSpellName[0], sizeof_bytesOfSpellName, &bytes_written);
	if (!IsAsmSpellNameWritten)
	{
		std::cerr << "Couldn't write process memory(spell name): [" << GetLastError() << "]" << std::endl;
		return false;
	}
	else
	{
		//std::cout << "Spell name written \t(" << bytes_written << " of " << sizeof_bytesOfSpellName << ")" << std::endl;
		return true;
	}
}

bool InjectAsm::setPetAction(const byte &action)
{
	SIZE_T bytes_read = 0;
	SIZE_T bytes_written = 0;
	DWORD petActionAdress = 0x15F518;
	// ------------ create ASM: opcodes for ActionButton ------------------------
	// ----------------------------------------------------------

	std::vector<byte> opCodesPetAction;
	opCodesPetAction.push_back(action);

	//std::cout << &spellName;
	//std::cout << "SpellName: ";
	//for (auto &x : opCodesSpellName)
	//{
	//	std::cout << std::hex << (int)x << " ";
	//}
	//std::cout << std::endl;
	size_t sizeof_bytesOfPetAction = opCodesPetAction.capacity();
	// ---------/ end create ASM: Spellname ------------------

	// Schreibt den Spellnamen in den Speicher
	bool IsAsmActionButtonWritten = WriteProcessMemory(hProc, (void *)((int)petActionAdress), &opCodesPetAction[0], sizeof_bytesOfPetAction, &bytes_written);
	if (!IsAsmActionButtonWritten)
	{
		std::cerr << "Couldn't write process memory(setActionButton): [" << GetLastError() << "]" << std::endl;
		return false;
	}
	else
	{
		//std::cout << "Spell name written \t(" << bytes_written << " of " << sizeof_bytesOfSpellName << ")" << std::endl;
		return true;
	}
}

bool InjectAsm::setActionButton(const byte &actionBtn)
{
	SIZE_T bytes_read = 0;
	SIZE_T bytes_written = 0;
	DWORD actionButtonAdress = 0x15F510;
	// ------------ create ASM: opcodes for ActionButton ------------------------
	// ----------------------------------------------------------

	std::vector<byte> opCodesActionButton;
	opCodesActionButton.push_back(actionBtn);

	//std::cout << &spellName;
	//std::cout << "SpellName: ";
	//for (auto &x : opCodesSpellName)
	//{
	//	std::cout << std::hex << (int)x << " ";
	//}
	//std::cout << std::endl;
	size_t sizeof_bytesOfActionButton = opCodesActionButton.capacity();
	// ---------/ end create ASM: Spellname ------------------

	// Schreibt den Spellnamen in den Speicher
	bool IsAsmActionButtonWritten = WriteProcessMemory(hProc, (void *)((int)actionButtonAdress), &opCodesActionButton[0], sizeof_bytesOfActionButton, &bytes_written);
	if (!IsAsmActionButtonWritten)
	{
		std::cerr << "Couldn't write process memory(setActionButton): [" << GetLastError() << "]" << std::endl;
		return false;
	}
	else
	{
		//std::cout << "Spell name written \t(" << bytes_written << " of " << sizeof_bytesOfSpellName << ")" << std::endl;
		return true;
	}
}

bool InjectAsm::setTarget(const unsigned long long &guid)
{
	SIZE_T bytes_read = 0;
	SIZE_T bytes_written = 0;
	//DWORD targetGuid1Address = 0x7Fee5C;
	//DWORD targetGuid2Address = 0x7Fee64;
	DWORD targetGuid1Address = 0x7FeFC0;
	DWORD targetGuid2Address = 0x7FeFC8;
	WowHelpers::splitGuid(guid, targetGuid2, targetGuid1);

	// ------------ create ASM: opcodes for GUID ------------------------
	// ----------------------------------------------------------

	//std::cout << "tguid3  1: " << targetGuid1 << std::endl;
	//std::cout << "tguid3: 2: " << targetGuid2 << std::endl;

	std::vector<byte> bytesOftargetGuid1 = WowHelpers::ToBytePattern(targetGuid1); //converts the int/long-value to a array of bytes
	std::vector<byte> bytesOftargetGuid2 = WowHelpers::ToBytePattern(targetGuid2);

	//cout << "<Guid4: " << to_string(targetGuid) << "\t " << hex << targetGuid << "\t ";
	//for (auto &x : bytesOftargetGuid)
	//{
	//	cout << hex << (int)x << " ";
	//}
	//cout << endl;

	size_t sizeof_bytesOftargetGuid1 = bytesOftargetGuid1.capacity();
	size_t sizeof_bytesOftargetGuid2 = bytesOftargetGuid2.capacity();
	//std::cout << "hProc: " << hProc << std::endl;
	//std::cout << "targetGuid1Address: " << targetGuid1Address << std::endl;
	//std::cout << "sizeof_bytesOftargetGuid1: " << sizeof_bytesOftargetGuid1 << std::endl;
	// ---------/ end create ASM: Spellname ------------------

	// Schreibt den 1. Teil der Guid kurz nach der Codecave in den Speicher
	bool IsAsmTargetGuid1Written = WriteProcessMemory(hProc, (void *)((int)targetGuid1Address), &bytesOftargetGuid1[0], sizeof_bytesOftargetGuid1, &bytes_written);
	if (!IsAsmTargetGuid1Written)
	{
		std::cerr << "Couldn't write process memory(Guid1): [" << GetLastError() << "]" << std::endl;
		return false;
	}
	else
	{
		//std::cout << "Guid1 written \t(" << bytes_written << " of " << sizeof_bytesOftargetGuid1 << ")" << std::endl;

		// Schreibt den 2. Teil der Guid kurz nach der Codecave in den Speicher
		bool IsAsmTargetGuid2Written = WriteProcessMemory(hProc, (void *)((int)targetGuid2Address), &bytesOftargetGuid2[0], sizeof_bytesOftargetGuid2, &bytes_written);
		if (!IsAsmTargetGuid2Written)
		{
			std::cerr << "Couldn't write process memory(Guid2): [" << GetLastError() << "]" << std::endl;
			return false;
		}
		else
		{
			//std::cout << "Guid2 written \t(" << bytes_written << " of " << sizeof_bytesOftargetGuid2 << ")" << std::endl;
			return true;
		}
	}

}

bool InjectAsm::changeEndScene(const DWORD &hookAddress)
{
	std::cout << "hProc: " << hProc << std::endl;
	SIZE_T bytes_read = 0;
	SIZE_T bytes_written = 0;

	// ------------ create ASM: Hook the Endscene ------------
	// ----------------------------------------------------------
	std::vector<byte> opCodesHookAddress; // Detour mit target auf id "B" (eigener char)

										  // first section of ASM-Code till jump to Codecave
	opCodesHookAddress.push_back(0xE9);

	// adding jmp to codecave to ASM-Code
	DWORD tmpJmpAddress2 = (codeCave - hookAddress) - 5;
	std::vector<byte> bytesOfJmpToCodecave = WowHelpers::ToBytePattern(tmpJmpAddress2); //converts the int/long-value to a array of bytes
	bytesOfJmpToCodecave.shrink_to_fit(); // remove unecessary allocations of empty elements
	for (auto &x : bytesOfJmpToCodecave)
	{
		opCodesHookAddress.push_back(x);
	}

	// second section of ASM-Code after jump to codecave
	byte arrayTemp3[16] = { 0x6A, 0xFF, 0x68, 0xFC, 0xAD, 0xA3, 0x66, 0x64, 0xA1 }; // create array of asm code after the jmp
	opCodesHookAddress.insert(opCodesHookAddress.end(), arrayTemp3, arrayTemp3 + 9); // ... and push them into the vector

	opCodesHookAddress.shrink_to_fit(); // remove unecessary allocations of empty elements
	size_t sizeof_opCodesHookAddress = opCodesHookAddress.capacity();
	// ---------/ end create ASM: Hook the Endscene ------------

	for (auto &x : opCodesHookAddress)
	{
		std::stringstream tempSs;
		if (x <= 0xf)
		{
			tempSs << "0";
		}
		tempSs << std::hex << (int)x;
		std::cout << tempSs.str() << " ";
	}
	std::cout << std::endl;
	//std::cout << "<space> write endscene" << std::endl;
	//std::cin.get();



	// Schreibt endscene
	if (!WriteProcessMemory(hProc, (void *)((int)hookAddress), &opCodesHookAddress[0], sizeof_opCodesHookAddress, &bytes_written))
	{
		std::cerr << "Couldn't write process memory (HookAddress): [" << GetLastError() << "]" << std::endl;
		return false;
	}
	else
	{
		std::cout << "HookAddress written \t(" << bytes_written << " of " << sizeof_opCodesHookAddress << ")" << std::endl;
		return true;
	}


}
bool InjectAsm::buildPetActionCave(const DWORD &hookAddress)
{
	//std::cout << "hProc: " << hProc << std::endl;
	SIZE_T bytes_read = 0;
	SIZE_T bytes_written = 0;


	if (hProc) {
		std::vector<byte> opCodesCodeCave;
		// thats the petActionCodeCave
		byte arrayTemp1[193] = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0xC7, 0x05, 0xE8, 0xF4, 0x15, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x8B, 0x35, 0x18, 0xF5, 0x15, 0x00, 0xA1, 0xA4, 0x14, 0xB7, 0x00, 0x8B, 0x0D, 0xA0, 0x14, 0xB7, 0x00, 0x68, 0x09, 0x04, 0x00, 0x00, 0x50, 0x51, 0xBA, 0x68, 0x78, 0x84, 0x00, 0xB9, 0x08, 0x00, 0x00, 0x00, 0xE8, 0x3E, 0x7F, 0xC3, 0xFF, 0x85, 0xC0, 0x74, 0x3E, 0x8B, 0x90, 0x10, 0x01, 0x00, 0x00, 0x8A, 0x82, 0xA3, 0x00, 0x00, 0x00, 0xB9, 0x01, 0x00, 0x00, 0x00, 0x84, 0xC1, 0x75, 0x29, 0x8B, 0x3D, 0xF8, 0xE2, 0xB4, 0x00, 0x8B, 0xD1, 0xE8, 0x48, 0x4C, 0xC6, 0xFF, 0x85, 0xFF, 0x74, 0x18, 0x8B, 0xD6, 0x8D, 0x4D, 0xFC, 0x89, 0x7D, 0xFC, 0xE8, 0xA7, 0xC8, 0xC8, 0xFF, 0x33, 0xC0, 0x5F, 0x5E, 0x8B, 0xE5, 0x5D, 0x8B, 0xE3, 0x5B, 0xC3, 0xA1, 0xA0, 0x14, 0xB7, 0x00, 0x0B, 0x05, 0xA4, 0x14, 0xB7, 0x00, 0x74, 0x2A, 0x83, 0xFE, 0x0A, 0x73, 0x25, 0x8D, 0x0C, 0xB5, 0x38, 0x14, 0xB7, 0x00, 0x33, 0xC0, 0x3B, 0xC8, 0x74, 0x18, 0x8D, 0x55, 0xF8, 0x89, 0x45, 0xF8, 0x89, 0x45, 0xFC, 0xE8, 0x3F, 0xCC, 0xC8, 0xFF, 0xB9, 0xA0, 0x79, 0x84, 0x00, 0xE8, 0x95, 0x7A, 0xC2, 0xFF, 0x61, 0xBD, 0x38, 0xFD, 0x18, 0x00, 0xBC, 0x38, 0xFD, 0x18, 0x00, 0xE9 }; // create array of asm code before the je [endscene]
		opCodesCodeCave.insert(opCodesCodeCave.end(), arrayTemp1, arrayTemp1 + 193); // .. and push it to the vector


		DWORD tmpJeAddress1 = (hookAddress - petActionCodeCave) - 192;
		std::vector<byte> bytesOfJeAddress1 = WowHelpers::ToBytePattern(tmpJeAddress1); //converts the int/long-value to a array of bytes
		bytesOfJeAddress1.shrink_to_fit(); // remove unecessary allocations of empty elements

		std::cout << "jmp1: ";
		for (auto &x : bytesOfJeAddress1)
		{
			std::cout << std::hex << (int)x << " ";
			opCodesCodeCave.push_back(x);
		}
		//std::cout << std::endl;
		//std::cout << "<space>" << std::endl;
		//std::cin.get();

		opCodesCodeCave.shrink_to_fit(); // remove unecessary allocations of empty elements
		size_t sizeof_opCodesCodeCave = opCodesCodeCave.capacity();


		for (auto &x : opCodesCodeCave)
		{
			std::stringstream tempSs;
			if (x <= 0xf)
			{
				tempSs << "0";
			}
			tempSs << std::hex << (int)x;
			std::cout << tempSs.str() << " ";
		}
		std::cout << std::endl;


		// Schreibt die petActionCodeCave bei 0x8304F1 in den Speicher
		bool IsAsmCodeCaveWritten = WriteProcessMemory(hProc, (void *)((int)petActionCodeCave), &opCodesCodeCave[0], sizeof_opCodesCodeCave, &bytes_written);
		if (!IsAsmCodeCaveWritten)
		{
			std::cerr << "Couldn't write process memory (petActionCodeCave): [" << GetLastError() << "]" << std::endl;
			return false;
		}
		else
		{
			//std::cout << "codeCave written \t(" << bytes_written << " of " << sizeof_opCodesCodeCave << ")" << std::endl;
			return true;
		}

	}
}
bool InjectAsm::buildActionButtonCave(const DWORD &hookAddress)
{
	//std::cout << "hProc: " << hProc << std::endl;
	SIZE_T bytes_read = 0;
	SIZE_T bytes_written = 0;


	if (hProc) {
		std::vector<byte> opCodesCodeCave; 
		// thats the ActionButtonCodeCave
		byte arrayTemp1[109] = { 0x60, 0x56, 0xBA, 0x01, 0x00, 0x00, 0x00, 0x8B, 0xF1, 0xE8, 0x82, 0x45, 0xEF, 0xFF, 0x57, 0xBA, 0x01, 0x00, 0x00, 0x00, 0x8B, 0xCE, 0xE8, 0xC5, 0x46, 0xEF, 0xFF, 0xE8, 0x50, 0xB3, 0xC0, 0xFF, 0xA1, 0x10, 0xF5, 0x15, 0x00, 0x8B, 0xF8, 0x6A, 0x00, 0xBA, 0x03, 0x00, 0x00, 0x00, 0x8B, 0xCE, 0x4F, 0xE8, 0x9A, 0x2C, 0xEF, 0xFF, 0x50, 0x6A, 0x00, 0xBA, 0x02, 0x00, 0x00, 0x00, 0x8B, 0xCE, 0xE8, 0x8B, 0x2C, 0xEF, 0xFF, 0xB8, 0x01, 0x00, 0x00, 0x00, 0x8B, 0xD0, 0x8B, 0xCF, 0xE8, 0x4d, 0x6F, 0xCE, 0xFF, 0x5F, 0x33, 0xC0, 0x5E, 0x61, 0xBD, 0x38, 0xFD, 0x18, 0x00, 0xBC, 0x38, 0xFD, 0x18, 0x00, 0xC7, 0x05, 0xE8, 0xF4, 0x15, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE9 }; // create array of asm code before the je [endscene]
		opCodesCodeCave.insert(opCodesCodeCave.end(), arrayTemp1, arrayTemp1 + 109); // .. and push it to the vector


		DWORD tmpJeAddress1 = (hookAddress - actionButtonCodeCave) - 108;
		std::vector<byte> bytesOfJeAddress1 = WowHelpers::ToBytePattern(tmpJeAddress1); //converts the int/long-value to a array of bytes
		bytesOfJeAddress1.shrink_to_fit(); // remove unecessary allocations of empty elements

		std::cout << "jmp1: ";
		for (auto &x : bytesOfJeAddress1)
		{
			std::cout << std::hex << (int)x << " ";
			opCodesCodeCave.push_back(x);
		}
		//std::cout << std::endl;
		//std::cout << "<space>" << std::endl;
		//std::cin.get();

		opCodesCodeCave.shrink_to_fit(); // remove unecessary allocations of empty elements
		size_t sizeof_opCodesCodeCave = opCodesCodeCave.capacity();


		for (auto &x : opCodesCodeCave)
		{
			std::stringstream tempSs;
			if (x <= 0xf)
			{
				tempSs << "0";
			}
			tempSs << std::hex << (int)x;
			std::cout << tempSs.str() << " ";
		}
		std::cout << std::endl;


		// Schreibt die ActionButtonCodeCave bei 7FFE0440 in den Speicher
		bool IsAsmCodeCaveWritten = WriteProcessMemory(hProc, (void *)((int)actionButtonCodeCave), &opCodesCodeCave[0], sizeof_opCodesCodeCave, &bytes_written);
		if (!IsAsmCodeCaveWritten)
		{
			std::cerr << "Couldn't write process memory (actionButtonCodeCave): [" << GetLastError() << "]" << std::endl;
			return false;
		}
		else
		{
			//std::cout << "codeCave written \t(" << bytes_written << " of " << sizeof_opCodesCodeCave << ")" << std::endl;
			return true;
		}

	}
}
bool InjectAsm::buildCodeCave(const DWORD &hookAddress)
{
	//std::cout << "hProc: " << hProc << std::endl;
	SIZE_T bytes_read = 0;
	SIZE_T bytes_written = 0;


	if (hProc) {


		// ------------ create ASM: Codecave ------------------------
		// ----------------------------------------------------------

		std::vector<byte> opCodesCodeCave; // Detour mit target auf id "B" (eigener char)

		// first section of ASM-Code till TargetGuid
		byte arrayTemp1[15] = { 0x55, 0x8B, 0xEC, 0x50, 0xA1, 0xE8, 0xF4, 0x15, 0x00, 0x83, 0xF8, 0x00, 0x58, 0x0F, 0x84 }; // create array of asm code before the je [endscene]
		opCodesCodeCave.insert(opCodesCodeCave.end(), arrayTemp1, arrayTemp1 + 15); // .. and push it to the vector


		DWORD tmpJeAddress1 = (hookAddress - codeCave) - 14;
		std::vector<byte> bytesOfJeAddress1 = WowHelpers::ToBytePattern(tmpJeAddress1); //converts the int/long-value to a array of bytes
		bytesOfJeAddress1.shrink_to_fit(); // remove unecessary allocations of empty elements

		std::cout << "je1: ";
		for (auto &x : bytesOfJeAddress1)
		{
			std::cout << std::hex << (int)x << " ";
			opCodesCodeCave.push_back(x);
		}
		//std::cout << std::endl;
		//std::cout << "<space>" << std::endl;
		//std::cin.get();


		//2. jump-section to jump to useItem 7FEE80
		byte arrayTemp2[16] = { 0x50, 0xA1, 0xE8, 0xF4, 0x15, 0x0, 0x83, 0xF8, 0x02, 0x58, 0x0F, 0x84, 0xC5, 0x0, 0x0, 0x0 }; // create array of asm code before the je [endscene]
		opCodesCodeCave.insert(opCodesCodeCave.end(), arrayTemp2, arrayTemp2 + 16); // .. and push it to the vector


		//2b. jump-section to jump to press Action Button 7FEF40
		byte arrayTemp2b[16] = { 0x50, 0xA1, 0xE8, 0xF4, 0x15, 0x00, 0x83, 0xF8, 0x04, 0x58, 0x0F, 0x84, 0x61, 0x01, 0x00, 0x00 }; // create array of asm code before the je [endscene]
		opCodesCodeCave.insert(opCodesCodeCave.end(), arrayTemp2b, arrayTemp2b + 16); // .. and push it to the vector

		//2c. jump-section to jump to press Action Button 830500
		byte arrayTemp2c[16] = { 0x50, 0xA1, 0xE8, 0xF4, 0x15, 0x00, 0x83, 0xF8, 0x05, 0x58, 0x0F, 0x84, 0x01, 0x17, 0x03, 0x00 }; // create array of asm code before the je [endscene]
		opCodesCodeCave.insert(opCodesCodeCave.end(), arrayTemp2c, arrayTemp2c + 16); // .. and push it to the vector


		//std::cout << std::endl;
		//std::cout << "<space>" << std::endl;
		//std::cin.get();


		//3. jump-section to jump to place spell 7FEED8
		byte arrayTemp3[16] = { 0x50, 0xA1, 0xE8, 0xF4, 0x15, 0x00, 0x83, 0xF8, 0x03, 0x58, 0x0F, 0x84, 0xD9, 0x0, 0x0, 0x0 }; // create array of asm code before the je [endscene]
		opCodesCodeCave.insert(opCodesCodeCave.end(), arrayTemp3, arrayTemp3 + 16); // .. and push it to the vector


		//std::cout << "<space>" << std::endl;
		//std::cin.get();


		//Targetguid1: 0x7FEF40
		//Targetguid2: 0x7FEF48
		// second section of ASM-Code till jump out of the TargetByGuid-Func
		byte arrayTemp4[140] = { 0xC7, 0x05, 0xE8, 0xF4, 0x15, 0x00, 0x00, 0x00, 0x00, 0x00, 0x50, 0x53, 0x51, 0x52, 0x55, 0x54, 0x56, 0x57, 0x51, 0x56, 0xBA, 0x01, 0x00, 0x00, 0x00, 0x89, 0xCE, 0xE8, 0xF1, 0x46, 0xEF, 0xFF, 0x53, 0x8B, 0x1D, 0xC0, 0xEF, 0x7F, 0x00, 0x57, 0x8B, 0x3D, 0xC8, 0xEF, 0x7F, 0x00, 0x6A, 0x00, 0xBA, 0x02, 0x00, 0x00, 0x00, 0x89, 0xF1, 0xE8, 0xD5, 0x2D, 0xEF, 0xFF, 0xBA, 0x01, 0x00, 0x00, 0x00, 0x89, 0xF1, 0xE8, 0x49, 0x48, 0xEF, 0xFF, 0xB8, 0x48, 0xF4, 0x15, 0x00, 0x8D, 0x55, 0xFC, 0x89, 0xC1, 0xE8, 0xFA, 0x4A, 0xCB, 0xFF, 0x85, 0xC0, 0x7C, 0x0C, 0x8B, 0x55, 0xFC, 0x53, 0x57, 0x89, 0xC1, 0xE8, 0x9A, 0x44, 0xCB, 0xFF, 0x5F, 0x5B, 0x31, 0xC0, 0x5E, 0x89, 0xEC, 0x5D, 0x5F, 0x5E, 0x5C, 0x5D, 0x5A, 0x59, 0x5B, 0xA1, 0xEC, 0xF4, 0x15, 0x00, 0xA3, 0xE8, 0xF4, 0x15, 0x00, 0x58, 0xBD, 0x38, 0xFD, 0x18, 0x00, 0xBC, 0x38, 0xFD, 0x18, 0x00, 0xE9 };// create array of asm code after the jz [endscene] to jmp [endscene]
		opCodesCodeCave.insert(opCodesCodeCave.end(), arrayTemp4, arrayTemp4 + 140); // .. and push it to the vector

		DWORD tmpJmpAddress4 = (hookAddress - codeCave) - 222; // calculate the relativ offset of the jmp
		std::vector<byte> bytesOfJmpAddress4 = WowHelpers::ToBytePattern(tmpJmpAddress4); //converts the int/long-value to a array of bytes
		bytesOfJmpAddress4.shrink_to_fit(); // remove unecessary allocations of empty elements
		std::cout << "jmp4: ";
		for (auto &x : bytesOfJmpAddress4)
		{
			std::cout << std::hex << (int)x << " ";
			opCodesCodeCave.push_back(x);
		}
		//std::cout << std::endl;
		//std::cout << "<space>" << std::endl;
		//std::cin.get();


		// fill gap between main section of codecave and useitem section with nop
		byte arrayTemp5[5] = { 0x90, 0x90, 0x90, 0x90, 0x90 };// nop
		opCodesCodeCave.insert(opCodesCodeCave.end(), arrayTemp5, arrayTemp5 + 5); // .. and push it to the vector




		//useItem 007FEE94
		byte arrayTemp6[62] = { 0x60, 0xBB, 0x1C, 0xFB, 0x18, 0x00, 0xBA, 0xF0, 0xFA, 0x18, 0x00, 0xBD, 0x10, 0xFB, 0x18, 0x00, 0xBC, 0xDC, 0xFA, 0x18, 0x00, 0xBF, 0xFF, 0xFF, 0xFF, 0xFF, 0xA1, 0xF0, 0xF4, 0x15, 0x00, 0x89, 0xC1, 0xE8, 0x46, 0x9E, 0xDD, 0xFF, 0x61, 0x50, 0xA1, 0xEC, 0xF4, 0x15, 0x00, 0xA3, 0xE8, 0xF4, 0x15, 0x00, 0x58, 0xBD, 0x38, 0xFD, 0x18, 0x00, 0xBC, 0x38, 0xFD, 0x18, 0x00, 0xE9 }; // ab Beginn (007FEE80) bis zum jmp (zurück zur endscene)
		opCodesCodeCave.insert(opCodesCodeCave.end(), arrayTemp6, arrayTemp6 + 62); // .. and push it to the vector

		DWORD tmpJmpAddress6 = (hookAddress - codeCave) - 293; // calculate the relativ offset of the jmp to endscene
		std::vector<byte> bytesOfJmpAddress6 = WowHelpers::ToBytePattern(tmpJmpAddress6); //converts the int/long-value to a array of bytes
		bytesOfJmpAddress6.shrink_to_fit(); // remove unecessary allocations of empty elements

		std::cout << "jmp6: ";
		for (auto &x : bytesOfJmpAddress6)
		{
			std::cout << std::hex << (int)x << " ";
			opCodesCodeCave.push_back(x);
		}
		//std::cout << std::endl;
		//std::cout << "<space>" << std::endl;
		//std::cin.get();


		// fill gap between main section of codecave and useitem section with nop
		byte arrayTemp7[2] = {  0x90, 0x90 };// nop
		opCodesCodeCave.insert(opCodesCodeCave.end(), arrayTemp7, arrayTemp7 + 2); // .. and push it to the vector




		//0x15F508 Zwischenspeicher gegen Crash für EBX(crash @ 5a17bc)
		//0x15F50C Zwischenspeicher gegen Crash für ESI(crash @ 5a17bc)
		//place spell 007FEED8
		byte arrayTemp8[82] = { 0x89, 0x1D, 0x08, 0xF5, 0x15, 0x00, 0x89, 0x35, 0x0C, 0xF5, 0x15, 0x00, 0x60, 0xB9, 0xF8, 0xF4, 0x15, 0x00, 0xBD, 0x24, 0xFB, 0x18, 0x00, 0xBC, 0xF0, 0xFA, 0x18, 0x00, 0xBE, 0xF8, 0xF4, 0x15, 0x00, 0xE8, 0xF2, 0x71, 0xEE, 0xFF, 0x61, 0xC7, 0x05, 0xEC, 0xF4, 0x15, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC7, 0x05, 0xE8, 0xF4, 0x15, 0x00, 0x00, 0x00, 0x00, 0x00, 0xBD, 0x38, 0xFD, 0x18, 0x00, 0xBC, 0x38, 0xFD, 0x18, 0x00, 0x8B, 0x1D, 0x08, 0xF5, 0x15, 0x00, 0x8B, 0x35, 0x0C, 0xF5, 0x15, 0x00, 0xE9 }; // ab Beginn (007FEE80) bis zum jmp (zurück zur endscene)
		opCodesCodeCave.insert(opCodesCodeCave.end(), arrayTemp8, arrayTemp8 + 82); // .. and push it to the vector

		DWORD tmpJmpAddress8 = (hookAddress - codeCave) - 381; // calculate the relativ offset of the jmp to endscene
		std::vector<byte> bytesOfJmpAddress8 = WowHelpers::ToBytePattern(tmpJmpAddress8); //converts the int/long-value to a array of bytes
		bytesOfJmpAddress8.shrink_to_fit(); // remove unecessary allocations of empty elements
											//std::cout << "jmp: ";
		std::cout << "jmp8: ";
		for (auto &x : bytesOfJmpAddress8)
		{
			std::cout << std::hex << (int)x << " ";
			opCodesCodeCave.push_back(x);
		}
		//std::cout << std::endl;
		//std::cout << "<space>" << std::endl;
		//std::cin.get();




		opCodesCodeCave.shrink_to_fit(); // remove unecessary allocations of empty elements
		size_t sizeof_opCodesCodeCave = opCodesCodeCave.capacity();
		// ---------/ end create ASM: Codecave ------------------


		for (auto &x : opCodesCodeCave)
		{
			std::stringstream tempSs;
			if (x <= 0xf)
			{
				tempSs << "0";
			}
			tempSs << std::hex << (int)x;
			std::cout << tempSs.str() << " ";
		}
		std::cout << std::endl;






		// Schreibt die Codecave bei 0x7FEDAC in den Speicher
		bool IsAsmCodeCaveWritten = WriteProcessMemory(hProc, (void *)((int)codeCave), &opCodesCodeCave[0], sizeof_opCodesCodeCave, &bytes_written);
		if (!IsAsmCodeCaveWritten)
		{
			std::cerr << "Couldn't write process memory (codeCave): [" << GetLastError() << "]" << std::endl;
			return false;
		}
		else
		{
			//std::cout << "codeCave written \t(" << bytes_written << " of " << sizeof_opCodesCodeCave << ")" << std::endl;
			return true;
		}

	}
	else
	{
		std::cerr << "Couldn't write process memory (codeCave). Error in hProc: " << hProc << std::endl;
		return false;
	}
}

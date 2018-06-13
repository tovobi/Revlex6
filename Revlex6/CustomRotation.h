#pragma once
#include "PlayerRotation.h"



class CustomRotation :
	public PlayerRotation
{
public:
	CustomRotation(WowHelpers &_wowHelperObj,InjectAsm &_wowInject);
	~CustomRotation();

	bool mainRotation(InjectAsm &wowInject, std::vector<RvxKeys> &rvxKey);
	bool autoRotation(InjectAsm &wowInject, std::vector<RvxKeys> &rvxKey);
	bool fullAutoRoutine = true;
	unsigned long blockAutoRoutine = 600;
	unsigned long lastAutoRoutine = 0;
	unsigned long blockPetAttack = 3000;
	unsigned long lastPetAttack = 0;
};


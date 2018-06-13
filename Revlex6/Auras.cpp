#include "stdafx.h"
#include "Auras.h"
#include <Windows.h>


Auras::Auras(std::string _name, unsigned int _id, float _duration, unsigned int _stacks, unsigned long _timeApplied)
	:
	name(_name),
	id(_id),
	duration(_duration),
	stacks(_stacks),
	timeApplied(_timeApplied)

{
	if (timeApplied == 0)
	{
		timeApplied = GetTickCount64();
	}
}


Auras::~Auras()
{
}

unsigned long Auras::getTime()
{
	return GetTickCount64();
}



#include "stdafx.h"
#include "Spells.h"


Spells::Spells(std::string _name, float _cd, unsigned int _id, unsigned int _rank, float _currentCooldown, bool _hasGlobalCooldown, bool _isDamageSpell)
:name(_name),
cd(_cd),
id(_id),
rank(_rank),
currentCooldown(_currentCooldown),
hasGlobalCooldown(_hasGlobalCooldown),
isDamageSpell(_isDamageSpell)
{

}
Spells::Spells()
	:name(""),
	cd(0),
	id(0),
	rank(0),
	currentCooldown(0),
	hasGlobalCooldown(true),
	isDamageSpell(true)
{

}


Spells::~Spells()
{
}

SpellsInHistory::SpellsInHistory(unsigned int _id, float _cd, unsigned int _startTime)
:id(_id),
cd(_cd),
startTime(_startTime)
{
}

SpellsInHistory::~SpellsInHistory()
{
}
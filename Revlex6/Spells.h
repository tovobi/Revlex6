#pragma once
#include <string>
class Spells
{
public:
	Spells(std::string _name, float _cd, unsigned int _id, unsigned int _rank, float _currentCooldown = 0.0f, bool _hasGlobalCooldown = true, bool _isDamageSpell = true);
	Spells();
	~Spells();

	std::string name;
	float cd = 0.0f;	
	unsigned int id = 0;
	unsigned int rank = 0;
	float currentCooldown = 0.0f;
	bool hasGlobalCooldown = false;
	bool isDamageSpell = false;
	long lastCast = 0;



};

class SpellsInHistory
{
public:
	SpellsInHistory(unsigned int _id, float _cd, unsigned int _startTime = 0);
	~SpellsInHistory();
	unsigned int id = 0;
	float cd = 0;
	unsigned int startTime = 0;
};


#pragma once
#include <string>
class Auras
{
public:
	Auras(std::string _name, unsigned int _id, float _duration = 1, unsigned int _stacks = 1, unsigned long _timeApplied = 0);
	~Auras();
	static unsigned long Auras::getTime();

	std::string name;
	unsigned int id;
	float duration;
	unsigned int stacks;
	unsigned long timeApplied;




};


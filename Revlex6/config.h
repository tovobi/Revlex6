#pragma once
#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <iterator>

class Config
{
public:
	Config();
	~Config();
	static bool cfgLoad();
	static bool cfgSave(int winPosX, int winPosY, std::vector<int> facHorde, std::vector<int> facAlliance);
	static int getWindowPosX();
	static int getWindowPosY();
	static std::vector<int> getHostileFactionHorde();
	static std::vector<int> getHostileFactionAlliance();

};


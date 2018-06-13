#pragma once
#include "stdafx.h"
#include <string>
#include <iostream>
#include <windows.h>
#include <vector>




class RvxKeys
{
public:
	RvxKeys(const unsigned int &c);
	RvxKeys();
	~RvxKeys();

	const unsigned int code;
	bool keyDown = false;
	bool keyTrigger = false;
	bool doneStillDown = false;
	//static std::vector<RvxKeys> rvxKey;
	static void processKeyState(std::vector<RvxKeys> &rkey);
	static void initKeys(std::vector<RvxKeys> &rkey);
	int getKeyState();


};

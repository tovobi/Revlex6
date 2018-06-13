#include "stdafx.h"
#include "RvxKeys.h"


const static std::vector<unsigned int> acceptedKeys{/*0-9*/ 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, /*A-Z*/ /*A: 0x41,*/ 0x42, 0x43, /*D: 0x44,*/ 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, 0x50, 0x51, 0x52, /*S: 0x53,*/ 0x54, 0x55, 0x56, /*W: 0x57,*/ 0x58, 0x59, 0x5A, /*LSHIFT,LCTRL,LALT,A1,A3,A5,Enter,Space,Backspace,Escape*/ 0xA0, 0xA2, 0xA4, 0xA1, 0xA3, 0xA5, 0x0D, /*Space: 0x20,*/ 0x08, 0x1B, /*^,<,,,.,-,ö,ä,#,ü,+,ß,´*/ 0xDC, 0xE2, 0xBC, 0xBE, 0xBD, 0xC0, 0xDE, 0xBF, 0xBA, 0xBB, 0xDB, 0xDD, /*F1,F2,F3,F4,F5,F6,F7,F8,F9,F10,F11,F12*/ 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x7B, /*Left,Down,Right,Up,Del,End,PgDn,Insert,Home,PgUp*/ 0x25, 0x28, 0x27, 0x26, 0x2E, 0x23, 0x22, 0x2D, 0x24, 0x21 };

RvxKeys::RvxKeys(const unsigned int &c)
	:code(c)
{
}
RvxKeys::RvxKeys()
	: code(0)
{
}

RvxKeys::~RvxKeys()
{
}

void RvxKeys::initKeys(std::vector<RvxKeys> &rkey)
{
	for (unsigned int i = 0; i < 256; i++)
	{
		rkey.push_back(RvxKeys(i));
	}
}



void RvxKeys::processKeyState(std::vector<RvxKeys> &rkey)
{
	for (unsigned int k : acceptedKeys)
	{
		unsigned short check = GetAsyncKeyState(k);
		if (rkey[k].doneStillDown == true)
		{
			rkey[k].keyDown = true;
			rkey[k].keyTrigger = false;
		}
		if (check == 0x0001 || check == 0x8001 || (check == 0x8000 && rkey[k].doneStillDown == false))
		{
			rkey[k].keyDown = true;
			rkey[k].keyTrigger = true;
			rkey[k].doneStillDown = true;
		}
		else if (check == 0 && rkey[k].doneStillDown == true)
		{
			rkey[k].keyDown = false;
			rkey[k].keyTrigger = false;
			rkey[k].doneStillDown = false;
		}
	}
}

int RvxKeys::getKeyState()
{
	return (keyDown + keyTrigger);
}




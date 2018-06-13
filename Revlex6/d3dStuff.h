#pragma once


#include "stdafx.h"
#include <d3d9.h>
#include <iostream>
#include <ctime>
#include <string>
#include "WowHelpers.h"

// include the Direct3D Library file
#pragma comment (lib, "d3d9.lib")

class d3dStuff
{
public:
	d3dStuff();
	~d3dStuff();
	static DWORD initD3D(HWND hWnd);

	// global declarations
	//static LPDIRECT3D9 d3d;    // the pointer to our Direct3D interface
	//static LPDIRECT3DDEVICE9 d3ddev;    // the pointer to the device class


};


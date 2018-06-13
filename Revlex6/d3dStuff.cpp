#include "stdafx.h"
#include "d3dStuff.h"


d3dStuff::d3dStuff()
{
}


d3dStuff::~d3dStuff()
{
}

// this function initializes and prepares Direct3D for use
DWORD d3dStuff::initD3D(HWND hWnd)
{
	LPDIRECT3D9 d3d = Direct3DCreate9(D3D_SDK_VERSION);
	LPDIRECT3DDEVICE9 d3ddev;

	D3DPRESENT_PARAMETERS d3dpp;

	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dpp.BackBufferWidth = 10;
	d3dpp.BackBufferHeight = 10;

	// create a device class using this information and the info from the d3dpp stuct
	d3d->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		&d3ddev);

	unsigned long *pInterface;
	pInterface = (unsigned long*)*((unsigned long*)d3ddev);
	DWORD edscOffset = (DWORD)pInterface[42];

	std::wstring tempEdsc = WowHelpers::s2ws(std::to_string(edscOffset));
	std::cout << std::hex << edscOffset << std::endl;
	//MessageBox(NULL, tempEdsc.c_str(), _T("message"), MB_OK | MB_SYSTEMMODAL);
	return edscOffset;
}



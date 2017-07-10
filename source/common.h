#ifndef __COMMON_H__
#define __COMMON_H__


#pragma warning(disable : 4005)


#define DIRECTINPUT_VERSION 0x0800

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>


#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <vector>


#include <d3d11.h>
#include <D3DX11.h>
#include <d3dx10math.h>
#include <dinput.h>


#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")


extern ID3D11Device* dev;
extern ID3D11DeviceContext *devc;

#define wndw GetSystemMetrics(SM_CXSCREEN)
#define wndh GetSystemMetrics(SM_CYSCREEN)


#define PI (float)D3DX_PI
#define DEG_TO_RAD(deg) \
	(deg * PI) / 180.0f


#endif
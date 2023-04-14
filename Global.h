#pragma once

#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <vector>
#include <string>
#include <DirectXMath.h>
#include <d3dcompiler.h>

#include <DirectXTex.h>
#include "MathFunc.h"

#define DIRECTINPUT_VERSION		0x0800		//DirectInputのバージョン指定

#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

using namespace DirectX;

// 円周率
const float PI = 3.141592f;





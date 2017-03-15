// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include <malloc.h>
#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")

// C RunTime Header Files
#include <stdlib.h>

#include <memory.h>
#include <tchar.h>

// STD Header Files
#include <string>

//DirectX Header Files
#include <d3d11_1.h>
#include <DirectXMath.h>

//Windows Imaging Component
#include <wincodec.h>
#include <wincodecsdk.h>
#pragma comment(lib, "WindowsCodecs.lib")

#include <Psapi.h>



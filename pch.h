

#ifndef PCH_H
#define PCH_H

#pragma once

// 여기에 미리 컴파일하려는 헤더 추가
#include "framework.h"

// 1) Windows 헤더 설정 (먼저 정의들)
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX             // <gdiplus.h> 포함 전 반드시 필요: min/max 매크로 충돌 방지


// 2) Windows 기본
#include <windows.h>
#include <objidl.h>          // GDI+ 내부에서 사용

#include <timeapi.h>
#pragma comment(lib, "winmm.lib")

// FMOD
#include <iostream>
#include <io.h>

#include "fmod.h"
#include "fmod.hpp"

#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")

#pragma comment(lib, "msimg32.lib") // AlphaBlend

#include <string>
#include <algorithm>
#include <cwctype>     // std::towlower
#include <cwchar>      // _wcsicmp (MSVC)
#include <list>
#include <unordered_map>
#include <map>
#include <vector>
#include <ctime>
#include <cmath>



#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif
#endif

using namespace std;

#endif //PCH_H

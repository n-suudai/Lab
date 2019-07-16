#pragma once


//-----------------------------------------------------------------
// Windows 関連
//-----------------------------------------------------------------
#if !defined(STRICT)
#   define STRICT
#endif

//#if !defined(WIN32_LEAN_AND_MEAN)
//#   define WIN32_LEAN_AND_MEAN
//#endif

#if !defined(NOMINMAX) && defined(_MSC_VER)
#   define NOMINMAX
#endif

#include <tchar.h>
#include <Windows.h>


#pragma once

#if defined(DEBUG_MODE) || defined(RELEASE_MODE) || defined(RELWITHDEBINFO_MODE)
#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>
#endif

#include <functional>
#include <string>
#include <windows.h>


namespace GameMaker
{
class CrashUtils
{
public:
	static LONG WINAPI DetectApplicationCrash(EXCEPTION_POINTERS* ep);

public:
	static std::wstring GetCurrentSystemTimeAsString();
	static std::wstring GetMinidumpPath();
	static bool CreateMinidumpFile(const std::wstring& path, EXCEPTION_POINTERS* ep);
};

}
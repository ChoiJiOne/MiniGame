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
/**
 * @brief 프로그램의 크래시 관련 처리를 수행합니다.
 */
class CrashUtils
{
public:
};

}
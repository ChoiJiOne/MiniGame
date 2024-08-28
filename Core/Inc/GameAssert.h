#pragma once

#include "GameError.h"
#include "GameLogger.h"

#if defined(DEBUG_MODE) || defined(RELEASE_MODE) || defined(RELWITHDEBINFO_MODE)
#ifndef GAME_ASSERT
#define GAME_ASSERT(EXP, MESSAGE)\
{\
	if(!(bool)(EXP))\
	{\
		GameLogger::DebugPrintF("\nAssertion check point failed!\nFile : %s\nLine : %d\nExpression : %s\nMessage : %s\n", __FILE__, __LINE__, #EXP, MESSAGE);\
		__debugbreak();\
	}\
}
#endif
#else
#ifndef GAME_ASSERT
#define GAME_ASSERT(EXP, MESSAGE) ((void)(EXP))
#endif
#endif

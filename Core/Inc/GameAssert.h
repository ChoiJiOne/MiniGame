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

#if defined(__glad_h_)
#if defined(DEBUG_MODE) || defined(RELEASE_MODE) || defined(RELWITHDEBINFO_MODE)
#ifndef GL_ASSERT
#define GL_ASSERT(EXP)\
EXP;\
{\
	GLenum _errorCode = glGetError();\
	if (_errorCode != GL_NO_ERROR)\
	{\
		GameLogger::DebugPrintF("\nOpenGL Assertion check point failed!\nFile : %s\nLine : %d\nExpression : %s\nMessage : %s\n", __FILE__, __LINE__, #EXP, GLError::GetErrorMessage(_errorCode));\
		__debugbreak();\
	}\
}
#endif
#else
#ifndef GL_ASSERT
#define GL_ASSERT(EXP) ((void)(EXP))
#endif
#endif
#endif
#pragma once

#include "GameError.h"
#include "GameLogger.h"

#if defined(DEBUG_MODE) || defined(RELEASE_MODE) || defined(RELWITHDEBINFO_MODE)
#ifndef GAME_ASSERT
#define GAME_ASSERT(EXP, ...)\
{\
	if(!(bool)(EXP))\
	{\
		GameLogger::DebugPrintF("\nAssertion check point failed!\nFile : %s\nLine : %d\nExpression : %s\nMessage : ", __FILE__, __LINE__, #EXP);\
		GameLogger::DebugPrintF(__VA_ARGS__);\
		GameLogger::DebugPrintF("\n");\
		__debugbreak();\
	}\
}
#endif
#ifndef GAME_CHECK
#define GAME_CHECK(EXP)\
{\
	if(!(bool)(EXP))\
	{\
		GameLogger::DebugPrintF("\nAssertion check point failed!\nFile : %s\nLine : %d\nExpression : %s\n", __FILE__, __LINE__, #EXP);\
		__debugbreak();\
	}\
}
#endif
#else
#ifndef GAME_ASSERT
#define GAME_ASSERT(EXP, ...) ((void)(EXP))
#endif
#ifndef GAME_CHECK
#define GAME_CHECK(EXP) ((void)(EXP))
#endif
#endif

#if defined(__glad_h_)
#if defined(DEBUG_MODE) || defined(RELEASE_MODE) || defined(RELWITHDEBINFO_MODE)
#ifndef GL_ASSERT
#define GL_ASSERT(EXP, ...)\
EXP;\
{\
	GLenum _errorCode = glGetError();\
	if (_errorCode != GL_NO_ERROR)\
	{\
		GameLogger::DebugPrintF("\nOpenGL Assertion check point failed!\nFile : %s\nLine : %d\nExpression : %s\nGL : %s\nMessage : ", __FILE__, __LINE__, #EXP, GLError::GetErrorMessage(_errorCode));\
		GameLogger::DebugPrintF(__VA_ARGS__);\
		GameLogger::DebugPrintF("\n");\
		__debugbreak();\
	}\
}
#endif
#ifndef GL_CHECK
#define GL_CHECK(EXP)\
EXP;\
{\
	GLenum _errorCode = glGetError();\
	if (_errorCode != GL_NO_ERROR)\
	{\
		GameLogger::DebugPrintF("\nOpenGL Assertion check point failed!\nFile : %s\nLine : %d\nExpression : %s\nGL : %s\n", __FILE__, __LINE__, #EXP, GLError::GetErrorMessage(_errorCode));\
		__debugbreak();\
	}\
}
#endif
#else
#ifndef GL_ASSERT
#define GL_ASSERT(EXP, ...) ((void)(EXP))
#endif
#ifndef GL_CHECK
#define GL_CHECK(EXP) ((void)(EXP))
#endif
#endif
#endif
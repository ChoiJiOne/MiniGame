#pragma once

#include <cstdarg>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>
#include <map>

#include <windows.h>

#if defined(DEBUG_MODE) || defined(RELEASE_MODE) || defined(RELWITHDEBINFO_MODE)
inline void DebugPrintF(const char* format, ...)
{
	static const int32_t BUFFER_SIZE = 1024;
	static char buffer[BUFFER_SIZE];

	va_list args;
	va_start(args, format);
	int32_t size = _vsnprintf_s(buffer, BUFFER_SIZE, BUFFER_SIZE, format, args);
	va_end(args);

	/**  MinSizeRel 에서는 동작하지 않음 */
	OutputDebugStringA(buffer);
}

inline void DebugPrintF(const wchar_t* format, ...)
{
	static const int32_t BUFFER_SIZE = 1024;
	static wchar_t buffer[BUFFER_SIZE];

	va_list args;
	va_start(args, format);
	int32_t size = _vsnwprintf_s(buffer, BUFFER_SIZE, format, args);
	va_end(args);

	/**  MinSizeRel 에서는 동작하지 않음 */
	OutputDebugStringW(buffer);
}
#endif

#if defined(DEBUG_MODE) || defined(RELEASE_MODE) || defined(RELWITHDEBINFO_MODE)
#ifndef ASSERT
#define ASSERT(EXP, ...)\
{\
	if(!(bool)(EXP))\
	{\
		DebugPrintF("\nAssertion check point failed!\nFile : %s\nLine : %d\nExpression : %s\nMessage : ", __FILE__, __LINE__, #EXP);\
		DebugPrintF(__VA_ARGS__);\
		DebugPrintF("\n");\
		__debugbreak();\
	}\
}
#endif
#ifndef CHECK
#define CHECK(EXP)\
{\
	if(!(bool)(EXP))\
	{\
		DebugPrintF("\nAssertion check point failed!\nFile : %s\nLine : %d\nExpression : %s\n", __FILE__, __LINE__, #EXP);\
		__debugbreak();\
	}\
}
#endif
#else
#ifndef ASSERT
#define ASSERT(EXP, ...) ((void)(EXP))
#endif
#ifndef CHECK
#define CHECK(EXP) ((void)(EXP))
#endif
#endif

#if defined(WIN32) || defined(WIN64)
#include <windows.h>
static const char* GetWinErrorMessage()
{
	static const uint32_t MAX_BUFFER_SIZE = 1024;
	static char buffer[MAX_BUFFER_SIZE];

	uint32_t size = FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, nullptr, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), buffer, MAX_BUFFER_SIZE, nullptr);

	return buffer;
}
#endif

#if defined(__glad_h_)
static const char* GetGLErrorMessage(const GLenum errorCode)
{
	static std::map<GLenum, const char*> errorMessages =
	{
		{ GL_NO_ERROR,                      "No error has been recorded. (GL_NO_ERROR)"                                                                              },
		{ GL_INVALID_ENUM,                  "An unacceptable value is specified for an enumerated argument. (GL_INVALID_ENUM)"                                       },
		{ GL_INVALID_VALUE,                 "A numeric argument is out of range. (GL_INVALID_VALUE)"                                                                 },
		{ GL_INVALID_OPERATION,             "The specified operation is not allowed in the current state. (GL_INVALID_OPERATION)"                                    },
		{ GL_INVALID_FRAMEBUFFER_OPERATION, "The framebuffer object is not complete. (GL_INVALID_FRAMEBUFFER_OPERATION)"                                             },
		{ GL_OUT_OF_MEMORY,                 "There is not enough memory left to execute the command. (GL_OUT_OF_MEMORY)"                                             },
		{ GL_STACK_UNDERFLOW,               "An attempt has been made to perform an operation that would cause an internal stack to underflow. (GL_STACK_UNDERFLOW)" },
		{ GL_STACK_OVERFLOW,                "An attempt has been made to perform an operation that would cause an internal stack to overflow. (GL_STACK_OVERFLOW)"   },
		{ GL_CONTEXT_LOST,                  "This occurs when the OpenGL context becomes lost due to a graphics card reset. (GL_CONTEXT_LOST)"                       },
		{ GL_NONE,                          "Undefined error message."                                                                                               },
	};

	auto it = errorMessages.find(errorCode);
	return (it != errorMessages.end()) ? it->second : errorMessages.at(GL_NONE);
}

#if defined(DEBUG_MODE) || defined(RELEASE_MODE) || defined(RELWITHDEBINFO_MODE)
#ifndef GL_ASSERT
#define GL_ASSERT(EXP, ...)\
EXP;\
{\
	GLenum _errorCode = glGetError();\
	if (_errorCode != GL_NO_ERROR)\
	{\
		DebugPrintF("\nOpenGL Assertion check point failed!\nFile : %s\nLine : %d\nExpression : %s\nGL : %s\nMessage : ", __FILE__, __LINE__, #EXP, GetGLErrorMessage(_errorCode));\
		DebugPrintF(__VA_ARGS__);\
		DebugPrintF("\n");\
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
		DebugPrintF("\nOpenGL Assertion check point failed!\nFile : %s\nLine : %d\nExpression : %s\nGL : %s\n", __FILE__, __LINE__, #EXP, GetGLErrorMessage(_errorCode));\
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
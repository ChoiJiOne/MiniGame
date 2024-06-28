#pragma once

#include <cstdarg>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

#if defined(DEBUG_MODE) || defined(RELEASE_MODE) || defined(RELWITHDEBINFO_MODE)
/**
 * @brief 디버그 창에 형식화된 문자열을 출력합니다.
 *
 * @param format 형식 문자열입니다.
 * @param .. 문자열 내의 형식에 대응하는 가변인자입니다.
 *
 * @note
 * - DEBUG_MODE, RELEASE_MODE, RELWITHDEBINFO_MODE 에서만 디버그 창에 문자열이 출력됩니다.
 * - MINSIZEREL_MODE 모드에서는 디버그 창에 문자열이 출력되지 않습니다.
 */
inline void DebugPrintF(const char* format, ...)
{
	static const int32_t BUFFER_SIZE = 1024;
	static char buffer[BUFFER_SIZE];

	va_list args;
	va_start(args, format);
	int32_t size = _vsnprintf_s(buffer, BUFFER_SIZE, BUFFER_SIZE, format, args);
	va_end(args);

	OutputDebugStringA(buffer);
}


/**
 * @brief 디버그 창에 형식화된 문자열을 출력합니다.
 *
 * @param format 형식 문자열입니다.
 * @param .. 문자열 내의 형식에 대응하는 가변인자입니다.
 *
 * @note
 * - DEBUG_MODE, RELEASE_MODE, RELWITHDEBINFO_MODE 에서만 디버그 창에 문자열이 출력됩니다.
 * - MINSIZEREL_MODE 모드에서는 디버그 창에 문자열이 출력되지 않습니다.
 */
inline void DebugPrintF(const wchar_t* format, ...)
{
	static const int32_t BUFFER_SIZE = 1024;
	static wchar_t buffer[BUFFER_SIZE];

	va_list args;
	va_start(args, format);
	int32_t size = _vsnwprintf_s(buffer, BUFFER_SIZE, format, args);
	va_end(args);

	OutputDebugStringW(buffer);
}
#endif


/**
 * @brief 평가식을 검사합니다.
 *
 * @param Expression 검사할 호출값입니다.
 * @param ... 평가식을 만족하지 못할 경우 표시할 가변 인자 메시지입니다.
 *
 * @note
 * - 디버거가 존재할 때, DEBUG_MODE에서는 브레이크 포인트가 걸린 후 프로세스가 종료됩니다.
 * - 디버거가 존재할 때, RELEASE_MODE와 RELWITHDEBINFO_MODE에서는 브레이크 포인트가 걸리지만 프로세스는 종료되지 않습니다.
 * - MINSIZEREL_MODE에서는 평가식을 검사하지 않습니다.
 */
#if defined(DEBUG_MODE)
#ifndef ASSERT
#define ASSERT(Expression, ...)\
{\
	if (!(bool)(Expression))                                                                                                                \
	{                                                                                                                                       \
		DebugPrintF("\nAssertion check point failed!\nFile : %s\nLine : %d\nExpression : %s\nMessage : ", __FILE__, __LINE__, #Expression); \
		DebugPrintF(__VA_ARGS__);                                                                                                           \
		DebugPrintF("\n\n");                                                                                                                \
		__debugbreak();                                                                                                                     \
		ExitProcess(-1);                                                                                                                    \
	}                                                                                                                                       \
}
#endif
#elif defined(RELEASE_MODE) || defined(RELWITHDEBINFO_MODE)
#ifndef ASSERT
#define ASSERT(Expression, ...)\
{\
	if (!(bool)(Expression))                                                                                                                \
	{                                                                                                                                       \
		DebugPrintF("\nAssertion check point failed!\nFile : %s\nLine : %d\nExpression : %s\nMessage : ", __FILE__, __LINE__, #Expression); \
		DebugPrintF(__VA_ARGS__);                                                                                                           \
		DebugPrintF("\n\n");                                                                                                                \
		__debugbreak();                                                                                                                     \
	}                                                                                                                                       \
}
#endif
#else // defined(MINSIZEREL_MODE)
#ifndef ASSERT
#define ASSERT(Expression, ...) ((void)(Expression))
#endif
#endif


/**
 * @brief 평가식을 검사합니다.
 *
 * @param Expression 검사할 호출값입니다.
 *
 * @note
 * - 디버거가 존재할 때, DEBUG_MODE에서는 브레이크 포인트가 걸린 후 프로세스가 종료됩니다.
 * - 디버거가 존재할 때, RELEASE_MODE와 RELWITHDEBINFO_MODE에서는 브레이크 포인트가 걸리지만 프로세스는 종료되지 않습니다.
 * - MINSIZEREL_MODE에서는 평가식을 검사하지 않습니다.
 */
#if defined(DEBUG_MODE)
#ifndef CHECK
#define CHECK(Expression)\
{\
	if (!(bool)(Expression))                                                                                              \
	{                                                                                                                     \
		DebugPrintF("\nCheck point failed!\nFile : %s\nLine : %d\nExpression : %s\n\n", __FILE__, __LINE__, #Expression); \
		__debugbreak();                                                                                                   \
		ExitProcess(-1);                                                                                                  \
	}                                                                                                                     \
}
#endif
#elif defined(RELEASE_MODE) || defined(RELWITHDEBINFO_MODE)
#ifndef CHECK
#define CHECK(Expression)\
{\
	if (!(bool)(Expression))                                                                                              \
	{                                                                                                                     \
		DebugPrintF("\nCheck point failed!\nFile : %s\nLine : %d\nExpression : %s\n\n", __FILE__, __LINE__, #Expression); \
		__debugbreak();                                                                                                   \
	}                                                                                                                     \
}
#endif
#else // defined(MINSIZEREL_MODE)
#ifndef CHECK
#define CHECK(Expression, ...) ((void)(Expression))
#endif
#endif


#if defined(SDL_h_)
/**
 * @brief 평가식을 검사합니다.
 *
 * @param Expression 검사할 호출값입니다.
 * @param ... 평가식을 만족하지 못할 경우 표시할 가변 인자 메시지입니다.
 *
 * @note
 * - 디버거가 존재할 때, DEBUG_MODE에서는 브레이크 포인트가 걸린 후 프로세스가 종료됩니다.
 * - 디버거가 존재할 때, RELEASE_MODE와 RELWITHDEBINFO_MODE에서는 브레이크 포인트가 걸리지만 프로세스는 종료되지 않습니다.
 * - MINSIZEREL_MODE에서는 평가식을 검사하지 않습니다.
 * - SDL API은 호출에 성공하면 0을 반환합니다.
 */
#if defined(DEBUG_MODE)
#ifndef SDL_ASSERT
#define SDL_ASSERT(Expression, ...)\
{\
	if ((bool)(Expression))                                                                                                                     \
	{                                                                                                                                           \
		DebugPrintF("\nSDL Assertion check point failed!\nFile : %s\nLine : %d\nExpression : %s\nMessage : ", __FILE__, __LINE__, #Expression); \
		DebugPrintF(__VA_ARGS__);                                                                                                               \
		DebugPrintF("\nSDL error message : %s\n", SDL_GetError());                                                                              \
		DebugPrintF("\n");                                                                                                                      \
		__debugbreak();                                                                                                                         \
		ExitProcess(-1);                                                                                                                        \
	}                                                                                                                                           \
}
#endif
#elif defined(RELEASE_MODE) || defined(RELWITHDEBINFO_MODE)
#ifndef SDL_ASSERT
#define SDL_ASSERT(Expression, ...)\
{\
	if ((bool)(Expression))                                                                                                                     \
	{                                                                                                                                           \
		DebugPrintF("\nSDL Assertion check point failed!\nFile : %s\nLine : %d\nExpression : %s\nMessage : ", __FILE__, __LINE__, #Expression); \
		DebugPrintF(__VA_ARGS__);                                                                                                               \
		DebugPrintF("\nSDL error message : %s\n", SDL_GetError());                                                                              \
		DebugPrintF("\n");                                                                                                                      \
		__debugbreak();                                                                                                                         \
}
#endif
#else // defined(MINSIZEREL_MODE)
#ifndef SDL_ASSERT
#define SDL_ASSERT(Expression, ...) ((void)(Expression))
#endif
#endif


/**
 * @brief SDL API가 호출에 실패했는지 확인합니다.
 *
 * @param Expression 검사할 호출값입니다.
 *
 * @note
 * - 디버거가 존재할 때, DEBUG_MODE에서는 브레이크 포인트가 걸린 후 프로세스가 종료됩니다.
 * - 디버거가 존재할 때, RELEASE_MODE와 RELWITHDEBINFO_MODE에서는 브레이크 포인트가 걸리지만 프로세스는 종료되지 않습니다.
 * - MINSIZEREL_MODE에서는 평가식을 검사하지 않습니다.
 * - SDL API은 호출에 성공하면 0을 반환합니다.
 */
#if defined(DEBUG_MODE)
#ifndef SDL_FAILED
#define SDL_FAILED(Expression)\
{\
	if ((bool)(Expression))                                                                                                   \
	{                                                                                                                         \
		DebugPrintF("\nSDL API call has failed!\nFile : %s\nLine : %d\nExpression : %s\n", __FILE__, __LINE__, #Expression);  \
		DebugPrintF("SDL error message : %s\n", SDL_GetError());                                                              \
		DebugPrintF("\n");                                                                                                    \
		__debugbreak();                                                                                                       \
		ExitProcess(-1);                                                                                                      \
	}                                                                                                                         \
}
#endif
#elif defined(RELEASE_MODE) || defined(RELWITHDEBINFO_MODE)
#ifndef SDL_FAILED
#define SDL_FAILED(Expression)\
{\
	if ((bool)(Expression))                                                                                                   \
	{                                                                                                                         \
		DebugPrintF("\nSDL API call has failed!\nFile : %s\nLine : %d\nExpression : %s\n", __FILE__, __LINE__, #Expression);  \
		DebugPrintF("SDL error message : %s\n", SDL_GetError());                                                              \
		DebugPrintF("\n");                                                                                                    \
		__debugbreak();                                                                                                       \
	}                                                                                                                         \
}
#endif
#else // defined(MINSIZEREL_MODE)
#ifndef SDL_FAILED
#define SDL_FAILED(Expression, ...) ((void)(Expression))
#endif
#endif
#endif
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
 * @brief ����� â�� ����ȭ�� ���ڿ��� ����մϴ�.
 *
 * @param format ���� ���ڿ��Դϴ�.
 * @param .. ���ڿ� ���� ���Ŀ� �����ϴ� ���������Դϴ�.
 *
 * @note
 * - DEBUG_MODE, RELEASE_MODE, RELWITHDEBINFO_MODE ������ ����� â�� ���ڿ��� ��µ˴ϴ�.
 * - MINSIZEREL_MODE ��忡���� ����� â�� ���ڿ��� ��µ��� �ʽ��ϴ�.
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
 * @brief ����� â�� ����ȭ�� ���ڿ��� ����մϴ�.
 *
 * @param format ���� ���ڿ��Դϴ�.
 * @param .. ���ڿ� ���� ���Ŀ� �����ϴ� ���������Դϴ�.
 *
 * @note
 * - DEBUG_MODE, RELEASE_MODE, RELWITHDEBINFO_MODE ������ ����� â�� ���ڿ��� ��µ˴ϴ�.
 * - MINSIZEREL_MODE ��忡���� ����� â�� ���ڿ��� ��µ��� �ʽ��ϴ�.
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
 * @brief �򰡽��� �˻��մϴ�.
 *
 * @param Expression �˻��� ȣ�Ⱚ�Դϴ�.
 * @param ... �򰡽��� �������� ���� ��� ǥ���� ���� ���� �޽����Դϴ�.
 *
 * @note
 * - ����Ű� ������ ��, DEBUG_MODE������ �극��ũ ����Ʈ�� �ɸ� �� ���μ����� ����˴ϴ�.
 * - ����Ű� ������ ��, RELEASE_MODE�� RELWITHDEBINFO_MODE������ �극��ũ ����Ʈ�� �ɸ����� ���μ����� ������� �ʽ��ϴ�.
 * - MINSIZEREL_MODE������ �򰡽��� �˻����� �ʽ��ϴ�.
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
 * @brief �򰡽��� �˻��մϴ�.
 *
 * @param Expression �˻��� ȣ�Ⱚ�Դϴ�.
 *
 * @note
 * - ����Ű� ������ ��, DEBUG_MODE������ �극��ũ ����Ʈ�� �ɸ� �� ���μ����� ����˴ϴ�.
 * - ����Ű� ������ ��, RELEASE_MODE�� RELWITHDEBINFO_MODE������ �극��ũ ����Ʈ�� �ɸ����� ���μ����� ������� �ʽ��ϴ�.
 * - MINSIZEREL_MODE������ �򰡽��� �˻����� �ʽ��ϴ�.
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
 * @brief �򰡽��� �˻��մϴ�.
 *
 * @param Expression �˻��� ȣ�Ⱚ�Դϴ�.
 * @param ... �򰡽��� �������� ���� ��� ǥ���� ���� ���� �޽����Դϴ�.
 *
 * @note
 * - ����Ű� ������ ��, DEBUG_MODE������ �극��ũ ����Ʈ�� �ɸ� �� ���μ����� ����˴ϴ�.
 * - ����Ű� ������ ��, RELEASE_MODE�� RELWITHDEBINFO_MODE������ �극��ũ ����Ʈ�� �ɸ����� ���μ����� ������� �ʽ��ϴ�.
 * - MINSIZEREL_MODE������ �򰡽��� �˻����� �ʽ��ϴ�.
 * - SDL API�� ȣ�⿡ �����ϸ� 0�� ��ȯ�մϴ�.
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
 * @brief SDL API�� ȣ�⿡ �����ߴ��� Ȯ���մϴ�.
 *
 * @param Expression �˻��� ȣ�Ⱚ�Դϴ�.
 *
 * @note
 * - ����Ű� ������ ��, DEBUG_MODE������ �극��ũ ����Ʈ�� �ɸ� �� ���μ����� ����˴ϴ�.
 * - ����Ű� ������ ��, RELEASE_MODE�� RELWITHDEBINFO_MODE������ �극��ũ ����Ʈ�� �ɸ����� ���μ����� ������� �ʽ��ϴ�.
 * - MINSIZEREL_MODE������ �򰡽��� �˻����� �ʽ��ϴ�.
 * - SDL API�� ȣ�⿡ �����ϸ� 0�� ��ȯ�մϴ�.
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


#if defined(_WIN32) || defined(_WIN64)
/**
 * @brief Windows ���� �ڵ忡 �����ϴ� �޽����� ����ϴ�.
 * 
 * @param errorCode ���� �޽����� ���� ���� �ڵ��Դϴ�.
 * 
 * @return ���� �ڵ忡 �����ϴ� �޽����� ��ȯ�մϴ�.
 * 
 * @see https://learn.microsoft.com/ko-kr/windows/win32/api/winbase/nf-winbase-formatmessagea
 */
inline std::string GetWinErrorCodeMessageA(uint32_t errorCode)
{
	static const uint32_t MAX_BUFFER_SIZE = 1024;
	static char buffer[MAX_BUFFER_SIZE];

	uint32_t size = FormatMessageA(
		FORMAT_MESSAGE_FROM_SYSTEM,
		nullptr,
		static_cast<DWORD>(errorCode),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		buffer,
		MAX_BUFFER_SIZE,
		nullptr
	);

	return std::string(buffer, size);
}


/**
 * @brief Windows ���� �ڵ忡 �����ϴ� �޽����� ����ϴ�.
 *
 * @param errorCode ���� �޽����� ���� ���� �ڵ��Դϴ�.
 *
 * @return ���� �ڵ忡 �����ϴ� �޽����� ��ȯ�մϴ�.
 */
inline std::wstring GetWinErrorCodeMessageW(uint32_t errorCode)
{
	static const uint32_t MAX_BUFFER_SIZE = 1024;
	static wchar_t buffer[MAX_BUFFER_SIZE];

	uint32_t size = FormatMessageW(
		FORMAT_MESSAGE_FROM_SYSTEM,
		nullptr,
		static_cast<DWORD>(errorCode),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		buffer,
		MAX_BUFFER_SIZE,
		nullptr
	);

	return std::wstring(buffer, size);
}


/**
 * @brief �򰡽��� �˻��մϴ�.
 *
 * @param Expression �˻��� ȣ�Ⱚ�Դϴ�.
 * @param ... �򰡽��� �������� ���� ��� ǥ���� ���� ���� �޽����Դϴ�.
 *
 * @note
 * - ����Ű� ������ ��, DEBUG_MODE������ �극��ũ ����Ʈ�� �ɸ� �� ���μ����� ����˴ϴ�.
 * - ����Ű� ������ ��, RELEASE_MODE�� RELWITHDEBINFO_MODE������ �극��ũ ����Ʈ�� �ɸ����� ���μ����� ������� �ʽ��ϴ�.
 * - MINSIZEREL_MODE������ �򰡽��� �˻����� �ʽ��ϴ�.
 */
#if defined(DEBUG_MODE)
#ifndef WINDOWS_ASSERT
#define WINDOWS_ASSERT(Expression, ...)\
{\
	if (!(bool)(Expression))                                                                                                                        \
	{                                                                                                                                               \
		DebugPrintF("\nWindows assertion check point failed!\nFile : %s\nLine : %d\nExpression : %s\nMessage : ", __FILE__, __LINE__, #Expression); \
		DebugPrintF(__VA_ARGS__);                                                                                                                   \
		DebugPrintF(L"Windows error message : %s\n", GetWinErrorCodeMessageW(GetLastError()).c_str());                                              \
		DebugPrintF("\n\n");                                                                                                                        \
		__debugbreak();                                                                                                                             \
		ExitProcess(-1);                                                                                                                            \
	}                                                                                                                                               \
}
#endif
#elif defined(RELEASE_MODE) || defined(RELWITHDEBINFO_MODE)
#ifndef WINDOWS_ASSERT
#define WINDOWS_ASSERT(Expression, ...)\
{\
	if (!(bool)(Expression))                                                                                                                        \
	{                                                                                                                                               \
		DebugPrintF("\nWindows assertion check point failed!\nFile : %s\nLine : %d\nExpression : %s\nMessage : ", __FILE__, __LINE__, #Expression); \
		DebugPrintF(__VA_ARGS__);                                                                                                                   \
		DebugPrintF(L"Windows error message : %s\n", GetWinErrorCodeMessageW(GetLastError()).c_str());                                              \
		DebugPrintF("\n\n");                                                                                                                        \
		__debugbreak();                                                                                                                             \
	}                                                                                                                                               \
}
#endif
#else // defined(MINSIZEREL_MODE)
#ifndef WINDOWS_ASSERT
#define WINDOWS_ASSERT(Expression, ...) ((void)(Expression))
#endif
#endif


/**
 * @brief �򰡽��� �˻��մϴ�.
 *
 * @param Expression �˻��� ȣ�Ⱚ�Դϴ�.
 *
 * @note
 * - ����Ű� ������ ��, DEBUG_MODE������ �극��ũ ����Ʈ�� �ɸ� �� ���μ����� ����˴ϴ�.
 * - ����Ű� ������ ��, RELEASE_MODE�� RELWITHDEBINFO_MODE������ �극��ũ ����Ʈ�� �ɸ����� ���μ����� ������� �ʽ��ϴ�.
 * - MINSIZEREL_MODE������ �򰡽��� �˻����� �ʽ��ϴ�.
 */
#if defined(DEBUG_MODE)
#ifndef WINDOWS_CHECK
#define WINDOWS_CHECK(Expression)\
{\
	if (!(bool)(Expression))                                                                                              \
	{                                                                                                                     \
		DebugPrintF("\nCheck point failed!\nFile : %s\nLine : %d\nExpression : %s\n\n", __FILE__, __LINE__, #Expression); \
		DebugPrintF(L"Windows error message : %s\n", GetWinErrorCodeMessageW(GetLastError()).c_str());                    \
		__debugbreak();                                                                                                   \
		ExitProcess(-1);                                                                                                  \
	}                                                                                                                     \
}
#endif
#elif defined(RELEASE_MODE) || defined(RELWITHDEBINFO_MODE)
#ifndef WINDOWS_CHECK
#define WINDOWS_CHECK(Expression)\
{\
	if (!(bool)(Expression))                                                                                              \
	{                                                                                                                     \
		DebugPrintF("\nCheck point failed!\nFile : %s\nLine : %d\nExpression : %s\n\n", __FILE__, __LINE__, #Expression); \
		DebugPrintF(L"Windows error message : %s\n", GetWinErrorCodeMessageW(GetLastError()).c_str());                    \
		__debugbreak();                                                                                                   \
	}                                                                                                                     \
}
#endif
#else // defined(MINSIZEREL_MODE)
#ifndef WINDOWS_CHECK
#define WINDOWS_CHECK(Expression, ...) ((void)(Expression))
#endif
#endif
#endif
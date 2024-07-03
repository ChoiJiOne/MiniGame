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
	/**
	 * @brief 어플리케이션의 크래시를 감지하고 덤프 파일을 생성합니다.
	 * 
	 * @param ep 예외에 대한 컨텍스트 포인터입니다.
	 * 
	 * @return 명시적으로 EXCEPTION_EXECUTE_HANDLER 값을 반환합니다.
	 */
	static LONG WINAPI DetectApplicationCrash(EXCEPTION_POINTERS* ep);


public:
	/**
	 * @brief 현재 시스템 시간을 문자열로 얻습니다.
	 *
	 * @return 문자열로 구성된 현재 시스템 시간을 반환합니다.
	 */
	static std::wstring GetCurrentSystemTimeAsString();


	/**
	 * @brief 미니 덤프 파일의 경로를 얻습니다.
	 *
	 * @return 미니 덤프 파일의 경로를 반환합니다.
	 */
	static std::wstring GetMinidumpPath();


	/**
	 * @brief 덤프 파일을 생성합니다.
	 *
	 * @param path 덤프 파일 경로입니다.
	 * @param ep 예외에 대한 컨텍스트 포인터입니다.
	 *
	 * @return 덤프 파일 생성에 성공하면 true, 그렇지 않으면 false 값을 반환합니다.
	 *
	 * @see
	 * - https://learn.microsoft.com/ko-kr/windows/win32/api/winnt/ns-winnt-exception_pointers
	 * - https://learn.microsoft.com/ko-kr/windows/win32/api/minidumpapiset/nf-minidumpapiset-minidumpwritedump
	 */
	static bool CreateMinidumpFile(const std::wstring& path, EXCEPTION_POINTERS* ep);
};

}
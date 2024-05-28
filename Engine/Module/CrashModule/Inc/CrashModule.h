#pragma once

#if defined(DEBUG_MODE) || defined(RELEASE_MODE) || defined(DEVELOPMENT_MODE)
#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>
#endif

#include <functional>
#include <windows.h>


/**
 * @brief 어플리케이션 크래시를 처리하는 기능을 제공합니다.
 */
class CrashModule
{
public:
	/**
	 * @brief 크래시 모듈의 에러 목록입니다.
	 */
	enum class Errors
	{
		OK            = 0x00, // 에러 없음
		FAILED        = 0x01, // 일반적인 에러
		ERR_CREATE    = 0x02, // 파일 생성 실패
		ERR_WRITE     = 0x03, // 파일 쓰기 실패
		ERR_CLOSE     = 0x04, // 파일 닫기 실패
		ERR_API       = 0x05, // API 호출 실패
		ERR_PREV_INIT = 0x06, // 이전에 초기화를 수행했습니다.
		ERR_NOT_INIT  = 0x07, // 초기화가 수행되지 않았습니다.
	};


	/**
	 * @brief 크래시 모듈을 초기화합니다.
	 *
	 * @param callback 크래시 발생 시 수행할 콜백 함수입니다.
	 *
	 * @return 크래시 모듈 초기화에 성공하면 Errors 열거형의 OK, 그렇지 않으면 그 외의 값을 반환합니다.
	 */
	static Errors Init(const std::function<void()>& callback = nullptr);


	/**
	 * @brief 크래시 모듈의 초기화를 해제합니다.
	 * 
	 * @return 크래시 모듈 초기화 해제에 성공하면 Errors 열거형의 OK, 그렇지 않으면 그 외의 값을 반환합니다.
	 */
	static Errors Uninit();


	/**
	 * @brief 크래시 모듈의 가장 최근 에러 메시지를 얻습니다.
	 *
	 * @return 크래시 모듈의 가장 마지막에 발생한 에러 메시지를 반환합니다.
	 */
	static const char* GetLastErrorMessage();


private:
	/**
	 * @brief 형식화된 문자열을 반환합니다.
	 *
	 * @note
	 * - 이 메서드는 snprintf와 같은 함수를 대체하기 위한 것입니다.
	 * - 문자열의 길이는 최대 1024 입니다.
	 *
	 * @param format 형식 문자열입니다.
	 * @param ... 문자열의 형식에 대응하는 가변인자입니다.
	 *
	 * @return 형식화된 표준 문자열을 반환합니다.
	 *
	 * @see https://learn.microsoft.com/ko-kr/cpp/c-runtime-library/reference/vsnprintf-vsnprintf-vsnprintf-l-vsnwprintf-vsnwprintf-l?view=msvc-170
	 */
	static std::wstring PrintF(const wchar_t* format, ...);


	/**
	 * @brief 에러 메시지를 설정합니다.
	 * 
	 * @note 에러가 존재하지 않으면 아무 동작도 수행하지 않습니다.
	 */
	static void SetLastErrorMessage();


	/**
	 * @brief 에러 메시지를 설정합니다.
	 *
	 * @param message 설정할 메시지입니다.
	 */
	static void SetLastErrorMessage(const std::string& message);


	/**
	 * @brief 현재 시스템 시간을 문자열로 얻습니다.
	 *
	 * @return 문자열로 구성된 현재 시스템 시간을 반환합니다.
	 */
	static std::wstring GetCurrentSystemTimeAsString();


	/**
	 * @brief 문자열 텍스트를 파일에 씁니다.
	 * 
	 * @param path 파일의 경로입니다.
	 * @param text 파일에 쓸 텍스트입니다.
	 * 
	 * @return 문자열 텍스트를 파일에 쓰는 데 성공했다면 Errors 열거형의 OK, 그렇지 않으면 그 외의 값을 반환합니다.
	 */
	static Errors WriteTextFile(const std::wstring& path, const std::string& text);


	/**
	 * @brief 덤프 파일을 생성합니다.
	 * 
	 * @param path 덤프 파일 경로입니다.
	 * @param exceptionPtr 예외에 대한 컨텍스트 포인터입니다.
	 * 
	 * @return 덤프 파일 생성에 성공하면 Errors 열거형의 OK, 그렇지 않으면 그 외의 값을 반환합니다.
	 * 
	 * @see 
	 * - https://learn.microsoft.com/ko-kr/windows/win32/api/winnt/ns-winnt-exception_pointers
	 * - https://learn.microsoft.com/ko-kr/windows/win32/api/minidumpapiset/nf-minidumpapiset-minidumpwritedump
	 */
	static Errors CreateMinidumpFile(const std::wstring& path, EXCEPTION_POINTERS* exceptionPtr);


	/**
	 * @brief 덤프 파일의 경로를 얻습니다.
	 * 
	 * @param outDumpFilePath 덤프 파일의 경로입니다.
	 * 
	 * @return 덤프 파일의 경로를 얻는 데 성공하면 Errors 열거형의 OK, 그렇지 않으면 그 외의 값을 반환합니다.
	 */
	static Errors GetDumpFilePath(std::wstring& outDumpFilePath);


	/**
	 * @brief 어플리케이션의 크래시를 감지합니다.
	 * 
	 * @param exceptionPtr 예외에 대한 컨텍스트 포인터입니다.
	 * 
	 * @return 명시적으로 EXCEPTION_EXECUTE_HANDLER 값을 반환합니다.
	 */
	static LONG WINAPI DetectApplicationCrash(EXCEPTION_POINTERS* exceptionPtr);


private:
	/**
	 * @brief 문자열 버퍼의 최대 크기입니다.
	 */
	static const uint32_t MAX_BUFFER_SIZE = 1024;


	/**
	 * @brief 크래시 모듈이 초기화되었는지 확인합니다.
	 */
	static bool bIsInitialize;


	/**
	 * @brief 가장 마지막 에러 메시지를 저장하는 버퍼입니다.
	 */
	static char lastErrorMessage[MAX_BUFFER_SIZE];


	/**
	 * @brief 덤프 파일 저장 경로입니다.
	 */
	static wchar_t dumpPath[MAX_BUFFER_SIZE];


	/**
	 * @brief 초기화 이전의 최상위 예외 처리 필터입니다.
	 */
	static LPTOP_LEVEL_EXCEPTION_FILTER topLevelExceptionFilter;


	/**
	 * @brief 크래시 발생 시 실행할 콜백 함수입니다.
	 * 
	 * @note 초기화 시 등록해야 사용할 수 있습니다.
	 */
	static std::function<void()> crashCallback;
};
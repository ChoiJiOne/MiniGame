#pragma once

#include <functional>
#include <string>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#else
#error "not support platform"
#endif


/**
 * @brief 플랫폼(Windows) 관련 처리를 수행하는 모듈입니다.
 * 
 * @note 이 클래스의 모든 멤버 변수와 메서드는 모두 정적(static) 타입입니다.
 */
class PlatformModule
{
public:
	/**
	 * @brief 플랫폼 모듈의 에러 처리 목록입니다.
	 */
	enum class Errors
	{
		OK            = 0x00, // 에러가 없습니다.
		FAILED        = 0x01, // 일반적인 에러입니다.
		ERR_API       = 0x02, // 일반적인 API 에러입니다.
		ERR_PREV_INIT = 0x03, // 이전에 초기화를 수행했습니다.
		ERR_NOT_INIT  = 0x04, // 초기화가 수행되지 않았습니다.
		ERR_IMGUI     = 0x05, // IMGUI 관련 에러입니다.
	};


	/**
	 * @brief 윈도우 생성에 사용할 파라미터입니다.
	 */
	struct WindowConstructParams
	{
		std::wstring title;     // 윈도우 타이틀입니다.
		int32_t x;              // 윈도우 상단 왼쪽의 x좌표입니다.
		int32_t y;              // 윈도우 상단 왼쪽의 y좌표입니다.
		int32_t w;              // 윈도우의 가로 크기입니다.
		int32_t h;              // 윈도우의 세로 크기입니다.
		bool bIsResizable;      // 윈도우 크기 변경 여부입니다.
		bool bIsFullscreenMode; // 윈도우의 풀스크린 여부입니다.
	};


public:
	/**
	 * @brief 플랫폼 모듈을 초기화합니다.
	 * 
	 * @param windowParam 윈도우 생성에 사용할 파라미터입니다.
	 * 
	 * @return 초기화에 성공하면 Errors 열거형의 OK 값을, 그렇지 않으면 그 외의 값을 반환합니다.
	 */
	static Errors Init(const WindowConstructParams& windowParam);


	/**
	 * @brief 플랫폼 모듈의 초기화를 해제합니다.
	 * 
	 * @return 초기화 해제에 성공하면 Errors 열거형의 OK 값을, 그렇지 않으면 그 외의 값을 반환합니다.
	 */
	static Errors Uninit();


	/**
	 * @brief 플랫폼 모듈의 가장 최근 에러 메시지를 얻습니다.
	 * 
	 * @return 플랫폼 모듈의 가장 최근에 발생한 에러 메시지의 포인터 값을 반환합니다.
	 */
	static const wchar_t* GetLastErrorMessage();


	/**
	 * @brief 윈도우 핸들 값을 얻습니다.
	 * 
	 * @return 윈도우 핸들 값을 반환합니다.
	 */
	static HWND GetWindowHandle();


	/**
	 * @brief 루프를 실행합니다.
	 * 
	 * @param callback 루프 실행 시 호출할 프레임 단위의 콜백 함수입니다.
	 */
	static void RunLoop(const std::function<void(float)>& callback);


	/**
	 * @brief 루프 실행 종료 시 호출할 콜백 함수를 설정합니다.
	 * 
	 * @param callback 루프 실행 종료 시 호출할 콜백 함수입니다.
	 */
	static void SetEndLoopCallback(const std::function<void()>& callback) { endLoopCallback = callback; }


	/**
	 * @brief 스크린 크기를 얻습니다.
	 *
	 * @param outWidth 스크린의 가로 크기입니다.
	 * @param outHeight 스크린의 세로 크기입니다.
	 *
	 * @return 스크린 크기를 얻는 데 성공하면 Errors 열거형의 OK 값을, 그렇지 않으면 그 외의 값을 반환합니다.
	 */
	static Errors GetScreenSize(int32_t& outWidth, int32_t& outHeight);


	/**
	 * @brief 풀스크린 모드인지 확인합니다.
	 * 
	 * @return 풀스크린 모드이면 true, 그렇지 않으면 false를 반환합니다.
	 */
	static bool IsFullscreenMode() { return bIsFullscreenMode; }


	/**
	 * @brief 루프 종료 여부를 설정합니다.
	 * 
	 * @param bIsQuit 설정할 루프 종료 여부입니다. 루프를 종료하고 싶다면 true, 그렇지 않으면 false입니다.
	 */
	static void SetQuitLoop(bool bIsQuit) { bIsQuitLoop = bIsQuit; }


private:
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
	static void SetLastErrorMessage(const std::wstring& message);


	/**
	 * @brief 윈도우 클래스를 등록합니다.
	 * 
	 * @return 윈도우 클래스 등록에 성공하면 Errors 열거형의 OK 값을, 그렇지 않으면 그 외의 값을 반환합니다.
	 */
	static Errors RegisterWindowClass();


	/**
	 * @brief 윈도우 창을 생성합니다.
	 * 
	 * @param windowParam 윈도우 생성에 사용할 파라미터입니다.
	 * 
	 * @return 윈도우 생성에 성공하면 Errors 열거형의 OK 값을, 그렇지 않으면 그 외의 값을 반환합니다.
	 */
	static Errors CreatePlatformWindow(const WindowConstructParams& windowParam);


	/**
	 * @brief 윈도우 프로시저 함수가 PlatformModule 내부를 참조할 수 있도록 friend 선언 추가
	 */
	friend LRESULT CALLBACK WindowProc(HWND hwnd, uint32_t message, WPARAM wParam, LPARAM lParam);


private:
	/**
	 * @brief 플랫폼 모듈이 초기화된 적이 있는지 확인합니다.
	 */
	static bool bIsInit;


	/**
	 * @brief 풀 스크린 모드인지 확인합니다.
	 */
	static bool bIsFullscreenMode;


	/**
	 * @brief 윈도우 창이 최소화 상태인지 확인합니다.
	 */
	static bool bIsMinimize;


	/**
	 * @brief 윈도우 창이 최대화 상태인지 확인합니다.
	 */
	static bool bIsMaximize;


	/**
	 * @brief 프로그램의 핸들 값입니다.
	 */
	static HINSTANCE instance;


	/**
	 * @brief 윈도우 클래스 이름입니다.
	 */
	static std::wstring className;


	/**
	 * @brief 윈도우 핸들 값입니다.
	 */
	static HWND windowHandle;


	/**
	 * @brief 문자열 버퍼의 최대 크기입니다.
	 */
	static const uint32_t MAX_BUFFER_SIZE = 1024;


	/**
	 * @brief 최근 에러 메시지입니다.
	 */
	static wchar_t lastErrorMessage[MAX_BUFFER_SIZE];


	/**
	 * @brief 루프를 종료할 지 확인합니다.
	 */
	static bool bIsQuitLoop;


	/**
	 * @brief 루프 실행 종료 시 실행할 콜백 함수입니다.
	 */
	static std::function<void()> endLoopCallback;


	/**
	 * @brief 루프 실행 시 호출할 프레임 단위의 콜백 함수입니다.
	 */
	static std::function<void(float)> frameCallback;
};
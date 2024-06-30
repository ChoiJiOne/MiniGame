#pragma once

#include <functional>
#include <string>


namespace GameMaker
{
/**
 * @brief 윈도우 생성 옵션입니다.
 * 
 * @note 풀 스크린으로 설정하면 윈도우의 일부 옵션이 무시됩니다.
 */
struct WindowParam
{
	std::string title;
	int32_t x;
	int32_t y;
	int32_t w;
	int32_t h;
	bool bIsResizble = false; // 윈도우의 크기 변경 여부입니다. 기본 값은 false입니다.
	bool bIsFullscreen = false; // 풀 스크린 여부입니다. 기본 값은 false입니다.
};


/**
 * @brief 게임 엔진 클래스입니다.
 *
 * @note 이 클래스의 모든 멤버 함수 및 변수는 모두 정적(static) 타입입니다.
 */
class GameEngine
{
public:
	/**
	 * @brief 게임 엔진을 초기화합니다.
	 * 
	 * @param param 윈도우 생성 시 참조할 파라미터입니다.
	 */
	static void Init(const WindowParam& param);


	/**
	 * @brief 게임 엔진의 초기화를 해제합니다.
	 */
	static void Shutdown();


	/**
	 * @brief 게임 엔진의 루프를 실행합니다.
	 *
	 * @param callback 루프 실행 시 호출할 프레임 단위의 콜백 함수입니다.
	 */
	static void RunLoop(const std::function<void(float)>& callback);


	/**
	 * @brief 루프 실행 종료 시 호출할 콜백 함수를 설정합니다.
	 *
	 * @param callback 루프 실행 종료 시 호출할 콜백 함수입니다.
	 */
	static void SetEndLoopCallback(const std::function<void()>& callback) { endLoopCallback_ = callback; }


	/**
	 * @brief 게임 윈도우의 포인터를 얻습니다.
	 *
	 * @return 게임 윈도우의 포인터를 반환합니다.
	 */
	static void* GetGameWindow() { return window_; }


	/**
	 * @brief 게임 윈도우 종료 여부를 설정합니다.
	 *
	 * @param bShouldCloseWindow 설정할 게임 윈도우 종료 여부입니다. 종료하고 싶다면 true, 그렇지 않으면 false입니다.
	 */
	static void SetShouldCloseWindow(bool bShouldCloseWindow) { bShouldCloseWindow_ = bShouldCloseWindow; }


private:
	/**
	 * @brief 게임 엔진의 하위 시스템을 초기화합니다.
	 */
	static void InitSubSystem();


private:
	/**
	 * @brief 게임 엔진이 초기화 되었는지 확인합니다.
	 */
	static bool bIsInit_;


	/**
	 * @brief 게임 윈도우의 포인터입니다.
	 */
	static void* window_;


	/**
	 * @brief 게임 윈도우 종료 여부입니다.
	 */
	static bool bShouldCloseWindow_;


	/**
	 * @brief 루프 실행 종료 시 실행할 콜백 함수입니다.
	 */
	static std::function<void()> endLoopCallback_;


	/**
	 * @brief 루프 실행 시 호출할 프레임 단위의 콜백 함수입니다.
	 */
	static std::function<void(float)> frameCallback_;
};

}
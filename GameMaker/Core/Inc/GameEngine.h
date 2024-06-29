#pragma once

#include <functional>


namespace GameMaker
{
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
	 */
	static void Init();


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
	static void* GetGameWindow() { return gameWindow_; }


	/**
	 * @brief 루프 종료 여부를 설정합니다.
	 *
	 * @param bIsQuit 설정할 루프 종료 여부입니다. 루프를 종료하고 싶다면 true, 그렇지 않으면 false입니다.
	 */
	static void SetQuitLoop(bool bIsQuit) { bIsQuitLoop_ = bIsQuit; }


private:
	/**
	 * @brief 게임 엔진이 초기화 되었는지 확인합니다.
	 */
	static bool bIsInit_;


	/**
	 * @brief 게임 윈도우의 포인터입니다.
	 */
	static void* gameWindow_;


	/**
	 * @brief 루프를 종료할 지 여부입니다.
	 */
	static bool bIsQuitLoop_;


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
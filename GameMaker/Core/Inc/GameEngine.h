#pragma once

#include <functional>
#include <string>


namespace GameMaker
{
struct WindowParam
{
	std::string title;
	int32_t x;
	int32_t y;
	int32_t w;
	int32_t h;
	bool bIsResizble = false; /** 윈도우의 크기 변경 여부입니다.기본 값은 false입니다. */
	bool bIsFullscreen = false; /** 풀 스크린 여부입니다. 기본 값은 false입니다. */
};

class GameEngine
{
public:
	static void Init(const WindowParam& param);
	static void Shutdown();
	static void RunLoop(const std::function<void(float)>& callback);

	static void SetEndLoopCallback(const std::function<void()>& callback) { endLoopCallback_ = callback; }
	static void* GetGameWindow() { return window_; }
	static void SetShouldCloseWindow(bool bShouldCloseWindow) { bShouldCloseWindow_ = bShouldCloseWindow; }

private:
	static void PreInit(); /** Init 호출 이전에 적절한 초기화 수행 */
	static void PostInit(); /** Init 호출 이후에 적절한 초기화 수행 */
	
private:
	static bool bIsInit_;
	static void* window_;
	static bool bShouldCloseWindow_;
	static std::function<void()> endLoopCallback_;
	static std::function<void(float)> frameCallback_;
};

}
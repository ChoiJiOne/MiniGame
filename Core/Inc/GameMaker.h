#pragma once

#include <functional>

#include "GameError.h"

namespace GameMaker
{
	/**  윈도우의 크기 변경과 풀 스크린 여부의 기본 값은 false입니다.  */
	GameError Startup(const char* title, int32_t x, int32_t y, int32_t w, int32_t h, bool bIsResizble = false, bool bIsFullscreen = false);
	GameError Shutdown();

	void RunLoop(const std::function<void(float)>& frameCallback);
}
/** 서드 파티 라이브러리 사용을 위한 헤더 선언 시작 */
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>

#include <SDL2/SDL.h>

#include <mimalloc-new-delete.h>
/** 서드 파티 라이브러리 사용을 위한 헤더 선언 종료*/

#include "GameMaker.h"

bool bIsStartup = false;

GameError GameMaker::Startup()
{
	if (bIsStartup)
	{
		return GameError(ErrorCode::FAILED_STARTUP, "Already startup GameMaker.");
	}

	return GameError(ErrorCode::OK, "Succeed startup GameMaker.");
}

GameError GameMaker::Shutdown()
{
	if (!bIsStartup)
	{
		return GameError(ErrorCode::FAILED_SHUTDOWN, "Startup has not called, or Shutdown has already been invoked.");
	}

	return GameError(ErrorCode::OK, "Succeed shutdown GameMaker.");
}

void GameMaker::RunLoop(const std::function<void(float)>& callback)
{
}

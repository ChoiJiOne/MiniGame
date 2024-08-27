#pragma once

#include <functional>

#include "GameError.h"

namespace GameMaker
{
	GameError Startup();
	GameError Shutdown();
	void RunLoop(const std::function<void(float)>& callback);
}
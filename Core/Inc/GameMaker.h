#pragma once

#include <functional>

#include "GameError.h"

namespace GameMaker
{
	GameError Startup();
	GameError Shutdown();
	GameError RunLoop(const std::function<void(float)>& callback);
}
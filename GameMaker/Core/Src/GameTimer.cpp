#include <SDL2/SDL.h>

#include "Assertion.h"
#include "GameTimer.h"

using namespace GameMaker;

float GameTimer::GetDeltaSeconds() const
{
	if (bIsStop_)
	{
		return 0.0f;
	}
	else
	{
		return static_cast<float>(currTime_ - prevTime_) * 0.001f; 
	}
}

float GameTimer::GetTotalSeconds() const
{
	if (bIsStop_)
	{
		return static_cast<float>(stopTime_ - pausedTime_ - baseTime_) * 0.001f;
	}
	else
	{
		return static_cast<float>(currTime_ - pausedTime_ - baseTime_) * 0.001f;
	}
}

void GameTimer::Reset()
{
	uint64_t tick = SDL_GetTicks64();

	bIsStop_ = false;
	baseTime_ = tick;
	pausedTime_ = 0ULL;
	stopTime_ = 0ULL;
	prevTime_ = tick;
	currTime_ = tick;
}

void GameTimer::Start()
{
	if (bIsStop_)
	{
		uint64_t tick = SDL_GetTicks64();

		pausedTime_ += (tick - stopTime_);
		prevTime_ = tick;
		stopTime_ = 0ULL;

		bIsStop_ = false;
	}
}

void GameTimer::Stop()
{
	if (!bIsStop_)
	{
		stopTime_ = SDL_GetTicks64();
		bIsStop_ = true;
	}
}

void GameTimer::Tick()
{
	prevTime_ = currTime_;
	currTime_ = SDL_GetTicks64();
}
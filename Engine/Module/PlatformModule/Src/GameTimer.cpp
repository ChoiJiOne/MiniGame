#include <windows.h>

#include "GameTimer.h"

GameTimer::GameTimer()
{
	int64_t counterPerSeconds;
	QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&counterPerSeconds));

	secondsPerCounter_ = 1.0 / static_cast<double>(counterPerSeconds);
}

float GameTimer::GetDeltaSeconds() const
{
	if (bIsStop_)
	{
		return 0.0f;
	}
	else
	{
		return static_cast<float>(currTime_ - prevTime_) * static_cast<float>(secondsPerCounter_);
	}
}

float GameTimer::GetTotalSeconds() const
{
	if (bIsStop_)
	{
		return static_cast<float>(stopTime_ - pausedTime_ - baseTime_) * static_cast<float>(secondsPerCounter_);
	}
	else
	{
		return static_cast<float>(currTime_ - pausedTime_ - baseTime_) * static_cast<float>(secondsPerCounter_);
	}
}

void GameTimer::Reset()
{
	uint64_t ticks;
	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&ticks));

	bIsStop_ = false;
	baseTime_ = ticks;
	pausedTime_ = 0ULL;
	stopTime_ = 0ULL;
	prevTime_ = ticks;
	currTime_ = ticks;
}

void GameTimer::Start()
{
	if (bIsStop_)
	{
		uint64_t ticks;
		QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&ticks));

		pausedTime_ += (ticks - stopTime_);
		prevTime_ = ticks;
		stopTime_ = 0ULL;

		bIsStop_ = false;
	}
}

void GameTimer::Stop()
{
	if (!bIsStop_)
	{
		uint64_t ticks;
		QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&ticks));

		stopTime_ = ticks;

		bIsStop_ = true;
	}
}

void GameTimer::Tick()
{
	uint64_t ticks;
	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&ticks));

	prevTime_ = currTime_;
	currTime_ = ticks;
}
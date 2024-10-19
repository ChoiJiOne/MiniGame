#include <windows.h>

#include <SDL2/SDL.h>

#include "GameTimer.h"
#include "GameUtils.h"

GameTimer::GameTimer(GameTimer&& instance) noexcept
	: bIsStop_(instance.bIsStop_)
	, baseTime_(instance.baseTime_)
	, pausedTime_(instance.pausedTime_)
	, stopTime_(instance.stopTime_)
	, prevTime_(instance.prevTime_)
	, currTime_(instance.currTime_) {}

GameTimer::GameTimer(const GameTimer& instance) noexcept
	: bIsStop_(instance.bIsStop_)
	, baseTime_(instance.baseTime_)
	, pausedTime_(instance.pausedTime_)
	, stopTime_(instance.stopTime_)
	, prevTime_(instance.prevTime_)
	, currTime_(instance.currTime_) {}

GameTimer& GameTimer::operator=(GameTimer&& instance) noexcept
{
	if (this == &instance) return *this;

	bIsStop_ = instance.bIsStop_;
	baseTime_ = instance.baseTime_;
	pausedTime_ = instance.pausedTime_;
	stopTime_ = instance.stopTime_;
	prevTime_ = instance.prevTime_;
	currTime_ = instance.currTime_;

	return *this;
}

GameTimer& GameTimer::operator=(const GameTimer& instance) noexcept
{
	if (this == &instance) return *this;

	bIsStop_ = instance.bIsStop_;
	baseTime_ = instance.baseTime_;
	pausedTime_ = instance.pausedTime_;
	stopTime_ = instance.stopTime_;
	prevTime_ = instance.prevTime_;
	currTime_ = instance.currTime_;

	return *this;
}

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

void GameTimer::GetCurrentSystemTime(int32_t& outYear, int32_t& outMonth, int32_t& outDay, int32_t& outHour, int32_t& outMinute, int32_t& outSecond)
{
	SYSTEMTIME systemTime;
	GetLocalTime(&systemTime);

	outYear = static_cast<int32_t>(systemTime.wYear);
	outMonth = static_cast<int32_t>(systemTime.wMonth);
	outDay = static_cast<int32_t>(systemTime.wDay);
	outHour = static_cast<int32_t>(systemTime.wHour);
	outMinute = static_cast<int32_t>(systemTime.wMinute);
	outSecond = static_cast<int32_t>(systemTime.wSecond);
}

std::string GameTimer::GetCurrentSystemTimeAsString()
{
	int32_t year = 0;
	int32_t month = 0;
	int32_t day = 0;
	int32_t hour = 0;
	int32_t minute = 0;
	int32_t second = 0;
	GetCurrentSystemTime(year, month, day, hour, minute, second);

	return GameUtils::PrintF("%4d-%02d-%02d-%02d-%02d-%02d", year, month, day, hour, minute, second);
}

std::wstring GameTimer::GetCurrentSystemTimeAsWString()
{
	int32_t year = 0;
	int32_t month = 0;
	int32_t day = 0;
	int32_t hour = 0;
	int32_t minute = 0;
	int32_t second = 0;
	GetCurrentSystemTime(year, month, day, hour, minute, second);

	return GameUtils::PrintF(L"%4d-%02d-%02d-%02d-%02d-%02d", year, month, day, hour, minute, second);
}
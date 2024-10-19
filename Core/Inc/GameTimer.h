#pragma once

#include <string>
#include <cstdint>

class GameTimer
{
public:
	GameTimer() = default;
	GameTimer(GameTimer&& instance) noexcept;
	GameTimer(const GameTimer& instance) noexcept;
	virtual ~GameTimer() = default;

	GameTimer& operator=(GameTimer&& instance) noexcept;
	GameTimer& operator=(const GameTimer& instance) noexcept;

	float GetDeltaSeconds() const;
	float GetTotalSeconds() const;
	void Reset();
	void Start();
	void Stop();
	void Tick();

	static void GetCurrentSystemTime(int32_t& outYear, int32_t& outMonth, int32_t& outDay, int32_t& outHour, int32_t& outMinute, int32_t& outSecond);
	static std::string GetCurrentSystemTimeAsString();
	static std::wstring GetCurrentSystemTimeAsWString();

private:
	bool bIsStop_ = false;
	uint64_t baseTime_ = 0LL;
	uint64_t pausedTime_ = 0LL;
	uint64_t stopTime_ = 0LL;
	uint64_t prevTime_ = 0LL;
	uint64_t currTime_ = 0LL;
};
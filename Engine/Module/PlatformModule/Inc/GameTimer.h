#pragma once

#include <cstdint>


/**
 * @brief 게임 내에서 시간을 측정하는 타이머입니다.
 */
class GameTimer
{
public:
	/**
	 * @brief 타이머 클래스의 생성자입니다.
	 *
	 * @note 타이머 클래스의 생성자는 초기화를 수행하지 않으므로 반드시 Start 혹은 Reset 메서드를 호출해야 합니다.
	 */
	GameTimer();


	/**
	 * @brief 타이머 클래스의 복사 생성자입니다.
	 *
	 * @param instance 복사할 객체입니다.
	 */
	GameTimer(GameTimer&& instance) noexcept
		: bIsStop_(instance.bIsStop_)
		, baseTime_(instance.baseTime_)
		, pausedTime_(instance.pausedTime_)
		, stopTime_(instance.stopTime_)
		, prevTime_(instance.prevTime_)
		, currTime_(instance.currTime_) {}


	/**
	 * @brief 타이머 클래스의 복사 생성자입니다.
	 *
	 * @param instance 복사할 객체입니다.
	 */
	GameTimer(const GameTimer& instance) noexcept
		: bIsStop_(instance.bIsStop_)
		, baseTime_(instance.baseTime_)
		, pausedTime_(instance.pausedTime_)
		, stopTime_(instance.stopTime_)
		, prevTime_(instance.prevTime_)
		, currTime_(instance.currTime_) {}


	/**
	 * @brief 타이머 클래스의 가상 소멸자입니다.
	 */
	virtual ~GameTimer() {};


	/**
	 * @brief 타이머 클래스의 대입 연산자입니다.
	 *
	 * @param instance 복사할 객체입니다.
	 *
	 * @return 복사한 객체의 참조자를 반환합니다.
	 */
	GameTimer& operator=(GameTimer&& instance) noexcept
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


	/**
	 * @brief 타이머 클래스의 대입 연산자입니다.
	 *
	 * @param instance 복사할 객체입니다.
	 *
	 * @return 복사한 객체의 참조자를 반환합니다.
	 */
	GameTimer& operator=(const GameTimer& instance) noexcept
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


	/**
	 * @brief 타이머의 초 단위 델타 시간값을 얻습니다.
	 *
	 * @return 초 단위의 델타 시간값을 반환합니다.
	 */
	float GetDeltaSeconds() const;


	/**
	 * @brief 타이머가 중지된 시간을 제외한 전체 초단위 시간값을 반환합니다.
	 *
	 * @return 타이머가 시작된 이후의 중지된 시간을 제외한 전체 시간값을 반환합니다.
	 */
	float GetTotalSeconds() const;


	/**
	 * @brief 타이머의 모든 상태를 리셋합니다.
	 */
	void Reset();


	/**
	 * @brief 타이머를 시작합니다.
	 *
	 * @note 타이머가 중지되어 있다면 중시된 시점부터 시작됩니다.
	 */
	void Start();


	/**
	 * @brief 타이머를 중지시킵니다.
	 */
	void Stop();


	/**
	 * @brief 타이머를 업데이트합니다.
	 */
	void Tick();


private:
	/**
	 * @brief 타이머의 중지 여부를 확인합니다.
	 */
	bool bIsStop_ = false;


	/**
	 * @brief 타이머가 시작된 시간입니다.
	 */
	uint64_t baseTime_ = 0LL;


	/**
	 * @brief 타이머가 중지된 동안의 누적 시간값 입니다.
	 */
	uint64_t pausedTime_ = 0LL;


	/**
	 * @brief 타이머가 중지된 시간입니다.
	 */
	uint64_t stopTime_ = 0LL;


	/**
	 * @brief 직전의 Tick 호출 시간입니다.
	 */
	uint64_t prevTime_ = 0LL;


	/**
	 * @brief Tick 호출 시간입니다.
	 */
	uint64_t currTime_ = 0LL;


	/**
	 * @brief 성능 카운터 빈도의 역수값입니다.
	 */
	double secondsPerCounter_ = 0.0;
};
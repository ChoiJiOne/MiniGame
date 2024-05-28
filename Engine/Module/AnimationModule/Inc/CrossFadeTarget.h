#pragma once

#include "Clip.h"
#include "Pose.h"


/**
 * @brief 애니메이션 패이딩 시 샘플링된 포즈의 상태 값입니다.
 */
struct CrossFadeTarget
{
	/**
	 * @brief 크로스 페이드 타겟의 기본 생성자입니다.
	 */
	CrossFadeTarget() = default;


	/**
	 * @brief 크로스 페이드 타겟의 생성자입니다.
	 * 
	 * @param targetClip 크로스 페이드 타겟의 클립입니다.
	 * @param targetPose 크로스 페이드 타겟의 포즈입니다.
	 * @param targetDuration 크로스 페이드 타겟의 애니메이션 지속 시간입니다.
	 */
	CrossFadeTarget(Clip* targetClip, const Pose& targetPose, float targetDuration)
		: clip(targetClip)
		, pose(targetPose)
		, time(targetClip->GetStartTime())
		, duration(targetDuration)
		, elapsed(0.0f) {}


	/**
	 * @brief 크로스 페이드 타겟의 복사 생성자입니다.
	 * 
	 * @param instance 복사할 인스턴스입니다.
	 */
	CrossFadeTarget(CrossFadeTarget&& instance) noexcept
		: pose(instance.pose)
		, clip(instance.clip)
		, time(instance.time)
		, duration(instance.duration)
		, elapsed(instance.elapsed) {}


	/**
	 * @brief 크로스 페이드 타겟의 복사 생성자입니다.
	 *
	 * @param instance 복사할 인스턴스입니다.
	 */
	CrossFadeTarget(const CrossFadeTarget& instance) noexcept
		: pose(instance.pose)
		, clip(instance.clip)
		, time(instance.time)
		, duration(instance.duration)
		, elapsed(instance.elapsed) {}


	/**
	 * @brief 크로스 페이드의 가상 소멸자입니다.
	 */
	virtual ~CrossFadeTarget() {}


	/**
	 * @brief 크로스 페이드 타겟의 대입 연산자입니다.
	 * 
	 * @param instance 대입 연산을 수행할 인스턴스입니다.
	 * 
	 * @return 대입 연산을 수행한 객체의 참조자를 반환합니다.
	 */
	CrossFadeTarget& operator=(CrossFadeTarget&& instance) noexcept
	{
		if (this == &instance) return *this;

		pose = instance.pose;
		clip = instance.clip;
		time = instance.time;
		duration = instance.duration;
		elapsed = instance.elapsed;

		return *this;
	}


	/**
	 * @brief 크로스 페이드 타겟의 대입 연산자입니다.
	 *
	 * @param instance 대입 연산을 수행할 인스턴스입니다.
	 *
	 * @return 대입 연산을 수행한 객체의 참조자를 반환합니다.
	 */
	CrossFadeTarget& operator=(const CrossFadeTarget& instance) noexcept
	{
		if (this == &instance) return *this;

		pose = instance.pose;
		clip = instance.clip;
		time = instance.time;
		duration = instance.duration;
		elapsed = instance.elapsed;

		return *this;
	}


	/**
	 * @brief 애니메이션 패이딩 시 샘플링할 포즈입니다.
	 */
	Pose pose;


	/**
	 * @brief 현재의 애니메이션 클립입니다.
	 */
	Clip* clip = nullptr;


	/**
	 * @brief 누적 시간값입니다.
	 */
	float time = 0.0f;


	/**
	 * @brief 애니메이션 지속 시간입니다.
	 */
	float duration = 0.0f;


	/**
	 * @brief 애니메이션 플레이 경과 시간입니다.
	 */
	float elapsed = 0.0f;
};
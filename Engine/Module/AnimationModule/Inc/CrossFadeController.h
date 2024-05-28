#pragma once

#include <vector>

#include "CrossFadeTarget.h"
#include "Skeleton.h"


/**
 * @brief 포즈를 블랜딩하는 크로스 페이드 컨트롤러입니다.
 */
class CrossFadeController
{
public:
	/**
	 * @brief 크로스 페이드 컨트롤러의 디폴트 생성자입니다
	 */
	CrossFadeController() = default;


	/**
	 * @brief 크로스 페이드 컨트롤러의 뼈대입니다.
	 * 
	 * @param skeleton 스켈레톤(뼈대)입니다.
	 */
	CrossFadeController(const Skeleton& skeleton);


	/**
	 * @brief 크로스 페이드 컨트롤러의 복사 생성자입니다.
	 * 
	 * @param instance 복사할 인스턴스입니다.
	 */
	CrossFadeController(CrossFadeController&& instance) noexcept
		: targets_(instance.targets_)
		, clip_(instance.clip_)
		, time_(instance.time_)
		, pose_(instance.pose_)
		, skeleton_(instance.skeleton_)
		, bWasSetSkeleton_(instance.bWasSetSkeleton_) {}


	/**
	 * @brief 크로스 페이드 컨트롤러의 복사 생성자입니다.
	 *
	 * @param instance 복사할 인스턴스입니다.
	 */
	CrossFadeController(const CrossFadeController& instance) noexcept
		: targets_(instance.targets_)
		, clip_(instance.clip_)
		, time_(instance.time_)
		, pose_(instance.pose_)
		, skeleton_(instance.skeleton_)
		, bWasSetSkeleton_(instance.bWasSetSkeleton_) {}


	/**
	 * @brief 크로스 페이드 컨트롤러의 가상 소멸자입니다.
	 */
	virtual ~CrossFadeController() {}


	/**
	 * @brief 크로스 페이드 컨트롤러의 대입 연산자입니다.
	 * 
	 * @param instance 대입 연산을 수행할 인스턴스입니다.
	 * 
	 * @return 대입 연산을 수행한 객체의 참조자를 반환합니다.
	 */
	CrossFadeController& operator=(CrossFadeController&& instance) noexcept
	{
		if (this == &instance) return *this;

		targets_ = instance.targets_;
		clip_ = instance.clip_;
		time_ = instance.time_;
		pose_ = instance.pose_;
		skeleton_ = instance.skeleton_;
		bWasSetSkeleton_ = instance.bWasSetSkeleton_;

		return *this;
	}


	/**
	 * @brief 크로스 페이드 컨트롤러의 대입 연산자입니다.
	 *
	 * @param instance 대입 연산을 수행할 인스턴스입니다.
	 *
	 * @return 대입 연산을 수행한 객체의 참조자를 반환합니다.
	 */
	CrossFadeController& operator=(const CrossFadeController& instance) noexcept
	{
		if (this == &instance) return *this;

		targets_ = instance.targets_;
		clip_ = instance.clip_;
		time_ = instance.time_;
		pose_ = instance.pose_;
		skeleton_ = instance.skeleton_;
		bWasSetSkeleton_ = instance.bWasSetSkeleton_;

		return *this;
	}


	/**
	 * @brief 크로스 페이드 컨트롤러의 뼈대를 설정합니다.
	 * 
	 * @param skeleton 설정할 스켈레톤(뼈대)입니다.
	 */
	void SetSkeleton(const Skeleton& skeleton);


	/**
	 * @brief 현재 크로스 페이드 컨트롤러의 뼈대를 얻습니다.
	 * 
	 * @return 현재 크로스 페이드 컨트롤러의 뼈대를 반환합니다.
	 */
	Skeleton& GetSkeleton() { return skeleton_; }


	/**
	 * @brief 현재 포즈를 얻습니다.
	 * 
	 * @return 현재 포즈의 참조자를 반환합니다.
	 */
	Pose& GetCurrentPose() { return pose_; }


	/**
	 * @brief 현재 클립을 얻습니다.
	 * 
	 * @return 현재 클립을 반환합니다.
	 */
	Clip* GetCurrentClip() { return clip_; }


	/**
	 * @brief 현재 플레이 시간 값을 얻습니다.
	 * 
	 * @return 현재 플레이 시간 값을 반환합니다.
	 */
	float GetTime() const { return time_; }


	/**
	 * @brief 활성화된 크로스 페이드를 삭제하고 클립 및 재생 시간을 재설정합니다.
	 * 
	 * @param target 재설정할 클립입니다.
	 */
	void Play(Clip* target);


	/**
	 * @brief 애니메이션 클립과 지속 시간을 페이드 목록에 추가합니다.
	 * 
	 * @param target 페이드 목록에 추가할 애니메이션 클립입니다.
	 * @param fadeTime 애니메이션 클립 페이드 시간입니다.
	 */
	void FadeTo(Clip* target, float fadeTime);


	/**
	 * @brief 크로스 페이드 컨트롤러를 업데이트합니다.
	 * 
	 * @param deltaSeconds 초단위 델타 시간값입니다.
	 */
	void Update(float deltaSeconds);


private:
	/**
	 * @brief 크로스 페이드 타겟 목록입니다.
	 */
	std::vector<CrossFadeTarget> targets_;


	/**
	 * @brief 크로스 페이드의 클립입니다.
	 */
	Clip* clip_ = nullptr;


	/**
	 * @brief 플레이 시간 값입니다.
	 */
	float time_ = 0.0f;


	/**
	 * @brief 크로스 페이드 포즈입니다.
	 */
	Pose pose_;


	/**
	 * @brief 크로스 페이드 스켈레톤(뼈대)입니다.
	 */
	Skeleton skeleton_;


	/**
	 * @brief 스켈레톤이 업데이트되었는지 확인합니다.
	 */
	bool bWasSetSkeleton_ = false;
};
#pragma once

#include <string>
#include <vector>

#include "Mat.h"

#include "Pose.h"


/**
 * @brief 스켈레톤(뼈대)입니다.
 */
class Skeleton
{
public:
	/**
	 * @brief 스켈레톤(뼈대)의 디폴트 생성자입니다.
	 */
	Skeleton() = default;


	/**
	 * @brief 스켈레톤(뼈대)의 생성자입니다.
	 * 
	 * @param restPose 휴식 포즈입니다.
	 * @param bindPose 바인드 포즈입니다.
	 * @param jointNames 관절(Bone) 이름 목록입니다.
	 */
	Skeleton(const Pose& restPose, const Pose& bindPose, const std::vector<std::string>& jointNames)
	{
		SetProperties(restPose, bindPose, jointNames);
	}


	/**
	 * @brief 스켈레톤(뼈대)의 복사 생성자입니다.
	 * 
	 * @param instance 복사할 인스턴스입니다.
	 */
	Skeleton(Skeleton&& instance) noexcept
		: restPose_(instance.restPose_)
		, bindPose_(instance.bindPose_)
		, invBindPose_(instance.invBindPose_)
		, jointNames_(instance.jointNames_) {}


	/**
	 * @brief 스켈레톤(뼈대)의 복사 생성자입니다.
	 * 
	 * @param instance 복사할 인스턴스입니다.
	 */
	Skeleton(const Skeleton& instance) noexcept
		: restPose_(instance.restPose_)
		, bindPose_(instance.bindPose_)
		, invBindPose_(instance.invBindPose_)
		, jointNames_(instance.jointNames_) {}


	/**
	 * @brief 스켈레톤(뼈대)의 가상 소멸자입니다.
	 */
	virtual ~Skeleton() {}


	/**
	 * @brief 스켈레톤(뼈대)의 대입 연산자입니다.
	 * 
	 * @param instance 대입 연산을 수행할 인스턴스입니다.
	 * 
	 * @return 대입 연산을 수행한 인스턴스의 참조자를 반환합니다.
	 */
	Skeleton& operator=(Skeleton&& instance) noexcept
	{
		if (this == &instance) return *this;

		restPose_ = instance.restPose_;
		bindPose_ = instance.bindPose_;
		invBindPose_ = instance.invBindPose_;
		jointNames_ = instance.jointNames_;

		return *this;
	}


	/**
	 * @brief 스켈레톤(뼈대)의 대입 연산자입니다.
	 *
	 * @param instance 대입 연산을 수행할 인스턴스입니다.
	 *
	 * @return 대입 연산을 수행한 인스턴스의 참조자를 반환합니다.
	 */
	Skeleton& operator=(const Skeleton& instance) noexcept
	{
		if (this == &instance) return *this;

		restPose_ = instance.restPose_;
		bindPose_ = instance.bindPose_;
		invBindPose_ = instance.invBindPose_;
		jointNames_ = instance.jointNames_;

		return *this;
	}


	/**
	 * @brief 바인드 포즈를 얻습니다.
	 * 
	 * @return 바인드 포즈의 참조자를 반환합니다.
	 */
	Pose& GetBindPose() { return bindPose_; }


	/**
	 * @brief 휴식 포즈를 얻습니다.
	 * 
	 * @return 휴식 포즈의 참조자를 반환합니다.
	 */
	Pose& GetRestPose() { return restPose_; }


	/**
	 * @brief 역 바인드 포즈 행렬을 얻습니다.
	 * 
	 * @return 역 바인드 포즈 행렬의 벡터 참조자를 반환합니다.
	 */
	std::vector<Mat4x4>& GetInvBindPose() { return invBindPose_; }


	/**
	 * @brief 관절(Bone) 이름 목록을 얻습니다.
	 * 
	 * @return 관절(Bone) 이름 목록의 벡터 참조자를 반환합니다.
	 */
	std::vector<std::string>& GetJointNames() { return jointNames_; }


	/**
	 * @brief 인덱스에 대응하는 관절 이름을 얻습니다.
	 * 
	 * @param index 관절 이름 목록의 인덱스입니다.
	 * 
	 * @return 인덱스에 대응하는 관절 이름을 반환합니다.
	 */
	std::string& GetJointName(uint32_t index) { return jointNames_[index]; }


	/**
	 * @brief 내부 프로퍼티를 설정합니다.
	 * 
	 * @param restPose 휴식 포즈입니다.
	 * @param bindPose 바인드 포즈입니다.
	 * @param jointNames 관절(Bone) 이름 목록입니다.
	 */
	void SetProperties(const Pose& restPose, const Pose& bindPose, const std::vector<std::string>& jointNames);


private:
	/**
	 * @brief 역 바인드 포즈 행렬 목록을 업데이트합니다.
	 */
	void UpdateInverseBindPose();


private:
	/**
	 * @brief 뼈대의 Rest 포즈입니다.
	 */
	Pose restPose_;


	/**
	 * @brief 뼈대의 Bind 포즈입니다.
	 */
	Pose bindPose_;


	/**
	 * @brief 역 바인드 포즈 행렬 목록입니다.
	 */
	std::vector<Mat4x4> invBindPose_;


	/**
	 * @brief 관절(Bone) 이름 목록입니다.
	 */
	std::vector<std::string> jointNames_;
};
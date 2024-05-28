#pragma once

#include <vector>

#include "Transform.h"


/**
 * @brief 애니메이션화할 캐릭터의 골격에서 모든 관절의 변화를 추적하는 클래스입니다.
 */
class Pose
{
public:
	/**
	 * @brief 포즈 클래스의 디폴트 생성자입니다.
	 */
	Pose() = default;


	/**
	 * @brief 포즈 클래스의 생성자입니다.
	 * 
	 * @param numJoints 관절 변환 목록 크기입니다.
	 */
	Pose(uint32_t numJoints)
	{
		Resize(numJoints);
	}


	/**
	 * @brief 포즈 클래스의 복사 생성자입니다.
	 * 
	 * @param instance 복사할 인스턴스입니다.
	 */
	Pose(Pose&& instance) noexcept
		: joints_(instance.joints_)
		, parents_(instance.parents_) {}


	/**
	 * @brief 포즈 클래스의 복사 생성자입니다.
	 *
	 * @param instance 복사할 인스턴스입니다.
	 */
	Pose(const Pose& instance) noexcept
		: joints_(instance.joints_)
		, parents_(instance.parents_) {}


	/**
	 * @brief 포즈 클래스의 가상 소멸자입니다.
	 */
	virtual ~Pose() {}


	/**
	 * @brief 포즈 클래스의 대입 연산자입니다.
	 * 
	 * @param instance 대입 연산을 수행할 인스턴스입니다.
	 * 
	 * @return 대입 연산을 수행한 객체의 참조자를 반환합니다.
	 */
	Pose& operator=(Pose&& instance) noexcept;


	/**
	 * @brief 포즈 클래스의 대입 연산자입니다.
	 *
	 * @param instance 대입 연산을 수행할 인스턴스입니다.
	 *
	 * @return 대입 연산을 수행한 객체의 참조자를 반환합니다.
	 */
	Pose& operator=(const Pose& instance) noexcept;


	/**
	 * @brief 두 포즈가 서로 일치하는지 확인합니다.
	 *
	 * @param instance 일치하는지 확인할 인스턴스입니다.
	 *
	 * @return 두 포즈가 일치한다면 true, 그렇지 않으면 false입니다.
	 */
	bool operator==(Pose&& instance) noexcept;


	/**
	 * @brief 두 포즈가 서로 일치하는지 확인합니다.
	 * 
	 * @param instance 일치하는지 확인할 인스턴스입니다.
	 * 
	 * @return 두 포즈가 일치한다면 true, 그렇지 않으면 false입니다.
	 */
	bool operator==(const Pose& instance);


	/**
	 * @brief 두 포즈가 서로 일치하지 않는지 확인합니다.
	 *
	 * @param instance 일치하지 않는지 확인할 인스턴스입니다.
	 *
	 * @return 두 포즈가 일치하지 않는다면 true, 그렇지 않으면 false입니다.
	 */
	bool operator!=(Pose&& instance) noexcept;


	/**
	 * @brief 두 포즈가 서로 일치하지 않는지 확인합니다.
	 *
	 * @param instance 일치하지 않는지 확인할 인스턴스입니다.
	 *
	 * @return 두 포즈가 일치하지 않는다면 true, 그렇지 않으면 false입니다.
	 */
	bool operator!=(const Pose& instance);


	/**
	 * @brief 포즈 내의 목록의 크기를 변경합니다.
	 * 
	 * @param size 변경할 크기입니다.
	 */
	void Resize(uint32_t size);


	/**
	 * @brief 포즈 내의 관절 변환 목록 크기를 얻습니다.
	 * 
	 * @return 포즈 내의 관절 변환 목록 크기를 반환합니다.
	 */
	uint32_t Size() const { return static_cast<uint32_t>(joints_.size()); }


	/**
	 * @brief 인덱스에 대응하는 관절 변환 값을 얻습니다.
	 * 
	 * @param index 관절 변환 값을 얻을 인덱스입니다.
	 * 
	 * @return 인덱스에 대응하는 관절 변환 값을 반환합니다.
	 */
	Transform GetLocalTransform(uint32_t index);


	/**
	 * @brief 인덱스에 대응하는 관절 변환 값을 설정합니다.
	 *
	 * @param index 관절 변환 값을 설정할 인덱스입니다.
	 * @param transform 설정할 변환 값입니다.
	 */
	void SetLocalTransform(uint32_t index, const Transform& transform);


	/**
	 * @brief 인덱스에 대응하는 글로벌 관절 변환 값을 얻습니다.
	 * 
	 * @param index 관절 변환 값을 얻을 인덱스입니다.
	 * 
	 * @return 인덱스에 대응하는 글로벌 관절 변환 값을 반환합니다.
	 */
	Transform GetGlobalTransform(uint32_t index);


	/**
	 * @brief 인덱스에 대응하는 글로벌 관절 변환 값을 얻습니다.
	 *
	 * @param index 관절 변환 값을 얻을 인덱스입니다.
	 *
	 * @return 인덱스에 대응하는 글로벌 관절 변환 값을 반환합니다.
	 */
	Transform operator[](uint32_t index);


	/**
	 * @brief 포즈 내의 변환을 행렬 목록으로 변환합니다.
	 * 
	 * @param outMatrixPalette 행렬 목록을 저장할 벡터입니다.
	 */
	void GetMatrixPalette(std::vector<Mat4x4>& outMatrixPalette);


	/**
	 * @brief 인덱스에 대응하는 부모 뼈대 값을 얻습니다.
	 * 
	 * @param index 부모 뼈대 값을 얻을 인덱스 값입니다.
	 * 
	 * @return 인덱스에 대응하는 부모 뼈대 값을 반환합니다.
	 */
	int32_t GetParent(uint32_t index) { return parents_[index]; }


	/**
	 * @brief 부모 뼈대 값을 설정합니다.
	 * 
	 * @param index 부모 뼈대 값을 설정할 인덱스입니다.
	 * @param parent 설정할 부모 뼈대 값입니다.
	 */
	void SetParent(uint32_t index, int32_t parent) { parents_[index] = parent; }


	/**
	 * @brief 한 노드가 다른 노드의 계층 구조 내에 있는지 확인합니다.
	 * 
	 * @param pose 계층 구조를 나타내는 포즈입니다.
	 * @param parent 계층 내의 다른 노드 인덱스입니다.
	 * @param search 확인할 노드 인덱스입니다.
	 * 
	 * @return 한 노드가 다른 노드의 계층 구조 내에 있다면 true, 그렇지 않으면 false를 반환합니다.
	 */
	static bool IsInHierarchy(Pose& pose, uint32_t parent, uint32_t search);


	/**
	 * @brief 두 포즈를 블랜딩합니다.
	 * 
	 * @param output 블랜딩된 포즈입니다.
	 * @param start 블랜딩할 첫 번째 포즈(t=0 시 적용)입니다.
	 * @param end 블랜딩할 두 번째 포즈(t=1 시 적용)입니다.
	 * @param t 0과 1 사이의 블랜딩 값입니다.
	 * @param blendRoot 루트 노드입니다.
	 */
	static void Blend(Pose& output, Pose& start, Pose& end, float t, int32_t blendRoot);


private:
	/**
	 * @brief 관절 변환 목록입니다.
	 */
	std::vector<Transform> joints_;


	/**
	 * @brief 관절의 부모 인덱스 목록입니다.
	 */
	std::vector<int32_t> parents_;
};
#pragma once

#include <vector>

#include "Vec.h"
#include "Quat.h"

#include "Interpolation.h"
#include "Keyframe.h"


/**
 * @brief 애니메이션 키 프레임을 모아놓은 트랙입니다.
 */
template <typename T, uint32_t N>
class Track
{
public:
	/**
	 * @brief 애니메이션 트랙의 기본 생성자입니다.
	 */
	Track() = default;


	/**
	 * @brief 애니메이션 트랙의 복사 생성자입니다.
	 * 
	 * @param instance 복사할 인스턴스입니다.
	 */
	Track(Track&& instance) noexcept
		: keyframes_(instance.keyframes_)
		, interpolation_(instance.interpolation_) {}


	/**
	 * @brief 애니메이션 트랙의 복사 생성자입니다.
	 *
	 * @param instance 복사할 인스턴스입니다.
	 */
	Track(const Track& instance) noexcept 
		: keyframes_(instance.keyframes_)
		, interpolation_(instance.interpolation_) {}


	/**
	 * @brief 애니메이션 트랙의 가상 소멸자입니다.
	 */
	virtual ~Track() {}


	/**
	 * @brief 애니메이션 트랙의 대입 연산자입니다.
	 * 
	 * @param instance 대입 연산을 수행할 인스턴스입니다.
	 * 
	 * @return 대입 연산을 수행한 객체의 참조자를 반환합니다.
	 */
	Track& operator=(Track&& instance) noexcept
	{
		if (this == &instance) return *this;

		keyframes_ = instance.keyframes_;
		interpolation_ = instance.interpolation_;

		return *this;
	}


	/**
	 * @brief 애니메이션 트랙의 대입 연산자입니다.
	 *
	 * @param instance 대입 연산을 수행할 인스턴스입니다.
	 *
	 * @return 대입 연산을 수행한 객체의 참조자를 반환합니다.
	 */
	Track& operator=(const Track& instance) noexcept
	{
		if (this == &instance) return *this;

		keyframes_ = instance.keyframes_;
		interpolation_ = instance.interpolation_;

		return *this;
	}


	/**
	 * @brief 키 프레임 목록의 크기를 변경합니다.
	 * 
	 * @param size 변경할 키 프레임 목록의 크기입니다.
	 */
	void Resize(uint32_t size) { keyframes_.resize(size); }


	/**
	 * @brief 키 프레임 목록의 크기를 얻습니다.
	 * 
	 * @return 키 프레임 목록의 크기를 반환합니다.
	 */
	uint32_t Size() const { return static_cast<uint32_t>(keyframes_.size()); }


	/**
	 * @brief 키 프레임을 보간할 때의 유형을 얻습니다.
	 * 
	 * @return 키 프레임을 보간할 때의 유형 값을 반환합니다.
	 */
	const EInterpolation& GetInterpolation() const { return interpolation_; }


	/**
	 * @brief 키 프레임을 보간할 때의 유형을 설정합니다.
	 * 
	 * @param interpolation 설정할 키 프레임 보간 유형입니다.
	 */
	void SetInterpolation(const EInterpolation& interpolation) { interpolation_ = interpolation; }


	/**
	 * @brief 키 프레임의 시작 시간을 얻습니다.
	 * 
	 * @return 키 프레임의 시작 시간 값을 반환합니다.
	 */
	float GetStartTime() const { return keyframes_.front().time; }


	/**
	 * @brief 키 프레임의 끝 시간을 얻습니다.
	 * 
	 * @return 키 프레임의 끝 시간 값을 반환합니다.
	 */
	float GetEndTime() const { return keyframes_.back().time; }


	/**
	 * @brief 인덱스로 키 프레임 목록을 참조합니다.
	 * 
	 * @param index 키 프레임 목록의 인덱스입니다.
	 * 
	 * @return 인덱스에 대응하는 키 프레임 값을 반환합니다.
	 */
	Keyframe<N>& operator[](uint32_t index);


	/**
	 * @brief 시간 값에 맞게 트랙을 샘플링합니다.
	 * 
	 * @param time 트랙을 샘플링할 시간 값입니다.
	 * @param bIsLooping 트랙의 반복 여부입니다.
	 * 
	 * @return 샘플링된 값을 반환합니다.
	 */
	T Sample(float time, bool bIsLooping);


private:
	/**
	 * @brief 상수 샘플링을 수행합니다.
	 * 
	 * @param time 트랙을 샘플링할 시간 값입니다.
	 * @param bIsLooping 트랙의 반복 여부입니다.
	 * 
	 * @return 샘플링된 값을 반환합니다.
	 */
	T SampleConstant(float time, bool bIsLooping);


	/**
	 * @brief 선형 샘플링을 수행합니다.
	 *
	 * @param time 트랙을 샘플링할 시간 값입니다.
	 * @param bIsLooping 트랙의 반복 여부입니다.
	 *
	 * @return 샘플링된 값을 반환합니다.
	 */
	T SampleLinear(float time, bool bIsLooping);


	/**
	 * @brief 삼차(Cubic) 샘플링을 수행합니다.
	 *
	 * @param time 트랙을 샘플링할 시간 값입니다.
	 * @param bIsLooping 트랙의 반복 여부입니다.
	 *
	 * @return 샘플링된 값을 반환합니다.
	 */
	T SampleCubic(float time, bool bIsLooping);


	/**
	 * @brief 애미트 스플라인 계산을 수행합니다.
	 * 
	 * @param time 시간 값입니다.
	 * @param point1 곡선의 첫 번째 지점입니다.
	 * @param slope1 곡선의 첫 번째 기울기입니다.
	 * @param point2 곡선의 두 번째 지점입니다.
	 * @param slope2 곡선의 두 번째 기울기입니다.
	 * 
	 * @return 계산된 애미트 스플라인 값을 반환합니다.
	 */
	T Hermite(float time, const T& point1, const T& slope1, const T& point2, const T& slope2);


	/**
	 * @brief 시간 값에 대한 키 프레임 인덱스를 얻습니다.
	 * 
	 * @param time 키 프레임 인덱스를 얻을 시간값입니다.
	 * @param bIsLooping 트랙의 반복 여부입니다.
	 * 
	 * @return 키 프레임 인덱스 값을 반환합니다.
	 */
	int32_t FrameIndex(float time, bool bIsLooping);


	/**
	 * @brief 트랙 범위를 벗어난 시간 값을 유효한 값으로 조정합니다.
	 * 
	 * @param time 유효한 값으로 조정할 시간값입니다.
	 * @param bIsLooping 트랙의 반복 여부입니다.
	 */
	float AdjustTimeToFitTrack(float time, bool bIsLooping);


	/**
	 * @brief 키 프레임 내부 배열을 템플릿된 타입으로 캐스팅합니다.
	 * 
	 * @param value 템플릿 타입으로 캐스팅할 키 프레임 배열 값입니다.
	 * 
	 * @return 캐스팅된 값을 반환합니다.
	 */
	T Cast(float* value);


private:
	/**
	 * @brief 트랙 내의 키 프레임 목록입니다.
	 */
	std::vector<Keyframe<N>> keyframes_;


	/**
	 * @brief 키 프레임을 보간할 때의 유형입니다.
	 */
	EInterpolation interpolation_ = EInterpolation::LINEAR;
};


using ScalarTrack = Track<float, 1>;
using VectorTrack = Track<Vec3f, 3>;
using QuaternionTrack = Track<Quat, 4>;
#pragma once

#include <chrono>
#include <cstdint>
#include <limits>
#include <random>


namespace GameMaker
{
	/**
	 * @brief 파이(π) 값입니다.
	 */
	const float PI = 3.141592654f;


	/**
	 * @brief 2파이(2π) 값입니다.
	 */
	const float TWO_PI = 6.283185307f;


	/**
	 * @brief 파이(π) 의 역수 값입니다.
	 */
	const float ONE_DIV_PI = 0.318309886f;


	/**
	 * @brief 2파이(2π) 의 역수 값입니다.
	 */
	const float ONE_DIV_2PI = 0.159154943f;


	/**
	 * @brief 파이(π) 의 반값(π/2)입니다.
	 */
	const float PI_DIV_2 = 1.570796327f;


	/**
	 * @brief 파이(π) 의 반의 반 값(π/4)입니다.
	 */
	const float PI_DIV_4 = 0.785398163f;


	/**
	 * @brief 엡실론(ε) 값입니다.
	 */
	const float EPSILON = 1.192092896e-07F;


	/**
	 * @brief 라디안 각을 육십분법 각으로 변환합니다.
	 *
	 * @param radian 변환할 라디안 각입니다.
	 *
	 * @return 변환된 육십분법 각입니다.
	 */
	__forceinline float ToDegree(float radian)
	{
		return (radian * 180.0f) / PI;
	}


	/**
	 * @brief 육십분법 각을 라디안 각으로 변환합니다.
	 *
	 * @param degree 변환할 육십분법 각입니다.
	 *
	 * @return 변환된 라디안 각입니다.
	 */
	__forceinline float ToRadian(float degree)
	{
		return (degree * PI) / 180.0f;
	}


	/**
	 * @brief 부동 소수점 값이 0에 가까운지 확인합니다.
	 *
	 * @param value 0에 가까운지 확인할 값입니다.
	 * @param epsilon 값 확인을 위한 엡실론 값입니다.
	 *
	 * @return 부동 소수점 값이 0에 가깝다면 true, 그렇지 않다면 false를 반환합니다.
	 */
	__forceinline bool NearZero(float value, float epsilon = EPSILON)
	{
		return (std::fabsf(value) <= epsilon);
	}


	/**
	 * @brief 두 값중 큰 값을 반환합니다.
	 *
	 * @param lhs 크기를 비교할 값 중 왼쪽 값입니다.
	 * @param rhs 크기를 비교할 값 중 오른쪽 값입니다.
	 *
	 * @return 두 값 중 큰 값을 반환합니다.
	 */
	template <typename T>
	__forceinline T Max(const T& lhs, const T& rhs)
	{
		return (lhs < rhs ? rhs : lhs);
	}


	/**
	 * @brief 두 값중 작은 값을 반환합니다.
	 *
	 * @param lhs 크기를 비교할 값 중 왼쪽 값입니다.
	 * @param rhs 크기를 비교할 값 중 오른쪽 값입니다.
	 *
	 * @return 두 값 중 작은 값을 반환합니다.
	 */
	template <typename T>
	__forceinline T Min(const T& lhs, const T& rhs)
	{
		return (lhs < rhs ? lhs : rhs);
	}


	/**
	 * @brief 라디안 각도에 대응하는 사인 값을 얻습니다.
	 *
	 * @param radian 사인 값을 얻을 라디안 각도입니다.
	 *
	 * @return 라디안 각도에 대응하는 사인값을 반환합니다.
	 *
	 * @note
	 * - DirectXMath의 XMScalarSin 참조
	 * - 알고리즘은 11차원 미니맥스(Minimax) 근사치(1th-degree Minimax approximation) 사용
	 * - https://gist.github.com/publik-void/067f7f2fef32dbe5c27d6e215f824c91
	 */
	static __forceinline float Sin(float radian)
	{
		float quotient = ONE_DIV_2PI * radian;
		if (radian >= 0.0f)
		{
			quotient = static_cast<float>(static_cast<int>(quotient + 0.5f));
		}
		else
		{
			quotient = static_cast<float>(static_cast<int>(quotient - 0.5f));
		}

		float y = radian - TWO_PI * quotient;

		if (y > PI_DIV_2)
		{
			y = PI - y;
		}
		else if (y < -PI_DIV_2)
		{
			y = -PI - y;
		}

		float y2 = y * y;
		return (((((-2.3889859e-08f * y2 + 2.7525562e-06f) * y2 - 0.00019840874f) * y2 + 0.0083333310f) * y2 - 0.16666667f) * y2 + 1.0f) * y;
	}
}
#pragma once

#include <cstdint>


/**
 * @brief 파이(π) 값입니다.
 */
const float Pi = 3.141592654f;


/**
 * @brief 2파이(2π) 값입니다.
 */
const float TwoPi = 6.283185307f;


/**
 * @brief 파이(π) 의 역수 값입니다.
 */
const float OneDivPi = 0.318309886f;


/**
 * @brief 2파이(2π) 의 역수 값입니다.
 */
const float OneDivTwoPi = 0.159154943f;


/**
 * @brief 파이(π) 의 반값(π/2)입니다.
 */
const float PiDiv2 = 1.570796327f;


/**
 * @brief 파이(π) 의 반의 반 값(π/4)입니다.
 */
const float PiDiv4 = 0.785398163f;


/**
 * @brief 엡실론(ε) 값입니다.
 */
const float Epsilon = 1.192092896e-07F;


/**
 * 수학 관련 기능을 제공합니다.
 */
class MathModule
{
public:
	/**
	 * @brief 라디안 각을 육십분법 각으로 변환합니다.
	 *
	 * @param radian 변환할 라디안 각입니다.
	 *
	 * @return 변환된 육십분법 각입니다.
	 */
	static __forceinline float ToDegree(float radian)
	{
		return (radian * 180.0f) / Pi;
	}


	/**
	 * @brief 육십분법 각을 라디안 각으로 변환합니다.
	 *
	 * @param degree 변환할 육십분법 각입니다.
	 *
	 * @return 변환된 라디안 각입니다.
	 */
	static __forceinline float ToRadian(float degree)
	{
		return (degree * Pi) / 180.0f;
	}


	/**
	 * @brief 부동 소수점 값이 0에 가까운지 확인합니다.
	 *
	 * @param value 0에 가까운지 확인할 값입니다.
	 * @param epsilon 값 확인을 위한 엡실론 값입니다.
	 *
	 * @return 부동 소수점 값이 0에 가깝다면 true, 그렇지 않다면 false를 반환합니다.
	 */
	static bool NearZero(float value, float epsilon = Epsilon);


	/**
	 * @brief 두 값중 큰 값을 반환합니다.
	 *
	 * @param lhs 크기를 비교할 값 중 왼쪽 값입니다.
	 * @param rhs 크기를 비교할 값 중 오른쪽 값입니다.
	 *
	 * @return 두 값 중 큰 값을 반환합니다.
	 */
	template <typename T>
	static __forceinline T Max(const T& lhs, const T& rhs)
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
	static __forceinline T Min(const T& lhs, const T& rhs)
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
		float quotient = OneDivTwoPi * radian;
		if (radian >= 0.0f)
		{
			quotient = static_cast<float>(static_cast<int>(quotient + 0.5f));
		}
		else
		{
			quotient = static_cast<float>(static_cast<int>(quotient - 0.5f));
		}

		float y = radian - TwoPi * quotient;

		if (y > PiDiv2)
		{
			y = Pi - y;
		}
		else if (y < -PiDiv2)
		{
			y = -Pi - y;
		}

		float y2 = y * y;
		return (((((-2.3889859e-08f * y2 + 2.7525562e-06f) * y2 - 0.00019840874f) * y2 + 0.0083333310f) * y2 - 0.16666667f) * y2 + 1.0f) * y;
	}


	/**
	 * @brief 라디안 각도에 대응하는 코사인 값을 반환합니다.
	 *
	 * @param radian 코사인 값을 얻을 라디안 각도입니다.
	 *
	 * @return 라디안 각도에 대응하는 코사인값을 반환합니다.
	 *
	 * @note
	 * - DirectXMath의 XMScalarCos 참조
	 * - 알고리즘은 10차원 미니맥스(Minimax) 근사치(1th-degree Minimax approximation) 사용
	 * - https://gist.github.com/publik-void/067f7f2fef32dbe5c27d6e215f824c91
	 */
	static __forceinline float Cos(float radian)
	{
		float quotient = OneDivTwoPi * radian;
		if (radian >= 0.0f)
		{
			quotient = static_cast<float>(static_cast<int>(quotient + 0.5f));
		}
		else
		{
			quotient = static_cast<float>(static_cast<int>(quotient - 0.5f));
		}

		float y = radian - TwoPi * quotient;
		float sign = 1.0f;

		if (y > PiDiv2)
		{
			y = Pi - y;
			sign = -1.0f;
		}
		else if (y < -PiDiv2)
		{
			y = -Pi - y;
			sign = -1.0f;
		}

		float y2 = y * y;
		float p = ((((-2.6051615e-07f * y2 + 2.4760495e-05f) * y2 - 0.0013888378f) * y2 + 0.041666638f) * y2 - 0.5f) * y2 + 1.0f;

		return sign * p;
	}


	/**
	 * @brief 부동 소수점 숫자의 아크 사인을 계산합니다.
	 * 
	 * @param value -1.0 에서 1.0 사이의 값입니다.
	 * 
	 * @return 입력한 값의 아크 사인(역 사인) 값을 반환합니다.
	 * 
	 * @note
	 * - DirectXMath의 XMScalarASin 참조
	 * - 알고리즘은 7차원 미니맥스(Minimax) 근사치(1th-degree Minimax approximation) 사용
	 * - https://gist.github.com/publik-void/067f7f2fef32dbe5c27d6e215f824c91
	 */
	static float ASin(float value);


	/**
	 * @brief 부동 소수점 숫자의 아크 코사인을 계산합니다.
	 *
	 * @param value -1.0 에서 1.0 사이의 값입니다.
	 *
	 * @return 입력한 값의 코아크 사인(역 사인) 값을 반환합니다.
	 *
	 * @note
	 * - DirectXMath의 XMScalarACos 참조
	 * - 알고리즘은 7차원 미니맥스(Minimax) 근사치(1th-degree Minimax approximation) 사용
	 * - https://gist.github.com/publik-void/067f7f2fef32dbe5c27d6e215f824c91
	 */
	static float ACos(float value);


	/**
	 * @brief 제곱근을 계산합니다.
	 *
	 * @param x 제곱근을 계산할 부동소수점수입니다.
	 *
	 * @return 제곱근 값을 반환합니다.
	 */
	static float Sqrt(float x);


	/**
	 * @brief 절댓값을 계산합니다.
	 *
	 * @param x 절댓값을 계산할 부동소수점수입니다.
	 *
	 * @return 절대값을 반환합니다.
	 */
	static float Abs(float x);


	/**
	 * @brief 부동 소수점 나머지를 계산합니다.
	 * 
	 * @param x 부동 소수점 나머지를 계산할 왼쪽 피연산자입니다.
	 * @param y 부동 소수점 나머지를 계산할 오른쪽 피연산자입니다.
	 * 
	 * @return 계산된 부동 소수점 나머지를 반환합니다.
	 */
	static float Fmod(float x, float y);


	/**
	 * @brief 값을 [lower, upper] 범위로 자릅니다.
	 *
	 * @param value 범위로 자를 값입니다.
	 * @param lower 범위의 최소값입니다.
	 * @param upper 범위의 최대값입니다.
	 *
	 * @return 범위로 잘려 나간 값을 반환합니다.
	 */
	template <typename T>
	static __forceinline T Clamp(const T& value, const T& lower, const T& upper)
	{
		return Min<T>(upper, Max<T>(lower, value));
	}
	

	/**
	 * @brief 임의의 정수를 생성합니다.
	 *
	 * @param minValue 생성할 난수 범위의 최솟값입니다.
	 * @param maxValue 생성할 난수 범위의 최댓값입니다.
	 *
	 * @return 생성된 임의의 정수를 반환합니다.
	 */
	static int32_t GenerateRandomInt(int32_t minValue, int32_t maxValue);


	/**
	 * @brief 임의의 실수를 생성합니다.
	 *
	 * @param minValue 생성할 난수 범위의 최솟값입니다.
	 * @param maxValue 생성할 난수 범위의 최댓값입니다.
	 *
	 * @return 생성된 임의의 실수를 반환합니다.
	 */
	static float GenerateRandomFloat(float minValue, float maxValue);
};
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
	__forceinline float Sin(float radian)
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
	__forceinline float Cos(float radian)
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
		float sign = 1.0f;

		if (y > PI_DIV_2)
		{
			y = PI - y;
			sign = -1.0f;
		}
		else if (y < -PI_DIV_2)
		{
			y = -PI - y;
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
	__forceinline float ASin(float value)
	{
		float x = std::fabsf(value);
		float omx = 1.0f - x;
		if (omx < 0.0f)
		{
			omx = 0.0f;
		}

		float root = sqrtf(omx);
		float result = ((((((-0.0012624911f * x + 0.0066700901f) * x - 0.0170881256f) * x + 0.0308918810f) * x - 0.0501743046f) * x + 0.0889789874f) * x - 0.2145988016f) * x + 1.5707963050f;
		result *= root;

		if (value >= 0.0f)
		{
			return (PI_DIV_2 - result);
		}
		else
		{
			return (result - PI_DIV_2);
		}
	}


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
	__forceinline float ACos(float value)
	{
		float x = std::fabsf(value);
		float omx = 1.0f - x;
		if (omx < 0.0f)
		{
			omx = 0.0f;
		}

		float root = sqrtf(omx);
		float result = ((((((-0.0012624911f * x + 0.0066700901f) * x - 0.0170881256f) * x + 0.0308918810f) * x - 0.0501743046f) * x + 0.0889789874f) * x - 0.2145988016f) * x + 1.5707963050f;
		result *= root;

		if (value >= 0.0f)
		{
			return result;
		}
		else
		{
			return PI - result;
		}
	}


	/**
	 * @brief 제곱근을 계산합니다.
	 *
	 * @param x 제곱근을 계산할 부동소수점수입니다.
	 *
	 * @return 제곱근 값을 반환합니다.
	 */
	__forceinline float Sqrt(float x)
	{
		return std::sqrtf(x);
	}


	/**
	 * @brief 절댓값을 계산합니다.
	 *
	 * @param x 절댓값을 계산할 부동소수점수입니다.
	 *
	 * @return 절대값을 반환합니다.
	 */
	__forceinline float Abs(float x)
	{
		return std::fabsf(x);
	}


	/**
	 * @brief 부동 소수점 나머지를 계산합니다.
	 *
	 * @param x 부동 소수점 나머지를 계산할 왼쪽 피연산자입니다.
	 * @param y 부동 소수점 나머지를 계산할 오른쪽 피연산자입니다.
	 *
	 * @return 계산된 부동 소수점 나머지를 반환합니다.
	 */
	__forceinline float Fmod(float x, float y)
	{
		return std::fmodf(x, y);
	}


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
	__forceinline T Clamp(const T& value, const T& lower, const T& upper)
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
	__forceinline int32_t GenerateRandomInt(int32_t minValue, int32_t maxValue)
	{
		std::random_device randomDevice;
		std::mt19937 generator(randomDevice());
		std::uniform_int_distribution<int32_t> distribution(Min<int32_t>(minValue, maxValue), Max<int32_t>(minValue, maxValue));

		return distribution(generator);
	}


	/**
	 * @brief 임의의 실수를 생성합니다.
	 *
	 * @param minValue 생성할 난수 범위의 최솟값입니다.
	 * @param maxValue 생성할 난수 범위의 최댓값입니다.
	 *
	 * @return 생성된 임의의 실수를 반환합니다.
	 */
	__forceinline float GenerateRandomFloat(float minValue, float maxValue)
	{
		std::random_device randomDevice;
		std::mt19937 generator(randomDevice());
		std::uniform_real_distribution<float> distribution(Min<float>(minValue, maxValue), Max<float>(minValue, maxValue));

		return distribution(generator);
	}


/**
 * @brief 원소의 데이터 타입이 정수인 2차원 벡터입니다.
 */
struct Vec2i
{
	/**
	 * @brief 2차원 벡터의 기본 생성자입니다.
	 *
	 * @note 모든 원소의 값을 0으로 초기화합니다.
	 */
	Vec2i() noexcept : x(0), y(0) {}


	/**
	 * @brief 2차원 벡터의 생성자입니다.
	 *
	 * @param xx 벡터의 x 성분입니다.
	 * @param yy 벡터의 y 성분입니다.
	 */
	Vec2i(int32_t&& xx, int32_t&& yy) noexcept : x(xx), y(yy) {}


	/**
	 * @brief 2차원 벡터의 생성자입니다.
	 *
	 * @param xx 벡터의 x 성분입니다.
	 * @param yy 벡터의 y 성분입니다.
	 */
	Vec2i(const int32_t& xx, const int32_t& yy) noexcept : x(xx), y(yy) {}


	/**
	 * @brief 2차원 벡터의 원소를 하나의 값으로 초기화합니다.
	 *
	 * @param e 모든 원소를 초기화 할 값입니다.
	 */
	Vec2i(int32_t&& e) noexcept : x(e), y(e) {}


	/**
	 * @brief 2차원 벡터의 원소를 하나의 값으로 초기화합니다.
	 *
	 * @param e 모든 원소를 초기화 할 값입니다.
	 */
	Vec2i(const int32_t& e) noexcept : x(e), y(e) {}


	/**
	 * @brief 2차원 벡터의 복사 생성자입니다.
	 *
	 * @param v 원소를 복사할 벡터 구조체의 인스턴스입니다.
	 */
	Vec2i(Vec2i&& v) noexcept : x(v.x), y(v.y) {}


	/**
	 * @brief 2차원 벡터의 복사 생성자입니다.
	 *
	 * @param v 원소를 복사할 벡터의 인스턴스입니다.
	 */
	Vec2i(const Vec2i& v) noexcept : x(v.x), y(v.y) {}


	/**
	 * @brief 2차원 벡터의 대입 연산자 입니다.
	 *
	 * @param v 원소를 복사할 벡터 구조체의 인스턴스입니다.
	 *
	 * @return 대입한 벡터의 참조자를 반환합니다.
	 */
	Vec2i& operator=(Vec2i&& v) noexcept
	{
		if (this == &v) return *this;

		x = v.x;
		y = v.y;

		return *this;
	}


	/**
	 * @brief 2차원 벡터의 대입 연산자 입니다.
	 *
	 * @param v 원소를 복사할 벡터 구조체의 인스턴스입니다.
	 *
	 * @return 대입한 벡터의 참조자를 반환합니다.
	 */
	Vec2i& operator=(const Vec2i& v) noexcept
	{
		if (this == &v) return *this;

		x = v.x;
		y = v.y;

		return *this;
	}


	/**
	 * @brief 2차원 벡터의 원소에 -부호를 취합니다.
	 *
	 * @return 2차원 벡터의 원소에 -부호를 취한 새로운 벡터를 반환합니다.
	 */
	Vec2i operator-() const
	{
		return Vec2i(-x, -y);
	}


	/**
	 * @brief 두 2차원 벡터에 대응하는 원소를 더합니다.
	 *
	 * @param v 벡터의 덧셈을 수행할 피연산자입니다.
	 *
	 * @return 두 벡터에 대응하는 원소를 더한 결과를 반환합니다.
	 */
	Vec2i operator+(Vec2i&& v) const
	{
		return Vec2i(x + v.x, y + v.y);
	}


	/**
	 * @brief 두 2차원 벡터에 대응하는 원소를 더합니다.
	 *
	 * @param v 벡터의 덧셈을 수행할 피연산자입니다.
	 *
	 * @return 두 벡터에 대응하는 원소를 더한 결과를 반환합니다.
	 */
	Vec2i operator+(const Vec2i& v) const
	{
		return Vec2i(x + v.x, y + v.y);
	}


	/**
	 * @brief 두 2차원 벡터에 대응하는 원소를 뺍니다.
	 *
	 * @param v 벡터의 뺄셈을 수행할 피연산자입니다.
	 *
	 * @return 두 벡터에 대응하는 원소를 뺀 결과를 반환합니다.
	 */
	Vec2i operator-(Vec2i&& v) const
	{
		return Vec2i(x - v.x, y - v.y);
	}


	/**
	 * @brief 두 2차원 벡터에 대응하는 원소를 뺍니다.
	 *
	 * @param v 벡터의 뺄셈을 수행할 피연산자입니다.
	 *
	 * @return 두 벡터에 대응하는 원소를 뺀 결과를 반환합니다.
	 */
	Vec2i operator-(const Vec2i& v) const
	{
		return Vec2i(x - v.x, y - v.y);
	}


	/**
	 * @brief 두 2차원 벡터에 대응하는 원소를 각각 곱합니다.
	 *
	 * @note 이 연산은 벡터의 내적 연산(Dot Product)와는 다릅니다.
	 *
	 * @param v 벡터의 곱셈을 수행할 피연산자입니다.
	 *
	 * @return 두 벡터에 대응하는 원소를 곱한 결과를 반환합니다.
	 */
	Vec2i operator*(Vec2i&& v) const
	{
		return Vec2i(x * v.x, y * v.y);
	}


	/**
	 * @brief 두 2차원 벡터에 대응하는 원소를 각각 곱합니다.
	 *
	 * @note 이 연산은 벡터의 내적 연산(Dot Product)와는 다릅니다.
	 *
	 * @param v 벡터의 곱셈을 수행할 피연산자입니다.
	 *
	 * @return 두 벡터에 대응하는 원소를 곱한 결과를 반환합니다.
	 */
	Vec2i operator*(const Vec2i& v) const
	{
		return Vec2i(x * v.x, y * v.y);
	}


	/**
	 * @brief 두 2차원 벡터에 대응하는 원소를 더합니다.
	 *
	 * @param v 연산을 수행할 피연산자입니다.
	 *
	 * @return 연산을 수행한 벡터의 참조자를 반환합니다.
	 */
	Vec2i& operator+=(Vec2i&& v) noexcept
	{
		x += v.x;
		y += v.y;

		return *this;
	}


	/**
	 * @brief 두 2차원 벡터에 대응하는 원소를 더합니다.
	 *
	 * @param v 연산을 수행할 피연산자입니다.
	 *
	 * @return 연산을 수행한 벡터의 참조자를 반환합니다.
	 */
	Vec2i& operator+=(const Vec2i& v) noexcept
	{
		x += v.x;
		y += v.y;

		return *this;
	}


	/**
	 * @brief 두 2차원 벡터에 대응하는 원소를 뺍니다.
	 *
	 * @param v 연산을 수행할 피연산자입니다.
	 *
	 * @return 연산을 수행한 벡터의 참조자를 반환합니다.
	 */
	Vec2i& operator-=(Vec2i&& v) noexcept
	{
		x -= v.x;
		y -= v.y;

		return *this;
	}


	/**
	 * @brief 두 2차원 벡터에 대응하는 원소를 뺍니다.
	 *
	 * @param v 연산을 수행할 피연산자입니다.
	 *
	 * @return 연산을 수행한 벡터의 참조자를 반환합니다.
	 */
	Vec2i& operator-=(const Vec2i& v) noexcept
	{
		x -= v.x;
		y -= v.y;

		return *this;
	}


	/**
	 * @brief 두 2차원 벡터의 대응하는 원소가 동일한지 확인합니다.
	 *
	 * @param v 연산을 수행할 피연산자입니다.
	 *
	 * @return 두 2차원 벡터의 대응하는 원소가 일치한다면 true, 그렇지 않으면 false를 반환합니다.
	 */
	bool operator==(Vec2i&& v) noexcept
	{
		return (x == v.x) && (y == v.y);
	}


	/**
	 * @brief 두 2차원 벡터의 대응하는 원소가 동일한지 확인합니다.
	 *
	 * @param v 연산을 수행할 피연산자입니다.
	 *
	 * @return 두 2차원 벡터의 대응하는 원소가 일치한다면 true, 그렇지 않으면 false를 반환합니다.
	 */
	bool operator==(const Vec2i& v) noexcept
	{
		return (x == v.x) && (y == v.y);
	}


	/**
	 * @brief 두 2차원 벡터의 대응하는 원소가 동일하지 않은지 확인합니다.
	 *
	 * @param v 연산을 수행할 피연산자입니다.
	 *
	 * @return 두 2차원 벡터의 대응하는 원소가 일치하지 않는다면 true, 그렇다면 false를 반환합니다.
	 */
	bool operator!=(Vec2i&& v) noexcept
	{
		return (x != v.x) || (y != v.y);
	}


	/**
	 * @brief 두 2차원 벡터의 대응하는 원소가 동일하지 않은지 확인합니다.
	 *
	 * @param v 연산을 수행할 피연산자입니다.
	 *
	 * @return 두 2차원 벡터의 대응하는 원소가 일치하지 않는다면 true, 그렇다면 false를 반환합니다.
	 */
	bool operator!=(const Vec2i& v) noexcept
	{
		return (x != v.x) || (y != v.y);
	}


	/**
	 * @brief 2차원 벡터 원소 배열의 포인터를 얻습니다.
	 *
	 * @return 2차원 벡터 원소 배열의 포인터를 반환합니다.
	 */
	const int32_t* GetPtr() const { return &data[0]; }


	/**
	 * @brief 2차원 벡터 원소 배열의 포인터를 얻습니다.
	 *
	 * @return 2차원 벡터 원소 배열의 포인터를 반환합니다.
	 */
	int32_t* GetPtr() { return &data[0]; }


	/**
	 * @brief 2차원 백터의 내적 연산을 수행합니다.
	 *
	 * @param lhs 백터의 내적 연산을 수행할 좌측 피연산자입니다.
	 * @param rhs 백터의 내적 연산을 수행할 우측 피연산자입니다.
	 *
	 * @return 내적 연산 결과를 반환합니다.
	 */
	static inline int32_t Dot(const Vec2i& lhs, const Vec2i& rhs)
	{
		return lhs.x * rhs.x + lhs.y * rhs.y;
	}


	/**
	 * @brief 2차원 백터의 외적 연산을 수행합니다.
	 *
	 * @param lhs 백터의 외적 연산을 수행할 좌측 피연산자입니다.
	 * @param rhs 백터의 외적 연산을 수행할 우측 피연산자입니다.
	 *
	 * @return 외적 연산 결과를 반환합니다.
	 *
	 * @note 일반적인 3차원 외적 연산과 다릅니다.
	 */
	static inline int32_t Cross(const Vec2i& lhs, const Vec2i& rhs)
	{
		return lhs.x * rhs.y - lhs.y * rhs.x;
	}


	/**
	 * @brief 2차원 백터의 크기 제곱 값을 계산합니다.
	 *
	 * @param v 크기 제곱을 계산할 벡터입니다.
	 *
	 * @return 계산된 크기 제곱 값을 반환합니다.
	 */
	static inline int32_t LengthSq(const Vec2i& v)
	{
		return v.x * v.x + v.y * v.y;
	}


	/**
	 * @brief 2차원 벡터의 크기를 얻습니다.
	 *
	 * @param v 크기를 계산할 벡터입니다.
	 *
	 * @return 계산된 크기 값을 반환합니다.
	 */
	static inline float Length(const Vec2i& v)
	{
		float lengthSq = static_cast<float>(LengthSq(v));
		return Sqrt(lengthSq);
	}


	/**
	 * @brief 두 2차원 벡터 사이의 각을 계산합니다.
	 *
	 * @param lhs 벡터 사이의 각을 계산할 왼쪽 피연산자 벡터입니다.
	 * @param rhs 벡터 사이의 각을 계산할 오른쪽 피연산자 벡터입니다.
	 *
	 * @return 두 2차원 벡터 사이의 라디안 각도를 반환합니다.
	 */
	static inline float Radian(const Vec2i& lhs, const Vec2i& rhs)
	{
		float lengthL = Length(lhs);
		float lengthR = Length(rhs);
		float dot = Dot(lhs, rhs);

		return ACos(dot / (lengthL * lengthR));
	}


	/**
	 * @brief 두 2차원 벡터 사이의 각을 계산합니다.
	 *
	 * @param lhs 벡터 사이의 각을 계산할 왼쪽 피연산자 벡터입니다.
	 * @param rhs 벡터 사이의 각을 계산할 오른쪽 피연산자 벡터입니다.
	 *
	 * @return 두 2차원 벡터 사이의 육십분법 각도를 반환합니다.
	 */
	static inline float Degree(const Vec2i& lhs, const Vec2i& rhs)
	{
		float radian = Radian(lhs, rhs);
		return ToDegree(radian);
	}


	/**
	 * @brief 2차원 벡터의 다양한 원소 형식입니다.
	 */
	union
	{
		struct
		{
			int32_t x;
			int32_t y;
		};
		int32_t data[2];
	};
};


/**
 * @brief 원소의 데이터 타입이 부동 소수점인 2차원 벡터입니다.
 */
struct Vec2f
{
	/**
	 * @brief 2차원 벡터의 기본 생성자입니다.
	 *
	 * @note 모든 원소의 값을 0으로 초기화합니다.
	 */
	Vec2f() noexcept : x(0.0f), y(0.0f) {}


	/**
	 * @brief 2차원 벡터의 생성자입니다.
	 *
	 * @param xx 벡터의 x 성분입니다.
	 * @param yy 벡터의 y 성분입니다.
	 */
	Vec2f(float&& xx, float&& yy) noexcept : x(xx), y(yy) {}


	/**
	 * @brief 2차원 벡터의 생성자입니다.
	 *
	 * @param xx 벡터의 x 성분입니다.
	 * @param yy 벡터의 y 성분입니다.
	 */
	Vec2f(const float& xx, const float& yy) noexcept : x(xx), y(yy) {}


	/**
	 * @brief 2차원 벡터의 원소를 하나의 값으로 초기화합니다.
	 *
	 * @param e 모든 원소를 초기화 할 값입니다.
	 */
	Vec2f(float&& e) noexcept : x(e), y(e) {}


	/**
	 * @brief 2차원 벡터의 원소를 하나의 값으로 초기화합니다.
	 *
	 * @param e 모든 원소를 초기화 할 값입니다.
	 */
	Vec2f(const float& e) noexcept : x(e), y(e) {}


	/**
	 * @brief 2차원 벡터의 복사 생성자입니다.
	 *
	 * @param v 원소를 복사할 벡터 구조체의 인스턴스입니다.
	 */
	Vec2f(Vec2f&& v) noexcept : x(v.x), y(v.y) {}


	/**
	 * @brief 2차원 벡터의 복사 생성자입니다.
	 *
	 * @param v 원소를 복사할 벡터의 인스턴스입니다.
	 */
	Vec2f(const Vec2f& v) noexcept : x(v.x), y(v.y) {}


	/**
	 * @brief 2차원 벡터의 대입 연산자 입니다.
	 *
	 * @param v 원소를 복사할 벡터 구조체의 인스턴스입니다.
	 *
	 * @return 대입한 벡터의 참조자를 반환합니다.
	 */
	Vec2f& operator=(Vec2f&& v) noexcept
	{
		if (this == &v) return *this;

		x = v.x;
		y = v.y;

		return *this;
	}


	/**
	 * @brief 2차원 벡터의 대입 연산자 입니다.
	 *
	 * @param v 원소를 복사할 벡터 구조체의 인스턴스입니다.
	 *
	 * @return 대입한 벡터의 참조자를 반환합니다.
	 */
	Vec2f& operator=(const Vec2f& v) noexcept
	{
		if (this == &v) return *this;

		x = v.x;
		y = v.y;

		return *this;
	}


	/**
	 * @brief 2차원 벡터의 원소에 -부호를 취합니다.
	 *
	 * @return 2차원 벡터의 원소에 -부호를 취한 새로운 벡터를 반환합니다.
	 */
	Vec2f operator-() const
	{
		return Vec2f(-x, -y);
	}


	/**
	 * @brief 두 2차원 벡터에 대응하는 원소를 더합니다.
	 *
	 * @param v 벡터의 덧셈을 수행할 피연산자입니다.
	 *
	 * @return 두 벡터에 대응하는 원소를 더한 결과를 반환합니다.
	 */
	Vec2f operator+(Vec2f&& v) const
	{
		return Vec2f(x + v.x, y + v.y);
	}


	/**
	 * @brief 두 2차원 벡터에 대응하는 원소를 더합니다.
	 *
	 * @param v 벡터의 덧셈을 수행할 피연산자입니다.
	 *
	 * @return 두 벡터에 대응하는 원소를 더한 결과를 반환합니다.
	 */
	Vec2f operator+(const Vec2f& v) const
	{
		return Vec2f(x + v.x, y + v.y);
	}


	/**
	 * @brief 두 2차원 벡터에 대응하는 원소를 뺍니다.
	 *
	 * @param v 벡터의 뺄셈을 수행할 피연산자입니다.
	 *
	 * @return 두 벡터에 대응하는 원소를 뺀 결과를 반환합니다.
	 */
	Vec2f operator-(Vec2f&& v) const
	{
		return Vec2f(x - v.x, y - v.y);
	}


	/**
	 * @brief 두 2차원 벡터에 대응하는 원소를 뺍니다.
	 *
	 * @param v 벡터의 뺄셈을 수행할 피연산자입니다.
	 *
	 * @return 두 벡터에 대응하는 원소를 뺀 결과를 반환합니다.
	 */
	Vec2f operator-(const Vec2f& v) const
	{
		return Vec2f(x - v.x, y - v.y);
	}


	/**
	 * @brief 두 2차원 벡터에 대응하는 원소를 각각 곱합니다.
	 *
	 * @note 이 연산은 벡터의 내적 연산(Dot Product)와는 다릅니다.
	 *
	 * @param v 벡터의 곱셈을 수행할 피연산자입니다.
	 *
	 * @return 두 벡터에 대응하는 원소를 곱한 결과를 반환합니다.
	 */
	Vec2f operator*(Vec2f&& v) const
	{
		return Vec2f(x * v.x, y * v.y);
	}


	/**
	 * @brief 두 2차원 벡터에 대응하는 원소를 각각 곱합니다.
	 *
	 * @note 이 연산은 벡터의 내적 연산(Dot Product)와는 다릅니다.
	 *
	 * @param v 벡터의 곱셈을 수행할 피연산자입니다.
	 *
	 * @return 두 벡터에 대응하는 원소를 곱한 결과를 반환합니다.
	 */
	Vec2f operator*(const Vec2f& v) const
	{
		return Vec2f(x * v.x, y * v.y);
	}


	/**
	 * @brief 두 2차원 벡터에 대응하는 원소를 더합니다.
	 *
	 * @param v 연산을 수행할 피연산자입니다.
	 *
	 * @return 연산을 수행한 벡터의 참조자를 반환합니다.
	 */
	Vec2f& operator+=(Vec2f&& v) noexcept
	{
		x += v.x;
		y += v.y;

		return *this;
	}


	/**
	 * @brief 두 2차원 벡터에 대응하는 원소를 더합니다.
	 *
	 * @param v 연산을 수행할 피연산자입니다.
	 *
	 * @return 연산을 수행한 벡터의 참조자를 반환합니다.
	 */
	Vec2f& operator+=(const Vec2f& v) noexcept
	{
		x += v.x;
		y += v.y;

		return *this;
	}


	/**
	 * @brief 두 2차원 벡터에 대응하는 원소를 뺍니다.
	 *
	 * @param v 연산을 수행할 피연산자입니다.
	 *
	 * @return 연산을 수행한 벡터의 참조자를 반환합니다.
	 */
	Vec2f& operator-=(Vec2f&& v) noexcept
	{
		x -= v.x;
		y -= v.y;

		return *this;
	}


	/**
	 * @brief 두 2차원 벡터에 대응하는 원소를 뺍니다.
	 *
	 * @param v 연산을 수행할 피연산자입니다.
	 *
	 * @return 연산을 수행한 벡터의 참조자를 반환합니다.
	 */
	Vec2f& operator-=(const Vec2f& v) noexcept
	{
		x -= v.x;
		y -= v.y;

		return *this;
	}


	/**
	 * @brief 두 2차원 벡터의 대응하는 원소가 동일한지 확인합니다.
	 *
	 * @param v 연산을 수행할 피연산자입니다.
	 *
	 * @return 두 2차원 벡터의 대응하는 원소가 일치한다면 true, 그렇지 않으면 false를 반환합니다.
	 */
	bool operator==(Vec2f&& v) noexcept
	{
		return Abs(x - v.x) <= EPSILON && Abs(y - v.y) <= EPSILON;
	}


	/**
	 * @brief 두 2차원 벡터의 대응하는 원소가 동일한지 확인합니다.
	 *
	 * @param v 연산을 수행할 피연산자입니다.
	 *
	 * @return 두 2차원 벡터의 대응하는 원소가 일치한다면 true, 그렇지 않으면 false를 반환합니다.
	 */
	bool operator==(const Vec2f& v) noexcept
	{
		return Abs(x - v.x) <= EPSILON && Abs(y - v.y) <= EPSILON;
	}


	/**
	 * @brief 두 2차원 벡터의 대응하는 원소가 동일하지 않은지 확인합니다.
	 *
	 * @param v 연산을 수행할 피연산자입니다.
	 *
	 * @return 두 2차원 벡터의 대응하는 원소가 일치하지 않는다면 true, 그렇다면 false를 반환합니다.
	 */
	bool operator!=(Vec2f&& v) noexcept
	{
		return Abs(x - v.x) > EPSILON || Abs(y - v.y) > EPSILON;
	}


	/**
	 * @brief 두 2차원 벡터의 대응하는 원소가 동일하지 않은지 확인합니다.
	 *
	 * @param v 연산을 수행할 피연산자입니다.
	 *
	 * @return 두 2차원 벡터의 대응하는 원소가 일치하지 않는다면 true, 그렇다면 false를 반환합니다.
	 */
	bool operator!=(const Vec2f& v) noexcept
	{
		return Abs(x - v.x) > EPSILON || Abs(y - v.y) > EPSILON;
	}


	/**
	 * @brief 2차원 벡터 원소 배열의 포인터를 얻습니다.
	 *
	 * @return 2차원 벡터 원소 배열의 포인터를 반환합니다.
	 */
	const float* GetPtr() const { return &data[0]; }


	/**
	 * @brief 2차원 벡터 원소 배열의 포인터를 얻습니다.
	 *
	 * @return 2차원 벡터 원소 배열의 포인터를 반환합니다.
	 */
	float* GetPtr() { return &data[0]; }


	/**
	 * @brief 2차원 백터의 내적 연산을 수행합니다.
	 *
	 * @param lhs 백터의 내적 연산을 수행할 좌측 피연산자입니다.
	 * @param rhs 백터의 내적 연산을 수행할 우측 피연산자입니다.
	 *
	 * @return 내적 연산 결과를 반환합니다.
	 */
	static inline float Dot(const Vec2f& lhs, const Vec2f& rhs)
	{
		return lhs.x * rhs.x + lhs.y * rhs.y;
	}


	/**
	 * @brief 2차원 백터의 외적 연산을 수행합니다.
	 *
	 * @param lhs 백터의 외적 연산을 수행할 좌측 피연산자입니다.
	 * @param rhs 백터의 외적 연산을 수행할 우측 피연산자입니다.
	 *
	 * @return 외적 연산 결과를 반환합니다.
	 *
	 * @note 일반적인 3차원 외적 연산과 다릅니다.
	 */
	static inline float Cross(const Vec2f& lhs, const Vec2f& rhs)
	{
		return lhs.x * rhs.y - lhs.y * rhs.x;
	}


	/**
	 * @brief 2차원 백터의 크기 제곱 값을 계산합니다.
	 *
	 * @param v 크기 제곱을 계산할 벡터입니다.
	 *
	 * @return 계산된 크기 제곱 값을 반환합니다.
	 */
	static inline float LengthSq(const Vec2f& v)
	{
		return v.x * v.x + v.y * v.y;
	}


	/**
	 * @brief 2차원 벡터의 크기를 얻습니다.
	 *
	 * @param v 크기를 계산할 벡터입니다.
	 *
	 * @return 계산된 크기 값을 반환합니다.
	 */
	static inline float Length(const Vec2f& v)
	{
		float lengthSq = LengthSq(v);
		return Sqrt(lengthSq);
	}


	/**
	 * @brief 2차원 벡터를 정규화합니다.
	 *
	 * @param v 정규화 할 벡터입니다.
	 *
	 * @return 정규화된 벡터를 반환합니다.
	 */
	static inline Vec2f Normalize(const Vec2f& v)
	{
		float length = Length(v);

		if (NearZero(length))
		{
			return v;
		}

		float invLength = 1.0f / length;
		return Vec2f(v.x * invLength, v.y * invLength);
	}


	/**
	 * @brief 두 2차원 벡터 사이의 각을 계산합니다.
	 *
	 * @param lhs 벡터 사이의 각을 계산할 왼쪽 피연산자 벡터입니다.
	 * @param rhs 벡터 사이의 각을 계산할 오른쪽 피연산자 벡터입니다.
	 *
	 * @return 두 2차원 벡터 사이의 라디안 각도를 반환합니다.
	 */
	static inline float Radian(const Vec2f& lhs, const Vec2f& rhs)
	{
		float lengthL = Length(lhs);
		float lengthR = Length(rhs);
		float dot = Dot(lhs, rhs);

		return ACos(dot / (lengthL * lengthR));
	}


	/**
	 * @brief 두 2차원 벡터 사이의 각을 계산합니다.
	 *
	 * @param lhs 벡터 사이의 각을 계산할 왼쪽 피연산자 벡터입니다.
	 * @param rhs 벡터 사이의 각을 계산할 오른쪽 피연산자 벡터입니다.
	 *
	 * @return 두 2차원 벡터 사이의 육십분법 각도를 반환합니다.
	 */
	static inline float Degree(const Vec2f& lhs, const Vec2f& rhs)
	{
		float radian = Radian(lhs, rhs);
		return ToDegree(radian);
	}


	/**
	 * @brief Base 2차원 벡터에 Target 2차원 벡터를 투영합니다.
	 *
	 * @param target 투영 대상에 투영할 벡터입니다.
	 * @param base 투영 대상이 되는 벡터입니다.
	 *
	 * @return base에 투영된 target 벡터를 반환합니다.
	 */
	static inline Vec2f Project(const Vec2f& target, const Vec2f& base)
	{
		float dot = Dot(target, base);
		float lengthSq = LengthSq(base);
		float scale = dot / lengthSq;

		return Vec2f(base.x * scale, base.y * scale);
	}


	/**
	 * @brief Base 2차원 벡터에 Target 2차원 벡터를 투영한 벡터에 수직인 벡터를 계산합니다.
	 *
	 * @param target 투영 대상에 투영할 벡터입니다.
	 * @param base 투영 대상이 되는 벡터입니다.
	 *
	 * @return 투영된 벡터의 수직인 벡터를 반환합니다.
	 */
	static inline Vec2f Reject(const Vec2f& target, const Vec2f& base)
	{
		Vec2f project = Project(target, base);
		return target - project;
	}


	/**
	 * @brief 두 벡터를 선형 보간한 벡터를 계산합니다.
	 *
	 * @param s 보간의 시작 벡터입니다.
	 * @param e 보간의 끝 벡터입니다.
	 * @param t 두 벡터의 보간 비율입니다.
	 *
	 * @return 보간된 벡터를 반환합니다.
	 */
	static inline Vec2f Lerp(const Vec2f& s, const Vec2f& e, const float& t)
	{
		return s * (1.0f - t) + e * t;
	}


	/**
	 * @brief 두 벡터를 구면 선형 보간한 벡터를 계산합니다.
	 *
	 * @param s 보간의 시작 벡터입니다.
	 * @param e 보간의 끝 벡터입니다.
	 * @param t 두 벡터의 보간 비율입니다.
	 *
	 * @return 보간된 벡터를 반환합니다.
	 */
	static inline Vec2f Slerp(const Vec2f& s, const Vec2f& e, const float& t)
	{
		Vec2f start = Normalize(s);
		Vec2f end = Normalize(e);

		float theta = Radian(start, end);
		float sinTheta = Sin(theta);

		float a = Sin((1.0f - t) * theta) / sinTheta;
		float b = Sin(t * theta) / sinTheta;

		return s * a + e * b;
	}


	/**
	 * @brief 2차원 벡터의 다양한 원소 형식입니다.
	 */
	union
	{
		struct
		{
			float x;
			float y;
		};
		float data[2];
	};
};


/**
 * @brief 원소의 데이터 타입이 정수인 3차원 벡터입니다.
 */
struct Vec3i
{
	/**
	 * @brief 3차원 벡터의 기본 생성자입니다.
	 *
	 * @note 모든 원소의 값을 0으로 초기화합니다.
	 */
	Vec3i() noexcept : x(0), y(0), z(0) {}


	/**
	 * @brief 3차원 벡터의 생성자입니다.
	 *
	 * @param xx 벡터의 x 성분입니다.
	 * @param yy 벡터의 y 성분입니다.
	 * @param zz 벡터의 z 성분입니다.
	 */
	Vec3i(int32_t&& xx, int32_t&& yy, int32_t&& zz) noexcept : x(xx), y(yy), z(zz) {}


	/**
	 * @brief 3차원 벡터의 생성자입니다.
	 *
	 * @param xx 벡터의 x 성분입니다.
	 * @param yy 벡터의 y 성분입니다.
	 * @param zz 벡터의 z 성분입니다.
	 */
	Vec3i(const int32_t& xx, const int32_t& yy, const int32_t& zz) noexcept : x(xx), y(yy), z(zz) {}


	/**
	 * @brief 3차원 벡터의 원소를 하나의 값으로 초기화합니다.
	 *
	 * @param e 모든 원소를 초기화 할 값입니다.
	 */
	Vec3i(int32_t&& e) noexcept : x(e), y(e), z(e) {}


	/**
	 * @brief 3차원 벡터의 원소를 하나의 값으로 초기화합니다.
	 *
	 * @param e 모든 원소를 초기화 할 값입니다.
	 */
	Vec3i(const int32_t& e) noexcept : x(e), y(e), z(e) {}


	/**
	 * @brief 3차원 벡터의 복사 생성자입니다.
	 *
	 * @param v 원소를 복사할 벡터 구조체의 인스턴스입니다.
	 */
	Vec3i(Vec3i&& v) noexcept : x(v.x), y(v.y), z(v.z) {}


	/**
	 * @brief 3차원 벡터의 복사 생성자입니다.
	 *
	 * @param v 원소를 복사할 벡터의 인스턴스입니다.
	 */
	Vec3i(const Vec3i& v) noexcept : x(v.x), y(v.y), z(v.z) {}


	/**
	 * @brief 3차원 벡터의 대입 연산자 입니다.
	 *
	 * @param v 원소를 복사할 벡터 구조체의 인스턴스입니다.
	 *
	 * @return 대입한 벡터의 참조자를 반환합니다.
	 */
	Vec3i& operator=(Vec3i&& v) noexcept
	{
		if (this == &v) return *this;

		x = v.x;
		y = v.y;
		z = v.z;

		return *this;
	}


	/**
	 * @brief 3차원 벡터의 대입 연산자 입니다.
	 *
	 * @param v 원소를 복사할 벡터 구조체의 인스턴스입니다.
	 *
	 * @return 대입한 벡터의 참조자를 반환합니다.
	 */
	Vec3i& operator=(const Vec3i& v) noexcept
	{
		if (this == &v) return *this;

		x = v.x;
		y = v.y;
		z = v.z;

		return *this;
	}


	/**
	 * @brief 3차원 벡터의 원소에 -부호를 취합니다.
	 *
	 * @return 3차원 벡터의 원소에 -부호를 취한 새로운 벡터를 반환합니다.
	 */
	Vec3i operator-() const
	{
		return Vec3i(-x, -y, -z);
	}


	/**
	 * @brief 두 3차원 벡터에 대응하는 원소를 더합니다.
	 *
	 * @param v 벡터의 덧셈을 수행할 피연산자입니다.
	 *
	 * @return 두 벡터에 대응하는 원소를 더한 결과를 반환합니다.
	 */
	Vec3i operator+(Vec3i&& v) const
	{
		return Vec3i(x + v.x, y + v.y, z + v.z);
	}


	/**
	 * @brief 두 3차원 벡터에 대응하는 원소를 더합니다.
	 *
	 * @param v 벡터의 덧셈을 수행할 피연산자입니다.
	 *
	 * @return 두 벡터에 대응하는 원소를 더한 결과를 반환합니다.
	 */
	Vec3i operator+(const Vec3i& v) const
	{
		return Vec3i(x + v.x, y + v.y, z + v.z);
	}


	/**
	 * @brief 두 3차원 벡터에 대응하는 원소를 뺍니다.
	 *
	 * @param v 벡터의 뺄셈을 수행할 피연산자입니다.
	 *
	 * @return 두 벡터에 대응하는 원소를 뺀 결과를 반환합니다.
	 */
	Vec3i operator-(Vec3i&& v) const
	{
		return Vec3i(x - v.x, y - v.y, z - v.z);
	}


	/**
	 * @brief 두 3차원 벡터에 대응하는 원소를 뺍니다.
	 *
	 * @param v 벡터의 뺄셈을 수행할 피연산자입니다.
	 *
	 * @return 두 벡터에 대응하는 원소를 뺀 결과를 반환합니다.
	 */
	Vec3i operator-(const Vec3i& v) const
	{
		return Vec3i(x - v.x, y - v.y, z - v.z);
	}


	/**
	 * @brief 두 3차원 벡터에 대응하는 원소를 각각 곱합니다.
	 *
	 * @note 이 연산은 벡터의 내적 연산(Dot Product)와는 다릅니다.
	 *
	 * @param v 벡터의 곱셈을 수행할 피연산자입니다.
	 *
	 * @return 두 벡터에 대응하는 원소를 곱한 결과를 반환합니다.
	 */
	Vec3i operator*(Vec3i&& v) const
	{
		return Vec3i(x * v.x, y * v.y, z * v.z);
	}


	/**
	 * @brief 두 3차원 벡터에 대응하는 원소를 각각 곱합니다.
	 *
	 * @note 이 연산은 벡터의 내적 연산(Dot Product)와는 다릅니다.
	 *
	 * @param v 벡터의 곱셈을 수행할 피연산자입니다.
	 *
	 * @return 두 벡터에 대응하는 원소를 곱한 결과를 반환합니다.
	 */
	Vec3i operator*(const Vec3i& v) const
	{
		return Vec3i(x * v.x, y * v.y, z * v.z);
	}


	/**
	 * @brief 두 3차원 벡터에 대응하는 원소를 더합니다.
	 *
	 * @param v 연산을 수행할 피연산자입니다.
	 *
	 * @return 연산을 수행한 벡터의 참조자를 반환합니다.
	 */
	Vec3i& operator+=(Vec3i&& v) noexcept
	{
		x += v.x;
		y += v.y;
		z += v.z;

		return *this;
	}


	/**
	 * @brief 두 3차원 벡터에 대응하는 원소를 더합니다.
	 *
	 * @param v 연산을 수행할 피연산자입니다.
	 *
	 * @return 연산을 수행한 벡터의 참조자를 반환합니다.
	 */
	Vec3i& operator+=(const Vec3i& v) noexcept
	{
		x += v.x;
		y += v.y;
		z += v.z;

		return *this;
	}


	/**
	 * @brief 두 3차원 벡터에 대응하는 원소를 뺍니다.
	 *
	 * @param v 연산을 수행할 피연산자입니다.
	 *
	 * @return 연산을 수행한 벡터의 참조자를 반환합니다.
	 */
	Vec3i& operator-=(Vec3i&& v) noexcept
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;

		return *this;
	}


	/**
	 * @brief 두 3차원 벡터에 대응하는 원소를 뺍니다.
	 *
	 * @param v 연산을 수행할 피연산자입니다.
	 *
	 * @return 연산을 수행한 벡터의 참조자를 반환합니다.
	 */
	Vec3i& operator-=(const Vec3i& v) noexcept
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;

		return *this;
	}


	/**
	 * @brief 두 3차원 벡터가 동일한지 검사합니다.
	 *
	 * @param v 검사를 수행할 피연산자입니다.
	 *
	 * @return 두 벡터가 동일하다면 true, 그렇지 않으면 false를 반환합니다.
	 */
	bool operator==(Vec3i&& v) const
	{
		return (x == v.x) && (y == v.y) && (z == v.z);
	}


	/**
	 * @brief 두 3차원 벡터가 동일한지 검사합니다.
	 *
	 * @param v 검사를 수행할 피연산자입니다.
	 *
	 * @return 두 벡터가 동일하다면 true, 그렇지 않으면 false를 반환합니다.
	 */
	bool operator==(const Vec3i& v) const
	{
		return (x == v.x) && (y == v.y) && (z == v.z);
	}


	/**
	 * @brief 두 3차원 벡터가 동일하지 않은지 검사합니다.
	 *
	 * @param v 검사를 수행할 피연산자입니다.
	 *
	 * @return 두 벡터가 동일하지 않다면 true, 그렇다면 false를 반환합니다.
	 */
	bool operator!=(Vec3i&& v) const
	{
		return (x != v.x) || (y != v.y) || (z != v.z);
	}


	/**
	 * @brief 두 3차원 벡터가 동일하지 않은지 검사합니다.
	 *
	 * @param v 검사를 수행할 피연산자입니다.
	 *
	 * @return 두 벡터가 동일하지 않다면 true, 그렇다면 false를 반환합니다.
	 */
	bool operator!=(const Vec3i& v) const
	{
		return (x != v.x) || (y != v.y) || (z != v.z);
	}


	/**
	 * @brief 3차원 벡터 원소 배열의 포인터를 얻습니다.
	 *
	 * @return 3차원 벡터 원소 배열의 포인터를 반환합니다.
	 */
	const int32_t* GetPtr() const { return &data[0]; }


	/**
	 * @brief 3차원 벡터 원소 배열의 포인터를 얻습니다.
	 *
	 * @return 3차원 벡터 원소 배열의 포인터를 반환합니다.
	 */
	int32_t* GetPtr() { return &data[0]; }


	/**
	 * @brief 3차원 백터의 내적 연산을 수행합니다.
	 *
	 * @param lhs 백터의 내적 연산을 수행할 좌측 피연산자입니다.
	 * @param rhs 백터의 내적 연산을 수행할 우측 피연산자입니다.
	 *
	 * @return 내적 연산 결과를 반환합니다.
	 */
	static inline int32_t Dot(const Vec3i& lhs, const Vec3i& rhs)
	{
		return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
	}


	/**
	 * @brief 3차원 벡터의 외적 연산을 수행합니다.
	 *
	 * @param lhs 백터의 외적 연산을 수행할 좌측 피연산자입니다.
	 * @param rhs 백터의 외적 연산을 수행할 우측 피연산자입니다.
	 *
	 * @return 외적 연산 결과를 반환합니다.
	 */
	static inline Vec3i Cross(const Vec3i& lhs, const Vec3i& rhs)
	{
		return Vec3i(
			lhs.y * rhs.z - lhs.z * rhs.y,
			lhs.z * rhs.x - lhs.x * rhs.z,
			lhs.x * rhs.y - lhs.y * rhs.x
		);
	}


	/**
	 * @brief 3차원 백터의 크기 제곱 값을 계산합니다.
	 *
	 * @param v 크기 제곱을 계산할 벡터입니다.
	 *
	 * @return 계산된 크기 제곱 값을 반환합니다.
	 */
	static inline int32_t LengthSq(const Vec3i& v)
	{
		return v.x * v.x + v.y * v.y + v.z * v.z;
	}


	/**
	 * @brief 3차원 벡터의 크기를 얻습니다.
	 *
	 * @param v 크기를 계산할 벡터입니다.
	 *
	 * @return 계산된 크기 값을 반환합니다.
	 */
	static inline float Length(const Vec3i& v)
	{
		float lengthSq = static_cast<float>(LengthSq(v));
		return Sqrt(lengthSq);
	}


	/**
	 * @brief 두 3차원 벡터 사이의 각을 계산합니다.
	 *
	 * @param lhs 벡터 사이의 각을 계산할 왼쪽 피연산자 벡터입니다.
	 * @param rhs 벡터 사이의 각을 계산할 오른쪽 피연산자 벡터입니다.
	 *
	 * @return 두 3차원 벡터 사이의 라디안 각도를 반환합니다.
	 */
	static inline float Radian(const Vec3i& lhs, const Vec3i& rhs)
	{
		float lengthL = Length(lhs);
		float lengthR = Length(rhs);
		float dot = Dot(lhs, rhs);

		return ACos(dot / (lengthL * lengthR));
	}


	/**
	 * @brief 두 3차원 벡터 사이의 각을 계산합니다.
	 *
	 * @param lhs 벡터 사이의 각을 계산할 왼쪽 피연산자 벡터입니다.
	 * @param rhs 벡터 사이의 각을 계산할 오른쪽 피연산자 벡터입니다.
	 *
	 * @return 두 3차원 벡터 사이의 육십분법 각도를 반환합니다.
	 */
	static inline float Degree(const Vec3i& lhs, const Vec3i& rhs)
	{
		float radian = Radian(lhs, rhs);
		return ToDegree(radian);
	}


	/**
	 * @brief 3차원 벡터의 다양한 원소 형식입니다.
	 */
	union
	{
		struct
		{
			int32_t x;
			int32_t y;
			int32_t z;
		};
		int32_t data[3];
	};
};


/**
 * @brief 원소의 데이터 타입이 부동 소수점인 3차원 벡터입니다.
 */
struct Vec3f
{
	/**
	 * @brief 3차원 벡터의 기본 생성자입니다.
	 *
	 * @note 모든 원소의 값을 0으로 초기화합니다.
	 */
	Vec3f() noexcept : x(0.0f), y(0.0f), z(0.0f) {}


	/**
	 * @brief 3차원 벡터의 생성자입니다.
	 *
	 * @param xx 벡터의 x 성분입니다.
	 * @param yy 벡터의 y 성분입니다.
	 * @param zz 벡터의 z 성분입니다.
	 */
	Vec3f(float&& xx, float&& yy, float&& zz) noexcept : x(xx), y(yy), z(zz) {}


	/**
	 * @brief 3차원 벡터의 생성자입니다.
	 *
	 * @param xx 벡터의 x 성분입니다.
	 * @param yy 벡터의 y 성분입니다.
	 * @param zz 벡터의 z 성분입니다.
	 */
	Vec3f(const float& xx, const float& yy, const float& zz) noexcept : x(xx), y(yy), z(zz) {}


	/**
	 * @brief 3차원 벡터의 원소를 하나의 값으로 초기화합니다.
	 *
	 * @param e 모든 원소를 초기화 할 값입니다.
	 */
	Vec3f(float&& e) noexcept : x(e), y(e), z(e) {}


	/**
	 * @brief 3차원 벡터의 원소를 하나의 값으로 초기화합니다.
	 *
	 * @param e 모든 원소를 초기화 할 값입니다.
	 */
	Vec3f(const float& e) noexcept : x(e), y(e), z(e) {}


	/**
	 * @brief 3차원 벡터의 복사 생성자입니다.
	 *
	 * @param v 원소를 복사할 벡터 구조체의 인스턴스입니다.
	 */
	Vec3f(Vec3f&& v) noexcept : x(v.x), y(v.y), z(v.z) {}


	/**
	 * @brief 3차원 벡터의 복사 생성자입니다.
	 *
	 * @param v 원소를 복사할 벡터의 인스턴스입니다.
	 */
	Vec3f(const Vec3f& v) noexcept : x(v.x), y(v.y), z(v.z) {}


	/**
	 * @brief 3차원 벡터의 대입 연산자 입니다.
	 *
	 * @param v 원소를 복사할 벡터 구조체의 인스턴스입니다.
	 *
	 * @return 대입한 벡터의 참조자를 반환합니다.
	 */
	Vec3f& operator=(Vec3f&& v) noexcept
	{
		if (this == &v) return *this;

		x = v.x;
		y = v.y;
		z = v.z;

		return *this;
	}


	/**
	 * @brief 3차원 벡터의 대입 연산자 입니다.
	 *
	 * @param v 원소를 복사할 벡터 구조체의 인스턴스입니다.
	 *
	 * @return 대입한 벡터의 참조자를 반환합니다.
	 */
	Vec3f& operator=(const Vec3f& v) noexcept
	{
		if (this == &v) return *this;

		x = v.x;
		y = v.y;
		z = v.z;

		return *this;
	}


	/**
	 * @brief 3차원 벡터의 원소에 -부호를 취합니다.
	 *
	 * @return 3차원 벡터의 원소에 -부호를 취한 새로운 벡터를 반환합니다.
	 */
	Vec3f operator-() const
	{
		return Vec3f(-x, -y, -z);
	}


	/**
	 * @brief 두 3차원 벡터에 대응하는 원소를 더합니다.
	 *
	 * @param v 벡터의 덧셈을 수행할 피연산자입니다.
	 *
	 * @return 두 벡터에 대응하는 원소를 더한 결과를 반환합니다.
	 */
	Vec3f operator+(Vec3f&& v) const
	{
		return Vec3f(x + v.x, y + v.y, z + v.z);
	}


	/**
	 * @brief 두 3차원 벡터에 대응하는 원소를 더합니다.
	 *
	 * @param v 벡터의 덧셈을 수행할 피연산자입니다.
	 *
	 * @return 두 벡터에 대응하는 원소를 더한 결과를 반환합니다.
	 */
	Vec3f operator+(const Vec3f& v) const
	{
		return Vec3f(x + v.x, y + v.y, z + v.z);
	}


	/**
	 * @brief 두 3차원 벡터에 대응하는 원소를 뺍니다.
	 *
	 * @param v 벡터의 뺄셈을 수행할 피연산자입니다.
	 *
	 * @return 두 벡터에 대응하는 원소를 뺀 결과를 반환합니다.
	 */
	Vec3f operator-(Vec3f&& v) const
	{
		return Vec3f(x - v.x, y - v.y, z - v.z);
	}


	/**
	 * @brief 두 3차원 벡터에 대응하는 원소를 뺍니다.
	 *
	 * @param v 벡터의 뺄셈을 수행할 피연산자입니다.
	 *
	 * @return 두 벡터에 대응하는 원소를 뺀 결과를 반환합니다.
	 */
	Vec3f operator-(const Vec3f& v) const
	{
		return Vec3f(x - v.x, y - v.y, z - v.z);
	}


	/**
	 * @brief 두 3차원 벡터에 대응하는 원소를 각각 곱합니다.
	 *
	 * @note 이 연산은 벡터의 내적 연산(Dot Product)와는 다릅니다.
	 *
	 * @param v 벡터의 곱셈을 수행할 피연산자입니다.
	 *
	 * @return 두 벡터에 대응하는 원소를 곱한 결과를 반환합니다.
	 */
	Vec3f operator*(Vec3f&& v) const
	{
		return Vec3f(x * v.x, y * v.y, z * v.z);
	}


	/**
	 * @brief 두 3차원 벡터에 대응하는 원소를 각각 곱합니다.
	 *
	 * @note 이 연산은 벡터의 내적 연산(Dot Product)와는 다릅니다.
	 *
	 * @param v 벡터의 곱셈을 수행할 피연산자입니다.
	 *
	 * @return 두 벡터에 대응하는 원소를 곱한 결과를 반환합니다.
	 */
	Vec3f operator*(const Vec3f& v) const
	{
		return Vec3f(x * v.x, y * v.y, z * v.z);
	}


	/**
	 * @brief 두 3차원 벡터에 대응하는 원소를 더합니다.
	 *
	 * @param v 연산을 수행할 피연산자입니다.
	 *
	 * @return 연산을 수행한 벡터의 참조자를 반환합니다.
	 */
	Vec3f& operator+=(Vec3f&& v) noexcept
	{
		x += v.x;
		y += v.y;
		z += v.z;

		return *this;
	}


	/**
	 * @brief 두 3차원 벡터에 대응하는 원소를 더합니다.
	 *
	 * @param v 연산을 수행할 피연산자입니다.
	 *
	 * @return 연산을 수행한 벡터의 참조자를 반환합니다.
	 */
	Vec3f& operator+=(const Vec3f& v) noexcept
	{
		x += v.x;
		y += v.y;
		z += v.z;

		return *this;
	}


	/**
	 * @brief 두 3차원 벡터에 대응하는 원소를 뺍니다.
	 *
	 * @param v 연산을 수행할 피연산자입니다.
	 *
	 * @return 연산을 수행한 벡터의 참조자를 반환합니다.
	 */
	Vec3f& operator-=(Vec3f&& v) noexcept
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;

		return *this;
	}


	/**
	 * @brief 두 3차원 벡터에 대응하는 원소를 뺍니다.
	 *
	 * @param v 연산을 수행할 피연산자입니다.
	 *
	 * @return 연산을 수행한 벡터의 참조자를 반환합니다.
	 */
	Vec3f& operator-=(const Vec3f& v) noexcept
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;

		return *this;
	}


	/**
	 * @brief 두 3차원 벡터가 동일한지 검사합니다.
	 *
	 * @param v 검사를 수행할 피연산자입니다.
	 *
	 * @return 두 벡터가 동일하다면 true, 그렇지 않으면 false를 반환합니다.
	 */
	bool operator==(Vec3f&& v) const
	{
		return Abs(x - v.x) <= EPSILON && Abs(y - v.y) <= EPSILON && Abs(z - v.z) <= EPSILON;
	}


	/**
	 * @brief 두 3차원 벡터가 동일한지 검사합니다.
	 *
	 * @param v 검사를 수행할 피연산자입니다.
	 *
	 * @return 두 벡터가 동일하다면 true, 그렇지 않으면 false를 반환합니다.
	 */
	bool operator==(const Vec3f& v) const
	{
		return Abs(x - v.x) <= EPSILON && Abs(y - v.y) <= EPSILON && Abs(z - v.z) <= EPSILON;
	}


	/**
	 * @brief 두 3차원 벡터가 동일하지 않은지 검사합니다.
	 *
	 * @param v 검사를 수행할 피연산자입니다.
	 *
	 * @return 두 벡터가 동일하지 않다면 true, 그렇다면 false를 반환합니다.
	 */
	bool operator!=(Vec3f&& v) const
	{
		return Abs(x - v.x) > EPSILON || Abs(y - v.y) > EPSILON || Abs(z - v.z) > EPSILON;
	}


	/**
	 * @brief 두 3차원 벡터가 동일하지 않은지 검사합니다.
	 *
	 * @param v 검사를 수행할 피연산자입니다.
	 *
	 * @return 두 벡터가 동일하지 않다면 true, 그렇다면 false를 반환합니다.
	 */
	bool operator!=(const Vec3f& v) const
	{
		return Abs(x - v.x) > EPSILON || Abs(y - v.y) > EPSILON || Abs(z - v.z) > EPSILON;
	}


	/**
	 * @brief 3차원 벡터 원소 배열의 포인터를 얻습니다.
	 *
	 * @return 3차원 벡터 원소 배열의 포인터를 반환합니다.
	 */
	const float* GetPtr() const { return &data[0]; }


	/**
	 * @brief 3차원 벡터 원소 배열의 포인터를 얻습니다.
	 *
	 * @return 3차원 벡터 원소 배열의 포인터를 반환합니다.
	 */
	float* GetPtr() { return &data[0]; }


	/**
	 * @brief 3차원 백터의 내적 연산을 수행합니다.
	 *
	 * @param lhs 백터의 내적 연산을 수행할 좌측 피연산자입니다.
	 * @param rhs 백터의 내적 연산을 수행할 우측 피연산자입니다.
	 *
	 * @return 내적 연산 결과를 반환합니다.
	 */
	static inline float Dot(const Vec3f& lhs, const Vec3f& rhs)
	{
		return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
	}


	/**
	 * @brief 3차원 벡터의 외적 연산을 수행합니다.
	 *
	 * @param lhs 백터의 외적 연산을 수행할 좌측 피연산자입니다.
	 * @param rhs 백터의 외적 연산을 수행할 우측 피연산자입니다.
	 *
	 * @return 외적 연산 결과를 반환합니다.
	 */
	static inline Vec3f Cross(const Vec3f& lhs, const Vec3f& rhs)
	{
		return Vec3f(
			lhs.y * rhs.z - lhs.z * rhs.y,
			lhs.z * rhs.x - lhs.x * rhs.z,
			lhs.x * rhs.y - lhs.y * rhs.x
		);
	}


	/**
	 * @brief 3차원 백터의 크기 제곱 값을 계산합니다.
	 *
	 * @param v 크기 제곱을 계산할 벡터입니다.
	 *
	 * @return 계산된 크기 제곱 값을 반환합니다.
	 */
	static inline float LengthSq(const Vec3f& v)
	{
		return v.x * v.x + v.y * v.y + v.z * v.z;
	}


	/**
	 * @brief 3차원 벡터의 크기를 얻습니다.
	 *
	 * @param v 크기를 계산할 벡터입니다.
	 *
	 * @return 계산된 크기 값을 반환합니다.
	 */
	static inline float Length(const Vec3f& v)
	{
		float lengthSq = LengthSq(v);
		return Sqrt(lengthSq);
	}


	/**
	 * @brief 3차원 벡터를 정규화합니다.
	 *
	 * @param v 정규화 할 벡터입니다.
	 *
	 * @return 정규화된 벡터를 반환합니다.
	 */
	static inline Vec3f Normalize(const Vec3f& v)
	{
		float length = Length(v);

		if (NearZero(length))
		{
			return v;
		}

		float invLength = 1.0f / length;
		return Vec3f(v.x * invLength, v.y * invLength, v.z * invLength);
	}


	/**
	 * @brief 두 3차원 벡터 사이의 각을 계산합니다.
	 *
	 * @param lhs 벡터 사이의 각을 계산할 왼쪽 피연산자 벡터입니다.
	 * @param rhs 벡터 사이의 각을 계산할 오른쪽 피연산자 벡터입니다.
	 *
	 * @return 두 3차원 벡터 사이의 라디안 각도를 반환합니다.
	 */
	static inline float Radian(const Vec3f& lhs, const Vec3f& rhs)
	{
		float lengthL = Length(lhs);
		float lengthR = Length(rhs);
		float dot = Dot(lhs, rhs);

		return ACos(dot / (lengthL * lengthR));
	}


	/**
	 * @brief 두 3차원 벡터 사이의 각을 계산합니다.
	 *
	 * @param lhs 벡터 사이의 각을 계산할 왼쪽 피연산자 벡터입니다.
	 * @param rhs 벡터 사이의 각을 계산할 오른쪽 피연산자 벡터입니다.
	 *
	 * @return 두 3차원 벡터 사이의 육십분법 각도를 반환합니다.
	 */
	static inline float Degree(const Vec3f& lhs, const Vec3f& rhs)
	{
		float radian = Radian(lhs, rhs);
		return ToDegree(radian);
	}


	/**
	 * @brief Base 3차원 벡터에 Target 3차원 벡터를 투영합니다.
	 *
	 * @param target 투영 대상에 투영할 벡터입니다.
	 * @param base 투영 대상이 되는 벡터입니다.
	 *
	 * @return base에 투영된 target 벡터를 반환합니다.
	 */
	static inline Vec3f Project(const Vec3f& target, const Vec3f& base)
	{
		float dot = Dot(target, base);
		float lengthSq = LengthSq(base);
		float scale = dot / lengthSq;

		return Vec3f(base.x * scale, base.y * scale, base.z * scale);
	}


	/**
	 * @brief Base 3차원 벡터에 Target 3차원 벡터를 투영한 벡터에 수직인 벡터를 계산합니다.
	 *
	 * @param target 투영 대상에 투영할 벡터입니다.
	 * @param base 투영 대상이 되는 벡터입니다.
	 *
	 * @return 투영된 벡터의 수직인 벡터를 반환합니다.
	 */
	static inline Vec3f Reject(const Vec3f& target, const Vec3f& base)
	{
		Vec3f project = Project(target, base);
		return target - project;
	}


	/**
	 * @brief 두 벡터를 선형 보간한 벡터를 계산합니다.
	 *
	 * @param s 보간의 시작 벡터입니다.
	 * @param e 보간의 끝 벡터입니다.
	 * @param t 두 벡터의 보간 비율입니다.
	 *
	 * @return 보간된 벡터를 반환합니다.
	 */
	static inline Vec3f Lerp(const Vec3f& s, const Vec3f& e, const float& t)
	{
		return s * (1.0f - t) + e * t;
	}


	/**
	 * @brief 두 벡터를 구면 선형 보간한 벡터를 계산합니다.
	 *
	 * @param s 보간의 시작 벡터입니다.
	 * @param e 보간의 끝 벡터입니다.
	 * @param t 두 벡터의 보간 비율입니다.
	 *
	 * @return 보간된 벡터를 반환합니다.
	 */
	static inline Vec3f Slerp(const Vec3f& s, const Vec3f& e, const float& t)
	{
		Vec3f start = Normalize(s);
		Vec3f end = Normalize(e);

		float theta = Radian(start, end);
		float sinTheta = Sin(theta);

		float a = Sin((1.0f - t) * theta) / sinTheta;
		float b = Sin(t * theta) / sinTheta;

		return s * a + e * b;
	}


	/**
	 * @brief 3차원 벡터의 다양한 원소 형식입니다.
	 */
	union
	{
		struct
		{
			float x;
			float y;
			float z;
		};
		float data[3];
	};
};


/**
 * @brief 원소의 데이터 타입이 정수인 3차원 벡터입니다.
 */
struct Vec4i
{
	/**
	 * @brief 4차원 벡터의 기본 생성자입니다.
	 *
	 * @note 모든 원소의 값을 0으로 초기화합니다.
	 */
	Vec4i() noexcept : x(0), y(0), z(0), w(0) {}


	/**
	 * @brief 4차원 벡터의 생성자입니다.
	 *
	 * @param xx 벡터의 x 성분입니다.
	 * @param yy 벡터의 y 성분입니다.
	 * @param zz 벡터의 z 성분입니다.
	 * @param ww 벡터의 w 성분입니다.
	 */
	Vec4i(int32_t&& xx, int32_t&& yy, int32_t&& zz, int32_t&& ww) noexcept : x(xx), y(yy), z(zz), w(ww) {}


	/**
	 * @brief 4차원 벡터의 생성자입니다.
	 *
	 * @param xx 벡터의 x 성분입니다.
	 * @param yy 벡터의 y 성분입니다.
	 * @param zz 벡터의 z 성분입니다.
	 * @param ww 벡터의 w 성분입니다.
	 */
	Vec4i(const int32_t& xx, const int32_t& yy, const int32_t& zz, const int32_t& ww) noexcept : x(xx), y(yy), z(zz), w(ww) {}


	/**
	 * @brief 4차원 벡터의 원소를 하나의 값으로 초기화합니다.
	 *
	 * @param e 모든 원소를 초기화 할 값입니다.
	 */
	Vec4i(int32_t&& e) noexcept : x(e), y(e), z(e), w(e) {}


	/**
	 * @brief 4차원 벡터의 원소를 하나의 값으로 초기화합니다.
	 *
	 * @param e 모든 원소를 초기화 할 값입니다.
	 */
	Vec4i(const int32_t& e) noexcept : x(e), y(e), z(e), w(e) {}


	/**
	 * @brief 4차원 벡터의 복사 생성자입니다.
	 *
	 * @param v 원소를 복사할 벡터 구조체의 인스턴스입니다.
	 */
	Vec4i(Vec4i&& v) noexcept : x(v.x), y(v.y), z(v.z), w(v.w) {}


	/**
	 * @brief 4차원 벡터의 복사 생성자입니다.
	 *
	 * @param v 원소를 복사할 벡터의 인스턴스입니다.
	 */
	Vec4i(const Vec4i& v) noexcept : x(v.x), y(v.y), z(v.z), w(v.w) {}


	/**
	 * @brief 4차원 벡터의 대입 연산자 입니다.
	 *
	 * @param v 원소를 복사할 벡터 구조체의 인스턴스입니다.
	 *
	 * @return 대입한 벡터의 참조자를 반환합니다.
	 */
	Vec4i& operator=(Vec4i&& v) noexcept
	{
		if (this == &v) return *this;

		x = v.x;
		y = v.y;
		z = v.z;
		w = v.w;

		return *this;
	}


	/**
	 * @brief 4차원 벡터의 대입 연산자 입니다.
	 *
	 * @param v 원소를 복사할 벡터 구조체의 인스턴스입니다.
	 *
	 * @return 대입한 벡터의 참조자를 반환합니다.
	 */
	Vec4i& operator=(const Vec4i& v) noexcept
	{
		if (this == &v) return *this;

		x = v.x;
		y = v.y;
		z = v.z;
		w = v.w;

		return *this;
	}


	/**
	 * @brief 4차원 벡터의 원소에 -부호를 취합니다.
	 *
	 * @return 4차원 벡터의 원소에 -부호를 취한 새로운 벡터를 반환합니다.
	 */
	Vec4i operator-() const
	{
		return Vec4i(-x, -y, -z, -w);
	}


	/**
	 * @brief 두 4차원 벡터에 대응하는 원소를 더합니다.
	 *
	 * @param v 벡터의 덧셈을 수행할 피연산자입니다.
	 *
	 * @return 두 벡터에 대응하는 원소를 더한 결과를 반환합니다.
	 */
	Vec4i operator+(Vec4i&& v) const
	{
		return Vec4i(x + v.x, y + v.y, z + v.z, w + v.w);
	}


	/**
	 * @brief 두 4차원 벡터에 대응하는 원소를 더합니다.
	 *
	 * @param v 벡터의 덧셈을 수행할 피연산자입니다.
	 *
	 * @return 두 벡터에 대응하는 원소를 더한 결과를 반환합니다.
	 */
	Vec4i operator+(const Vec4i& v) const
	{
		return Vec4i(x + v.x, y + v.y, z + v.z, w + v.w);
	}


	/**
	 * @brief 두 4차원 벡터에 대응하는 원소를 뺍니다.
	 *
	 * @param v 벡터의 뺄셈을 수행할 피연산자입니다.
	 *
	 * @return 두 벡터에 대응하는 원소를 뺀 결과를 반환합니다.
	 */
	Vec4i operator-(Vec4i&& v) const
	{
		return Vec4i(x - v.x, y - v.y, z - v.z, w - v.w);
	}


	/**
	 * @brief 두 4차원 벡터에 대응하는 원소를 뺍니다.
	 *
	 * @param v 벡터의 뺄셈을 수행할 피연산자입니다.
	 *
	 * @return 두 벡터에 대응하는 원소를 뺀 결과를 반환합니다.
	 */
	Vec4i operator-(const Vec4i& v) const
	{
		return Vec4i(x - v.x, y - v.y, z - v.z, w - v.w);
	}


	/**
	 * @brief 두 4차원 벡터에 대응하는 원소를 각각 곱합니다.
	 *
	 * @note 이 연산은 벡터의 내적 연산(Dot Product)와는 다릅니다.
	 *
	 * @param v 벡터의 곱셈을 수행할 피연산자입니다.
	 *
	 * @return 두 벡터에 대응하는 원소를 곱한 결과를 반환합니다.
	 */
	Vec4i operator*(Vec4i&& v) const
	{
		return Vec4i(x * v.x, y * v.y, z * v.z, w * v.w);
	}


	/**
	 * @brief 두 4차원 벡터에 대응하는 원소를 각각 곱합니다.
	 *
	 * @note 이 연산은 벡터의 내적 연산(Dot Product)와는 다릅니다.
	 *
	 * @param v 벡터의 곱셈을 수행할 피연산자입니다.
	 *
	 * @return 두 벡터에 대응하는 원소를 곱한 결과를 반환합니다.
	 */
	Vec4i operator*(const Vec4i& v) const
	{
		return Vec4i(x * v.x, y * v.y, z * v.z, w * v.w);
	}


	/**
	 * @brief 두 4차원 벡터에 대응하는 원소를 더합니다.
	 *
	 * @param v 연산을 수행할 피연산자입니다.
	 *
	 * @return 연산을 수행한 벡터의 참조자를 반환합니다.
	 */
	Vec4i& operator+=(Vec4i&& v) noexcept
	{
		x += v.x;
		y += v.y;
		z += v.z;
		w += v.w;

		return *this;
	}


	/**
	 * @brief 두 4차원 벡터에 대응하는 원소를 더합니다.
	 *
	 * @param v 연산을 수행할 피연산자입니다.
	 *
	 * @return 연산을 수행한 벡터의 참조자를 반환합니다.
	 */
	Vec4i& operator+=(const Vec4i& v) noexcept
	{
		x += v.x;
		y += v.y;
		z += v.z;
		w += v.w;

		return *this;
	}


	/**
	 * @brief 두 4차원 벡터에 대응하는 원소를 뺍니다.
	 *
	 * @param v 연산을 수행할 피연산자입니다.
	 *
	 * @return 연산을 수행한 벡터의 참조자를 반환합니다.
	 */
	Vec4i& operator-=(Vec4i&& v) noexcept
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		w -= v.w;

		return *this;
	}


	/**
	 * @brief 두 4차원 벡터에 대응하는 원소를 뺍니다.
	 *
	 * @param v 연산을 수행할 피연산자입니다.
	 *
	 * @return 연산을 수행한 벡터의 참조자를 반환합니다.
	 */
	Vec4i& operator-=(const Vec4i& v) noexcept
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		w -= v.w;

		return *this;
	}


	/**
	 * @brief 두 4차원 벡터가 동일한지 검사합니다.
	 *
	 * @param v 검사를 수행할 피연산자입니다.
	 *
	 * @return 두 벡터가 동일하다면 true, 그렇지 않으면 false를 반환합니다.
	 */
	bool operator==(Vec4i&& v) const
	{
		return (x == v.x) && (y == v.y) && (z == v.z) && (w == v.w);
	}


	/**
	 * @brief 두 4차원 벡터가 동일한지 검사합니다.
	 *
	 * @param v 검사를 수행할 피연산자입니다.
	 *
	 * @return 두 벡터가 동일하다면 true, 그렇지 않으면 false를 반환합니다.
	 */
	bool operator==(const Vec4i& v) const
	{
		return (x == v.x) && (y == v.y) && (z == v.z) && (w == v.w);
	}


	/**
	 * @brief 두 4차원 벡터가 동일하지 않은지 검사합니다.
	 *
	 * @param v 검사를 수행할 피연산자입니다.
	 *
	 * @return 두 벡터가 동일하지 않다면 true, 그렇다면 false를 반환합니다.
	 */
	bool operator!=(Vec4i&& v) const
	{
		return (x != v.x) || (y != v.y) || (z != v.z) || (w != v.w);
	}


	/**
	 * @brief 두 4차원 벡터가 동일하지 않은지 검사합니다.
	 *
	 * @param v 검사를 수행할 피연산자입니다.
	 *
	 * @return 두 벡터가 동일하지 않다면 true, 그렇다면 false를 반환합니다.
	 */
	bool operator!=(const Vec4i& v) const
	{
		return (x != v.x) || (y != v.y) || (z != v.z) || (w != v.w);
	}


	/**
	 * @brief 4차원 벡터 원소 배열의 포인터를 얻습니다.
	 *
	 * @return 4차원 벡터 원소 배열의 포인터를 반환합니다.
	 */
	const int32_t* GetPtr() const { return &data[0]; }


	/**
	 * @brief 4차원 벡터 원소 배열의 포인터를 얻습니다.
	 *
	 * @return 4차원 벡터 원소 배열의 포인터를 반환합니다.
	 */
	int32_t* GetPtr() { return &data[0]; }


	/**
	 * @brief 4차원 백터의 내적 연산을 수행합니다.
	 *
	 * @param lhs 백터의 내적 연산을 수행할 좌측 피연산자입니다.
	 * @param rhs 백터의 내적 연산을 수행할 우측 피연산자입니다.
	 *
	 * @return 내적 연산 결과를 반환합니다.
	 */
	static inline int32_t Dot(const Vec4i& lhs, const Vec4i& rhs)
	{
		return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
	}


	/**
	 * @brief 4차원 백터의 크기 제곱 값을 계산합니다.
	 *
	 * @param v 크기 제곱을 계산할 벡터입니다.
	 *
	 * @return 계산된 크기 제곱 값을 반환합니다.
	 */
	static inline int32_t LengthSq(const Vec4i& v)
	{
		return v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w;
	}


	/**
	 * @brief 4차원 벡터의 크기를 얻습니다.
	 *
	 * @param v 크기를 계산할 벡터입니다.
	 *
	 * @return 계산된 크기 값을 반환합니다.
	 */
	static inline float Length(const Vec4i& v)
	{
		float lengthSq = static_cast<float>(LengthSq(v));
		return Sqrt(lengthSq);
	}


	/**
	 * @brief 두 4차원 벡터 사이의 각을 계산합니다.
	 *
	 * @param lhs 벡터 사이의 각을 계산할 왼쪽 피연산자 벡터입니다.
	 * @param rhs 벡터 사이의 각을 계산할 오른쪽 피연산자 벡터입니다.
	 *
	 * @return 두 4차원 벡터 사이의 라디안 각도를 반환합니다.
	 */
	static inline float Radian(const Vec4i& lhs, const Vec4i& rhs)
	{
		float lengthL = Length(lhs);
		float lengthR = Length(rhs);
		float dot = Dot(lhs, rhs);

		return ACos(dot / (lengthL * lengthR));
	}


	/**
	 * @brief 두 4차원 벡터 사이의 각을 계산합니다.
	 *
	 * @param lhs 벡터 사이의 각을 계산할 왼쪽 피연산자 벡터입니다.
	 * @param rhs 벡터 사이의 각을 계산할 오른쪽 피연산자 벡터입니다.
	 *
	 * @return 두 4차원 벡터 사이의 육십분법 각도를 반환합니다.
	 */
	static inline float Degree(const Vec4i& lhs, const Vec4i& rhs)
	{
		float radian = Radian(lhs, rhs);
		return ToDegree(radian);
	}


	/**
	 * @brief 4차원 벡터의 다양한 원소 형식입니다.
	 */
	union
	{
		struct
		{
			int32_t x;
			int32_t y;
			int32_t z;
			int32_t w;
		};
		int32_t data[4];
	};
};


/**
 * @brief 원소의 데이터 타입이 부동 소수점인 4차원 벡터입니다.
 */
struct Vec4f
{
	/**
	 * @brief 4차원 벡터의 기본 생성자입니다.
	 *
	 * @note 모든 원소의 값을 0으로 초기화합니다.
	 */
	Vec4f() noexcept : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}


	/**
	 * @brief 4차원 벡터의 생성자입니다.
	 *
	 * @param xx 벡터의 x 성분입니다.
	 * @param yy 벡터의 y 성분입니다.
	 * @param zz 벡터의 z 성분입니다.
	 * @param ww 벡터의 w 성분입니다.
	 */
	Vec4f(float&& xx, float&& yy, float&& zz, float&& ww) noexcept : x(xx), y(yy), z(zz), w(ww) {}


	/**
	 * @brief 4차원 벡터의 생성자입니다.
	 *
	 * @param xx 벡터의 x 성분입니다.
	 * @param yy 벡터의 y 성분입니다.
	 * @param zz 벡터의 z 성분입니다.
	 * @param ww 벡터의 w 성분입니다.
	 */
	Vec4f(const float& xx, const float& yy, const float& zz, const float& ww) noexcept : x(xx), y(yy), z(zz), w(ww) {}


	/**
	 * @brief 4차원 벡터의 원소를 하나의 값으로 초기화합니다.
	 *
	 * @param e 모든 원소를 초기화 할 값입니다.
	 */
	Vec4f(float&& e) noexcept : x(e), y(e), z(e), w(e) {}


	/**
	 * @brief 4차원 벡터의 원소를 하나의 값으로 초기화합니다.
	 *
	 * @param e 모든 원소를 초기화 할 값입니다.
	 */
	Vec4f(const float& e) noexcept : x(e), y(e), z(e), w(e) {}


	/**
	 * @brief 4차원 벡터의 복사 생성자입니다.
	 *
	 * @param v 원소를 복사할 벡터 구조체의 인스턴스입니다.
	 */
	Vec4f(Vec4f&& v) noexcept : x(v.x), y(v.y), z(v.z), w(v.w) {}


	/**
	 * @brief 4차원 벡터의 복사 생성자입니다.
	 *
	 * @param v 원소를 복사할 벡터의 인스턴스입니다.
	 */
	Vec4f(const Vec4f& v) noexcept : x(v.x), y(v.y), z(v.z), w(v.w) {}


	/**
	 * @brief 4차원 벡터의 대입 연산자 입니다.
	 *
	 * @param v 원소를 복사할 벡터 구조체의 인스턴스입니다.
	 *
	 * @return 대입한 벡터의 참조자를 반환합니다.
	 */
	Vec4f& operator=(Vec4f&& v) noexcept
	{
		if (this == &v) return *this;

		x = v.x;
		y = v.y;
		z = v.z;
		w = v.w;

		return *this;
	}


	/**
	 * @brief 4차원 벡터의 대입 연산자 입니다.
	 *
	 * @param v 원소를 복사할 벡터 구조체의 인스턴스입니다.
	 *
	 * @return 대입한 벡터의 참조자를 반환합니다.
	 */
	Vec4f& operator=(const Vec4f& v) noexcept
	{
		if (this == &v) return *this;

		x = v.x;
		y = v.y;
		z = v.z;
		w = v.w;

		return *this;
	}


	/**
	 * @brief 4차원 벡터의 원소에 -부호를 취합니다.
	 *
	 * @return 4차원 벡터의 원소에 -부호를 취한 새로운 벡터를 반환합니다.
	 */
	Vec4f operator-() const
	{
		return Vec4f(-x, -y, -z, -w);
	}


	/**
	 * @brief 두 4차원 벡터에 대응하는 원소를 더합니다.
	 *
	 * @param v 벡터의 덧셈을 수행할 피연산자입니다.
	 *
	 * @return 두 벡터에 대응하는 원소를 더한 결과를 반환합니다.
	 */
	Vec4f operator+(Vec4f&& v) const
	{
		return Vec4f(x + v.x, y + v.y, z + v.z, w + v.w);
	}


	/**
	 * @brief 두 4차원 벡터에 대응하는 원소를 더합니다.
	 *
	 * @param v 벡터의 덧셈을 수행할 피연산자입니다.
	 *
	 * @return 두 벡터에 대응하는 원소를 더한 결과를 반환합니다.
	 */
	Vec4f operator+(const Vec4f& v) const
	{
		return Vec4f(x + v.x, y + v.y, z + v.z, w + v.w);
	}


	/**
	 * @brief 두 4차원 벡터에 대응하는 원소를 뺍니다.
	 *
	 * @param v 벡터의 뺄셈을 수행할 피연산자입니다.
	 *
	 * @return 두 벡터에 대응하는 원소를 뺀 결과를 반환합니다.
	 */
	Vec4f operator-(Vec4f&& v) const
	{
		return Vec4f(x - v.x, y - v.y, z - v.z, w - v.w);
	}


	/**
	 * @brief 두 4차원 벡터에 대응하는 원소를 뺍니다.
	 *
	 * @param v 벡터의 뺄셈을 수행할 피연산자입니다.
	 *
	 * @return 두 벡터에 대응하는 원소를 뺀 결과를 반환합니다.
	 */
	Vec4f operator-(const Vec4f& v) const
	{
		return Vec4f(x - v.x, y - v.y, z - v.z, w - v.w);
	}


	/**
	 * @brief 두 4차원 벡터에 대응하는 원소를 각각 곱합니다.
	 *
	 * @note 이 연산은 벡터의 내적 연산(Dot Product)와는 다릅니다.
	 *
	 * @param v 벡터의 곱셈을 수행할 피연산자입니다.
	 *
	 * @return 두 벡터에 대응하는 원소를 곱한 결과를 반환합니다.
	 */
	Vec4f operator*(Vec4f&& v) const
	{
		return Vec4f(x * v.x, y * v.y, z * v.z, w * v.w);
	}


	/**
	 * @brief 두 4차원 벡터에 대응하는 원소를 각각 곱합니다.
	 *
	 * @note 이 연산은 벡터의 내적 연산(Dot Product)와는 다릅니다.
	 *
	 * @param v 벡터의 곱셈을 수행할 피연산자입니다.
	 *
	 * @return 두 벡터에 대응하는 원소를 곱한 결과를 반환합니다.
	 */
	Vec4f operator*(const Vec4f& v) const
	{
		return Vec4f(x * v.x, y * v.y, z * v.z, w * v.w);
	}


	/**
	 * @brief 두 4차원 벡터에 대응하는 원소를 더합니다.
	 *
	 * @param v 연산을 수행할 피연산자입니다.
	 *
	 * @return 연산을 수행한 벡터의 참조자를 반환합니다.
	 */
	Vec4f& operator+=(Vec4f&& v) noexcept
	{
		x += v.x;
		y += v.y;
		z += v.z;
		w += v.w;

		return *this;
	}


	/**
	 * @brief 두 4차원 벡터에 대응하는 원소를 더합니다.
	 *
	 * @param v 연산을 수행할 피연산자입니다.
	 *
	 * @return 연산을 수행한 벡터의 참조자를 반환합니다.
	 */
	Vec4f& operator+=(const Vec4f& v) noexcept
	{
		x += v.x;
		y += v.y;
		z += v.z;
		w += v.w;

		return *this;
	}


	/**
	 * @brief 두 4차원 벡터에 대응하는 원소를 뺍니다.
	 *
	 * @param v 연산을 수행할 피연산자입니다.
	 *
	 * @return 연산을 수행한 벡터의 참조자를 반환합니다.
	 */
	Vec4f& operator-=(Vec4f&& v) noexcept
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		w -= v.w;

		return *this;
	}


	/**
	 * @brief 두 4차원 벡터에 대응하는 원소를 뺍니다.
	 *
	 * @param v 연산을 수행할 피연산자입니다.
	 *
	 * @return 연산을 수행한 벡터의 참조자를 반환합니다.
	 */
	Vec4f& operator-=(const Vec4f& v) noexcept
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		w -= v.w;

		return *this;
	}


	/**
	 * @brief 두 4차원 벡터가 동일한지 검사합니다.
	 *
	 * @param v 검사를 수행할 피연산자입니다.
	 *
	 * @return 두 벡터가 동일하다면 true, 그렇지 않으면 false를 반환합니다.
	 */
	bool operator==(Vec4f&& v) const
	{
		return Abs(x - v.x) <= EPSILON && Abs(y - v.y) <= EPSILON && Abs(z - v.z) <= EPSILON && Abs(w - v.w) <= EPSILON;
	}


	/**
	 * @brief 두 4차원 벡터가 동일한지 검사합니다.
	 *
	 * @param v 검사를 수행할 피연산자입니다.
	 *
	 * @return 두 벡터가 동일하다면 true, 그렇지 않으면 false를 반환합니다.
	 */
	bool operator==(const Vec4f& v) const
	{
		return Abs(x - v.x) <= EPSILON && Abs(y - v.y) <= EPSILON && Abs(z - v.z) <= EPSILON && Abs(w - v.w) <= EPSILON;
	}


	/**
	 * @brief 두 4차원 벡터가 동일하지 않은지 검사합니다.
	 *
	 * @param v 검사를 수행할 피연산자입니다.
	 *
	 * @return 두 벡터가 동일하지 않다면 true, 그렇다면 false를 반환합니다.
	 */
	bool operator!=(Vec4f&& v) const
	{
		return Abs(x - v.x) > EPSILON || Abs(y - v.y) > EPSILON || Abs(z - v.z) > EPSILON || Abs(w - v.w) > EPSILON;
	}


	/**
	 * @brief 두 4차원 벡터가 동일하지 않은지 검사합니다.
	 *
	 * @param v 검사를 수행할 피연산자입니다.
	 *
	 * @return 두 벡터가 동일하지 않다면 true, 그렇다면 false를 반환합니다.
	 */
	bool operator!=(const Vec4f& v) const
	{
		return Abs(x - v.x) > EPSILON || Abs(y - v.y) > EPSILON || Abs(z - v.z) > EPSILON || Abs(w - v.w) > EPSILON;
	}


	/**
	 * @brief 4차원 벡터 원소 배열의 포인터를 얻습니다.
	 *
	 * @return 4차원 벡터 원소 배열의 포인터를 반환합니다.
	 */
	const float* GetPtr() const { return &data[0]; }


	/**
	 * @brief 4차원 벡터 원소 배열의 포인터를 얻습니다.
	 *
	 * @return 4차원 벡터 원소 배열의 포인터를 반환합니다.
	 */
	float* GetPtr() { return &data[0]; }


	/**
	 * @brief 4차원 백터의 내적 연산을 수행합니다.
	 *
	 * @param lhs 백터의 내적 연산을 수행할 좌측 피연산자입니다.
	 * @param rhs 백터의 내적 연산을 수행할 우측 피연산자입니다.
	 *
	 * @return 내적 연산 결과를 반환합니다.
	 */
	static inline float Dot(const Vec4f& lhs, const Vec4f& rhs)
	{
		return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
	}


	/**
	 * @brief 4차원 백터의 크기 제곱 값을 계산합니다.
	 *
	 * @param v 크기 제곱을 계산할 벡터입니다.
	 *
	 * @return 계산된 크기 제곱 값을 반환합니다.
	 */
	static inline float LengthSq(const Vec4f& v)
	{
		return v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w;
	}


	/**
	 * @brief 4차원 벡터의 크기를 얻습니다.
	 *
	 * @param v 크기를 계산할 벡터입니다.
	 *
	 * @return 계산된 크기 값을 반환합니다.
	 */
	static inline float Length(const Vec4f& v)
	{
		float lengthSq = LengthSq(v);
		return Sqrt(lengthSq);
	}


	/**
	 * @brief 4차원 벡터를 정규화합니다.
	 *
	 * @param v 정규화 할 벡터입니다.
	 *
	 * @return 정규화된 벡터를 반환합니다.
	 */
	static inline Vec4f Normalize(const Vec4f& v)
	{
		float length = Length(v);

		if (NearZero(length))
		{
			return v;
		}

		float invLength = 1.0f / length;
		return Vec4f(v.x * invLength, v.y * invLength, v.z * invLength, v.w * invLength);
	}


	/**
	 * @brief 두 4차원 벡터 사이의 각을 계산합니다.
	 *
	 * @param lhs 벡터 사이의 각을 계산할 왼쪽 피연산자 벡터입니다.
	 * @param rhs 벡터 사이의 각을 계산할 오른쪽 피연산자 벡터입니다.
	 *
	 * @return 두 4차원 벡터 사이의 라디안 각도를 반환합니다.
	 */
	static inline float Radian(const Vec4f& lhs, const Vec4f& rhs)
	{
		float lengthL = Length(lhs);
		float lengthR = Length(rhs);
		float dot = Dot(lhs, rhs);

		return ACos(dot / (lengthL * lengthR));
	}


	/**
	 * @brief 두 4차원 벡터 사이의 각을 계산합니다.
	 *
	 * @param lhs 벡터 사이의 각을 계산할 왼쪽 피연산자 벡터입니다.
	 * @param rhs 벡터 사이의 각을 계산할 오른쪽 피연산자 벡터입니다.
	 *
	 * @return 두 4차원 벡터 사이의 육십분법 각도를 반환합니다.
	 */
	static inline float Degree(const Vec4f& lhs, const Vec4f& rhs)
	{
		float radian = Radian(lhs, rhs);
		return ToDegree(radian);
	}


	/**
	 * @brief Base 4차원 벡터에 Target 4차원 벡터를 투영합니다.
	 *
	 * @param target 투영 대상에 투영할 벡터입니다.
	 * @param base 투영 대상이 되는 벡터입니다.
	 *
	 * @return base에 투영된 target 벡터를 반환합니다.
	 */
	static inline Vec4f Project(const Vec4f& target, const Vec4f& base)
	{
		float dot = Dot(target, base);
		float lengthSq = LengthSq(base);
		float scale = dot / lengthSq;

		return Vec4f(base.x * scale, base.y * scale, base.z * scale, base.w * scale);
	}


	/**
	 * @brief Base 4차원 벡터에 Target 4차원 벡터를 투영한 벡터에 수직인 벡터를 계산합니다.
	 *
	 * @param target 투영 대상에 투영할 벡터입니다.
	 * @param base 투영 대상이 되는 벡터입니다.
	 *
	 * @return 투영된 벡터의 수직인 벡터를 반환합니다.
	 */
	static inline Vec4f Reject(const Vec4f& target, const Vec4f& base)
	{
		Vec4f project = Project(target, base);
		return target - project;
	}


	/**
	 * @brief 두 벡터를 선형 보간한 벡터를 계산합니다.
	 *
	 * @param s 보간의 시작 벡터입니다.
	 * @param e 보간의 끝 벡터입니다.
	 * @param t 두 벡터의 보간 비율입니다.
	 *
	 * @return 보간된 벡터를 반환합니다.
	 */
	static inline Vec4f Lerp(const Vec4f& s, const Vec4f& e, const float& t)
	{
		return s * (1.0f - t) + e * t;
	}


	/**
	 * @brief 두 벡터를 구면 선형 보간한 벡터를 계산합니다.
	 *
	 * @param s 보간의 시작 벡터입니다.
	 * @param e 보간의 끝 벡터입니다.
	 * @param t 두 벡터의 보간 비율입니다.
	 *
	 * @return 보간된 벡터를 반환합니다.
	 */
	static inline Vec4f Slerp(const Vec4f& s, const Vec4f& e, const float& t)
	{
		Vec4f start = Normalize(s);
		Vec4f end = Normalize(e);

		float theta = Radian(start, end);
		float sinTheta = Sin(theta);

		float a = Sin((1.0f - t) * theta) / sinTheta;
		float b = Sin(t * theta) / sinTheta;

		return s * a + e * b;
	}


	/**
	 * @brief 4차원 벡터의 다양한 원소 형식입니다.
	 */
	union
	{
		struct
		{
			float x;
			float y;
			float z;
			float w;
		};
		float data[4];
	};
};


/**
 * @brief 2x2 행렬입니다.
 */
struct Mat2x2
{
	/**
	 * @brief 2x2 행렬의 기본 생성자입니다.
	 *
	 * @note 모든 원소의 값을 0으로 초기화합니다.
	 */
	Mat2x2() noexcept
	{
		e00 = 0.0f; e01 = 0.0f;
		e10 = 0.0f; e11 = 0.0f;
	}


	/**
	 * @brief 2x2 행렬의 생성자입니다.
	 *
	 * @param ee00 행렬의 (0, 0) 성분입니다.
	 * @param ee01 행렬의 (0, 1) 성분입니다.
	 * @param ee10 행렬의 (1, 0) 성분입니다.
	 * @param ee11 행렬의 (1, 1) 성분입니다.
	 */
	Mat2x2(
		float&& ee00, float&& ee01,
		float&& ee10, float&& ee11
	) noexcept
	{
		e00 = ee00; e01 = ee01;
		e10 = ee10; e11 = ee11;
	}


	/**
	 * @brief 2x2 행렬의 생성자입니다.
	 *
	 * @param e00 행렬의 (0, 0) 성분입니다.
	 * @param e01 행렬의 (0, 1) 성분입니다.
	 * @param e10 행렬의 (1, 0) 성분입니다.
	 * @param e11 행렬의 (1, 1) 성분입니다.
	 */
	Mat2x2(
		const float& ee00, const float& ee01,
		const float& ee10, const float& ee11
	) noexcept
	{
		e00 = ee00; e01 = ee01;
		e10 = ee10; e11 = ee11;
	}


	/**
	 * @brief 2x2 행렬의 원소를 하나의 값으로 초기화합니다.
	 *
	 * @param e 모든 원소를 초기화 할 값입니다.
	 */
	Mat2x2(float&& e) noexcept
	{
		e00 = e; e01 = e;
		e10 = e; e11 = e;
	}


	/**
	 * @brief 2x2 행렬의 원소를 하나의 값으로 초기화합니다.
	 *
	 * @param e 모든 원소를 초기화 할 값입니다.
	 */
	Mat2x2(const float& e) noexcept
	{
		e00 = e; e01 = e;
		e10 = e; e11 = e;
	}


	/**
	 * @brief 2x2 행렬의 복사 생성자입니다.
	 *
	 * @param m 원소를 복사할 행렬의 인스턴스입니다.
	 */
	Mat2x2(Mat2x2&& m) noexcept
	{
		e00 = m.e00; e01 = m.e01;
		e10 = m.e10; e11 = m.e11;
	}


	/**
	 * @brief 2x2 행렬의 복사 생성자입니다.
	 *
	 * @param m 원소를 복사할 행렬의 인스턴스입니다.
	 */
	Mat2x2(const Mat2x2& m) noexcept
	{
		e00 = m.e00; e01 = m.e01;
		e10 = m.e10; e11 = m.e11;
	}


	/**
	 * @brief 2x2 행렬의 대입 연산자입니다.
	 *
	 * @param m 원소를 복사할 행렬의 인스턴스입니다.
	 *
	 * @return 대입한 행렬의 참조자를 반환합니다.
	 */
	Mat2x2& operator=(Mat2x2&& m) noexcept
	{
		if (this == &m) return *this;

		e00 = m.e00; e01 = m.e01;
		e10 = m.e10; e11 = m.e11;

		return *this;
	}


	/**
	 * @brief 2x2 행렬의 대입 연산자입니다.
	 *
	 * @param m 원소를 복사할 행렬의 인스턴스입니다.
	 *
	 * @return 대입한 행렬의 참조자를 반환합니다.
	 */
	Mat2x2& operator=(const Mat2x2& m) noexcept
	{
		if (this == &m) return *this;

		e00 = m.e00; e01 = m.e01;
		e10 = m.e10; e11 = m.e11;

		return *this;
	}


	/**
	 * @brief 2x2 행렬의 모든 원소에 -부호를 취합니다.
	 *
	 * @return 모든 원소에 -부호를 취한 새로운 행렬을 반환합니다.
	 */
	Mat2x2 operator-() const
	{
		return Mat2x2(
			-e00, -e01,
			-e10, -e11
		);
	}


	/**
	 * @brief 두 2x2 행렬의 대응하는 원소를 더합니다.
	 *
	 * @param m 행렬의 덧셈을 수행할 피연산자입니다.
	 *
	 * @return 두 행렬의 대응하는 원소를 더한 결과를 반환합니다.
	 */
	Mat2x2 operator+(Mat2x2&& m) const
	{
		return Mat2x2(
			e00 + m.e00, e01 + m.e01,
			e10 + m.e10, e11 + m.e11
		);
	}


	/**
	 * @brief 두 2x2 행렬의 대응하는 원소를 더합니다.
	 *
	 * @param m 행렬의 덧셈을 수행할 피연산자입니다.
	 *
	 * @return 두 행렬의 대응하는 원소를 더한 결과를 반환합니다.
	 */
	Mat2x2 operator+(const Mat2x2& m) const
	{
		return Mat2x2(
			e00 + m.e00, e01 + m.e01,
			e10 + m.e10, e11 + m.e11
		);
	}


	/**
	 * @brief 두 2x2 행렬의 대응하는 원소를 뺍니다.
	 *
	 * @param m 행렬의 뺄셈을 수행할 피연산자입니다.
	 *
	 * @return 두 행렬의 대응하는 원소를 뺀 결과를 반환합니다.
	 */
	Mat2x2 operator-(Mat2x2&& m) const
	{
		return Mat2x2(
			e00 - m.e00, e01 - m.e01,
			e10 - m.e10, e11 - m.e11
		);
	}


	/**
	 * @brief 두 2x2 행렬의 대응하는 원소를 뺍니다.
	 *
	 * @param m 행렬의 뺄셈을 수행할 피연산자입니다.
	 *
	 * @return 두 행렬의 대응하는 원소를 뺀 결과를 반환합니다.
	 */
	Mat2x2 operator-(const Mat2x2& m) const
	{
		return Mat2x2(
			e00 - m.e00, e01 - m.e01,
			e10 - m.e10, e11 - m.e11
		);
	}


	/**
	 * @brief 2x2 행렬에 부동소수점을 곱합니다.
	 *
	 * @param scalar 2x2 행렬에 부동수소점 수를 곱할 스칼라 값입니다.
	 *
	 * @return 2x2 행렬에 부동소수점을 곱합 결과를 반환합니다.
	 */
	Mat2x2 operator*(float&& scalar) const
	{
		return Mat2x2(
			scalar * e00, scalar * e01,
			scalar * e10, scalar * e11
		);
	}


	/**
	 * @brief 2x2 행렬에 부동소수점을 곱합니다.
	 *
	 * @param scalar 2x2 행렬에 부동수소점 수를 곱할 스칼라 값입니다.
	 *
	 * @return 2x2 행렬에 부동소수점을 곱합 결과를 반환합니다.
	 */
	Mat2x2 operator*(const float& scalar) const
	{
		return Mat2x2(
			scalar * e00, scalar * e01,
			scalar * e10, scalar * e11
		);
	}


	/**
	 * @brief 두 2x2 행렬을 곱합니다.
	 *
	 * @param m 행렬의 곱셈을 수행할 피연산자입니다.
	 *
	 * @return 두 행렬을 곱한 결과를 반환합니다.
	 */
	Mat2x2 operator*(Mat2x2&& m) const
	{
		return Mat2x2(
			e00 * m.e00 + e01 * m.e10,
			e00 * m.e01 + e01 * m.e11,
			e10 * m.e00 + e11 * m.e10,
			e10 * m.e01 + e11 * m.e11
		);
	}


	/**
	 * @brief 두 2x2 행렬을 곱합니다.
	 *
	 * @param m 행렬의 곱셈을 수행할 피연산자입니다.
	 *
	 * @return 두 행렬을 곱한 결과를 반환합니다.
	 */
	Mat2x2 operator*(const Mat2x2& m) const
	{
		return Mat2x2(
			e00 * m.e00 + e01 * m.e10,
			e00 * m.e01 + e01 * m.e11,
			e10 * m.e00 + e11 * m.e10,
			e10 * m.e01 + e11 * m.e11
		);
	}


	/**
	 * @brief 두 2x2 행렬에 대응하는 원소를 더합니다.
	 *
	 * @param m 연산을 수행할 피연산자입니다.
	 *
	 * @return 연산을 수행한 행렬의 참조자를 반환합니다.
	 */
	Mat2x2& operator+=(Mat2x2&& m) noexcept
	{
		e00 += m.e00; e01 += m.e01;
		e10 += m.e10; e11 += m.e11;

		return *this;
	}


	/**
	 * @brief 두 2x2 행렬에 대응하는 원소를 더합니다.
	 *
	 * @param m 연산을 수행할 피연산자입니다.
	 *
	 * @return 연산을 수행한 행렬의 참조자를 반환합니다.
	 */
	Mat2x2& operator+=(const Mat2x2& m) noexcept
	{
		e00 += m.e00; e01 += m.e01;
		e10 += m.e10; e11 += m.e11;

		return *this;
	}


	/**
	 * @brief 두 2x2 행렬에 대응하는 원소를 뺍니다.
	 *
	 * @param m 연산을 수행할 피연산자입니다.
	 *
	 * @return 연산을 수행한 행렬의 참조자를 반환합니다.
	 */
	Mat2x2& operator-=(Mat2x2&& m) noexcept
	{
		e00 -= m.e00; e01 -= m.e01;
		e10 -= m.e10; e11 -= m.e11;

		return *this;
	}


	/**
	 * @brief 두 2x2 행렬에 대응하는 원소를 뺍니다.
	 *
	 * @param m 연산을 수행할 피연산자입니다.
	 *
	 * @return 연산을 수행한 행렬의 참조자를 반환합니다.
	 */
	Mat2x2& operator-=(const Mat2x2& m) noexcept
	{
		e00 -= m.e00; e01 -= m.e01;
		e10 -= m.e10; e11 -= m.e11;

		return *this;
	}


	/**
	 * @brief 두 2x2행렬이 동일한지 검사합니다.
	 *
	 * @param m 검사를 수행할 피연산자입니다.
	 *
	 * @return 두 행렬이 동일하다면 true, 그렇지 않으면 false를 반환합니다.
	 */
	bool operator==(Mat2x2&& m) const
	{
		return Abs(e00 - m.e00) <= EPSILON
			&& Abs(e01 - m.e01) <= EPSILON
			&& Abs(e10 - m.e10) <= EPSILON
			&& Abs(e11 - m.e11) <= EPSILON;
	}


	/**
	 * @brief 두 2x2행렬이 동일한지 검사합니다.
	 *
	 * @param m 검사를 수행할 피연산자입니다.
	 *
	 * @return 두 행렬이 동일하다면 true, 그렇지 않으면 false를 반환합니다.
	 */
	bool operator==(const Mat2x2& m) const
	{
		return Abs(e00 - m.e00) <= EPSILON
			&& Abs(e01 - m.e01) <= EPSILON
			&& Abs(e10 - m.e10) <= EPSILON
			&& Abs(e11 - m.e11) <= EPSILON;
	}


	/**
	 * @brief 두 2x2행렬이 동일하지 않은지 검사합니다.
	 *
	 * @param m 검사를 수행할 피연산자입니다.
	 *
	 * @return 두 행렬이 동일하지 않다면 true, 그렇다면 false를 반환합니다.
	 */
	bool operator!=(Mat2x2&& m) const
	{
		return Abs(e00 - m.e00) > EPSILON
			|| Abs(e01 - m.e01) > EPSILON
			|| Abs(e10 - m.e10) > EPSILON
			|| Abs(e11 - m.e11) > EPSILON;
	}


	/**
	 * @brief 두 2x2행렬이 동일하지 않은지 검사합니다.
	 *
	 * @param m 검사를 수행할 피연산자입니다.
	 *
	 * @return 두 행렬이 동일하지 않다면 true, 그렇다면 false를 반환합니다.
	 */
	bool operator!=(const Mat2x2& m) const
	{
		return Abs(e00 - m.e00) > EPSILON
			|| Abs(e01 - m.e01) > EPSILON
			|| Abs(e10 - m.e10) > EPSILON
			|| Abs(e11 - m.e11) > EPSILON;
	}


	/**
	 * @brief 2x2 행렬 원소 배열의 포인터를 얻습니다.
	 *
	 * @return 2x2 행렬 원소 배열의 포인터를 반환합니다.
	 */
	const float* GetPtr() const { return &data[0]; }


	/**
	 * @brief 2x2 행렬 원소 배열의 포인터를 얻습니다.
	 *
	 * @return 2x2 행렬 원소 배열의 포인터를 반환합니다.
	 */
	float* GetPtr() { return &data[0]; }


	/**
	 * @brief 2x2 행렬의 모든 원소가 0인 행렬을 얻습니다.
	 *
	 * @return 모든 원소가 0인 2x2 행렬을 반환합니다.
	 */
	static inline Mat2x2 Zero()
	{
		return Mat2x2(
			0.0f, 0.0f,
			0.0f, 0.0f
		);
	}


	/**
	 * @brief 2x2 행렬의 단위 행렬을 얻습니다.
	 *
	 * @return 2x2 행렬의 단위 행렬를 반환합니다.
	 */
	static inline Mat2x2 Identity()
	{
		return Mat2x2(
			1.0f, 0.0f,
			0.0f, 1.0f
		);
	}


	/**
	 * @brief 2x2 행렬의 전치 행렬을 얻습니다.
	 *
	 * @param m 원소들을 전치할 2x2 행렬입니다.
	 *
	 * @return 원소가 전치된 2x2 행렬을 반환합니다.
	 */
	static inline Mat2x2 Transpose(const Mat2x2& m)
	{
		return Mat2x2(
			m.e00, m.e10,
			m.e01, m.e11
		);
	}


	/**
	 * @brief 2x2 행렬의 행렬식 값을 얻습니다.
	 *
	 * @param m 행렬식 값을 계산할 2x2 행렬입니다.
	 *
	 * @return 2x2 행렬의 행렬식 값을 반환합니다.
	 */
	static inline float Determinant(const Mat2x2& m)
	{
		return m.e00 * m.e11 - m.e01 * m.e10;
	}


	/**
	 * @brief 2x2 행렬의 역행렬을 얻습니다.
	 *
	 * @param m 역행렬을 계산할 2x2 행렬입니다.
	 *
	 * @return 2x2 행렬의 역행렬을 반환합니다.
	 */
	static inline Mat2x2 Inverse(const Mat2x2& m)
	{
		float oneOverDeterminant = 1.0f / Determinant(m);

		return Mat2x2(
			+m.e11 * oneOverDeterminant,
			-m.e01 * oneOverDeterminant,
			-m.e10 * oneOverDeterminant,
			+m.e00 * oneOverDeterminant
		);
	}


	/**
	 * @brief 2x2 행렬의 원소입니다.
	 */
	union
	{
		struct
		{
			float e00; float e01;
			float e10; float e11;
		};
		float data[4];
	};
};


/**
 * @brief 3x3 행렬입니다.
 */
struct Mat3x3
{
	/**
	 * @brief 3x3 행렬의 기본 생성자입니다.
	 *
	 * @note 모든 원소의 값을 0으로 초기화합니다.
	 */
	Mat3x3() noexcept
	{
		e00 = 0.0f; e01 = 0.0f; e02 = 0.0f;
		e10 = 0.0f; e11 = 0.0f; e12 = 0.0f;
		e20 = 0.0f; e21 = 0.0f; e22 = 0.0f;
	}


	/**
	 * @brief 3x3 행렬의 생성자입니다.
	 *
	 * @param ee00 행렬의 (0, 0) 성분입니다.
	 * @param ee01 행렬의 (0, 1) 성분입니다.
	 * @param ee02 행렬의 (0, 2) 성분입니다.
	 * @param ee10 행렬의 (1, 0) 성분입니다.
	 * @param ee11 행렬의 (1, 1) 성분입니다.
	 * @param ee12 행렬의 (1, 2) 성분입니다.
	 * @param ee20 행렬의 (2, 0) 성분입니다.
	 * @param ee21 행렬의 (2, 1) 성분입니다.
	 * @param ee22 행렬의 (2, 2) 성분입니다.
	 */
	Mat3x3(
		float&& ee00, float&& ee01, float&& ee02,
		float&& ee10, float&& ee11, float&& ee12,
		float&& ee20, float&& ee21, float&& ee22
	) noexcept
	{
		e00 = ee00; e01 = ee01; e02 = ee02;
		e10 = ee10; e11 = ee11; e12 = ee12;
		e20 = ee20; e21 = ee21; e22 = ee22;
	}


	/**
	 * @brief 3x3 행렬의 생성자입니다.
	 *
	 * @param ee00 행렬의 (0, 0) 성분입니다.
	 * @param ee01 행렬의 (0, 1) 성분입니다.
	 * @param ee02 행렬의 (0, 2) 성분입니다.
	 * @param ee10 행렬의 (1, 0) 성분입니다.
	 * @param ee11 행렬의 (1, 1) 성분입니다.
	 * @param ee12 행렬의 (1, 2) 성분입니다.
	 * @param ee20 행렬의 (2, 0) 성분입니다.
	 * @param ee21 행렬의 (2, 1) 성분입니다.
	 * @param ee22 행렬의 (2, 2) 성분입니다.
	 */
	Mat3x3(
		const float& ee00, const float& ee01, const float& ee02,
		const float& ee10, const float& ee11, const float& ee12,
		const float& ee20, const float& ee21, const float& ee22
	) noexcept
	{
		e00 = ee00; e01 = ee01; e02 = ee02;
		e10 = ee10; e11 = ee11; e12 = ee12;
		e20 = ee20; e21 = ee21; e22 = ee22;
	}


	/**
	 * @brief 3x3 행렬의 원소를 하나의 값으로 초기화합니다.
	 *
	 * @param e 모든 원소를 초기화 할 값입니다.
	 */
	Mat3x3(float&& e) noexcept
	{
		e00 = e; e01 = e; e02 = e;
		e10 = e; e11 = e; e12 = e;
		e20 = e; e21 = e; e22 = e;
	}


	/**
	 * @brief 3x3 행렬의 원소를 하나의 값으로 초기화합니다.
	 *
	 * @param e 모든 원소를 초기화 할 값입니다.
	 */
	Mat3x3(const float& e) noexcept
	{
		e00 = e; e01 = e; e02 = e;
		e10 = e; e11 = e; e12 = e;
		e20 = e; e21 = e; e22 = e;
	}


	/**
	 * @brief 3x3 행렬의 복사 생성자입니다.
	 *
	 * @param m 원소를 복사할 행렬의 인스턴스입니다.
	 */
	Mat3x3(Mat3x3&& m) noexcept
	{
		e00 = m.e00; e01 = m.e01; e02 = m.e02;
		e10 = m.e10; e11 = m.e11; e12 = m.e12;
		e20 = m.e20; e21 = m.e21; e22 = m.e22;
	}


	/**
	 * @brief 3x3 행렬의 복사 생성자입니다.
	 *
	 * @param m 원소를 복사할 행렬의 인스턴스입니다.
	 */
	Mat3x3(const Mat3x3& m) noexcept
	{
		e00 = m.e00; e01 = m.e01; e02 = m.e02;
		e10 = m.e10; e11 = m.e11; e12 = m.e12;
		e20 = m.e20; e21 = m.e21; e22 = m.e22;
	}


	/**
	 * @brief 3x3 행렬의 대입 연산자입니다.
	 *
	 * @param m 원소를 복사할 행렬의 인스턴스입니다.
	 *
	 * @return 대입한 행렬의 참조자를 반환합니다.
	 */
	Mat3x3& operator=(Mat3x3&& m) noexcept
	{
		if (this == &m) return *this;

		e00 = m.e00; e01 = m.e01; e02 = m.e02;
		e10 = m.e10; e11 = m.e11; e12 = m.e12;
		e20 = m.e20; e21 = m.e21; e22 = m.e22;

		return *this;
	}


	/**
	 * @brief 3x3 행렬의 대입 연산자입니다.
	 *
	 * @param m 원소를 복사할 행렬의 인스턴스입니다.
	 *
	 * @return 대입한 행렬의 참조자를 반환합니다.
	 */
	Mat3x3& operator=(const Mat3x3& m) noexcept
	{
		if (this == &m) return *this;

		e00 = m.e00; e01 = m.e01; e02 = m.e02;
		e10 = m.e10; e11 = m.e11; e12 = m.e12;
		e20 = m.e20; e21 = m.e21; e22 = m.e22;

		return *this;
	}


	/**
	 * @brief 3x3 행렬의 모든 원소에 -부호를 취합니다.
	 *
	 * @return 모든 원소에 -부호를 취한 새로운 행렬을 반환합니다.
	 */
	Mat3x3 operator-() const
	{
		return Mat3x3(
			-e00, -e01, -e02,
			-e10, -e11, -e12,
			-e20, -e21, -e22
		);
	}


	/**
	 * @brief 두 3x3 행렬의 대응하는 원소를 더합니다.
	 *
	 * @param m 행렬의 덧셈을 수행할 피연산자입니다.
	 *
	 * @return 두 행렬의 대응하는 원소를 더한 결과를 반환합니다.
	 */
	Mat3x3 operator+(Mat3x3&& m) const
	{
		return Mat3x3(
			e00 + m.e00, e01 + m.e01, e02 + m.e02,
			e10 + m.e10, e11 + m.e11, e12 + m.e12,
			e20 + m.e20, e21 + m.e21, e22 + m.e22
		);
	}


	/**
	 * @brief 두 3x3 행렬의 대응하는 원소를 더합니다.
	 *
	 * @param m 행렬의 덧셈을 수행할 피연산자입니다.
	 *
	 * @return 두 행렬의 대응하는 원소를 더한 결과를 반환합니다.
	 */
	Mat3x3 operator+(const Mat3x3& m) const
	{
		return Mat3x3(
			e00 + m.e00, e01 + m.e01, e02 + m.e02,
			e10 + m.e10, e11 + m.e11, e12 + m.e12,
			e20 + m.e20, e21 + m.e21, e22 + m.e22
		);
	}


	/**
	 * @brief 두 3x3 행렬의 대응하는 원소를 뺍니다.
	 *
	 * @param m 행렬의 뺄셈을 수행할 피연산자입니다.
	 *
	 * @return 두 행렬의 대응하는 원소를 뺀 결과를 반환합니다.
	 */
	Mat3x3 operator-(Mat3x3&& m) const
	{
		return Mat3x3(
			e00 - m.e00, e01 - m.e01, e02 - m.e02,
			e10 - m.e10, e11 - m.e11, e12 - m.e12,
			e20 - m.e20, e21 - m.e21, e22 - m.e22
		);
	}


	/**
	 * @brief 두 3x3 행렬의 대응하는 원소를 뺍니다.
	 *
	 * @param m 행렬의 뺄셈을 수행할 피연산자입니다.
	 *
	 * @return 두 행렬의 대응하는 원소를 뺀 결과를 반환합니다.
	 */
	Mat3x3 operator-(const Mat3x3& m) const
	{
		return Mat3x3(
			e00 - m.e00, e01 - m.e01, e02 - m.e02,
			e10 - m.e10, e11 - m.e11, e12 - m.e12,
			e20 - m.e20, e21 - m.e21, e22 - m.e22
		);
	}


	/**
	 * @brief 3x3 행렬에 부동소수점을 곱합니다.
	 *
	 * @param scalar 3x3 행렬에 부동수소점 수를 곱할 스칼라 값입니다.
	 *
	 * @return 3x3 행렬에 부동소수점을 곱합 결과를 반환합니다.
	 */
	Mat3x3 operator*(float&& scalar) const
	{
		return Mat3x3(
			scalar * e00, scalar * e01, scalar * e02,
			scalar * e10, scalar * e11, scalar * e12,
			scalar * e20, scalar * e21, scalar * e22
		);
	}


	/**
	 * @brief 3x3 행렬에 부동소수점을 곱합니다.
	 *
	 * @param scalar 3x3 행렬에 부동수소점 수를 곱할 스칼라 값입니다.
	 *
	 * @return 3x3 행렬에 부동소수점을 곱합 결과를 반환합니다.
	 */
	Mat3x3 operator*(const float& scalar) const
	{
		return Mat3x3(
			scalar * e00, scalar * e01, scalar * e02,
			scalar * e10, scalar * e11, scalar * e12,
			scalar * e20, scalar * e21, scalar * e22
		);
	}


	/**
	 * @brief 두 3x3 행렬을 곱합니다.
	 *
	 * @param m 행렬의 곱셈을 수행할 피연산자입니다.
	 *
	 * @return 두 행렬을 곱한 결과를 반환합니다.
	 */
	Mat3x3 operator*(Mat3x3&& m) const
	{
		return Mat3x3(
			e00 * m.e00 + e01 * m.e10 + e02 * m.e20,
			e00 * m.e01 + e01 * m.e11 + e02 * m.e21,
			e00 * m.e02 + e01 * m.e12 + e02 * m.e22,
			e10 * m.e00 + e11 * m.e10 + e12 * m.e20,
			e10 * m.e01 + e11 * m.e11 + e12 * m.e21,
			e10 * m.e02 + e11 * m.e12 + e12 * m.e22,
			e20 * m.e00 + e21 * m.e10 + e22 * m.e20,
			e20 * m.e01 + e21 * m.e11 + e22 * m.e21,
			e20 * m.e02 + e21 * m.e12 + e22 * m.e22
		);
	}


	/**
	 * @brief 두 3x3 행렬을 곱합니다.
	 *
	 * @param m 행렬의 곱셈을 수행할 피연산자입니다.
	 *
	 * @return 두 행렬을 곱한 결과를 반환합니다.
	 */
	Mat3x3 operator*(const Mat3x3& m) const
	{
		return Mat3x3(
			e00 * m.e00 + e01 * m.e10 + e02 * m.e20,
			e00 * m.e01 + e01 * m.e11 + e02 * m.e21,
			e00 * m.e02 + e01 * m.e12 + e02 * m.e22,
			e10 * m.e00 + e11 * m.e10 + e12 * m.e20,
			e10 * m.e01 + e11 * m.e11 + e12 * m.e21,
			e10 * m.e02 + e11 * m.e12 + e12 * m.e22,
			e20 * m.e00 + e21 * m.e10 + e22 * m.e20,
			e20 * m.e01 + e21 * m.e11 + e22 * m.e21,
			e20 * m.e02 + e21 * m.e12 + e22 * m.e22
		);
	}


	/**
	 * @brief 두 3x3 행렬에 대응하는 원소를 더합니다.
	 *
	 * @param m 연산을 수행할 피연산자입니다.
	 *
	 * @return 연산을 수행한 행렬의 참조자를 반환합니다.
	 */
	Mat3x3& operator+=(Mat3x3&& m) noexcept
	{
		e00 += m.e00; e01 += m.e01; e02 += m.e02;
		e10 += m.e10; e11 += m.e11; e12 += m.e12;
		e20 += m.e20; e21 += m.e21; e22 += m.e22;

		return *this;
	}


	/**
	 * @brief 두 3x3 행렬에 대응하는 원소를 더합니다.
	 *
	 * @param m 연산을 수행할 피연산자입니다.
	 *
	 * @return 연산을 수행한 행렬의 참조자를 반환합니다.
	 */
	Mat3x3& operator+=(const Mat3x3& m) noexcept
	{
		e00 += m.e00; e01 += m.e01; e02 += m.e02;
		e10 += m.e10; e11 += m.e11; e12 += m.e12;
		e20 += m.e20; e21 += m.e21; e22 += m.e22;

		return *this;
	}


	/**
	 * @brief 두 3x3 행렬에 대응하는 원소를 뺍니다.
	 *
	 * @param m 연산을 수행할 피연산자입니다.
	 *
	 * @return 연산을 수행한 행렬의 참조자를 반환합니다.
	 */
	Mat3x3& operator-=(Mat3x3&& m) noexcept
	{
		e00 -= m.e00; e01 -= m.e01; e02 -= m.e02;
		e10 -= m.e10; e11 -= m.e11; e12 -= m.e12;
		e20 -= m.e20; e21 -= m.e21; e22 -= m.e22;

		return *this;
	}


	/**
	 * @brief 두 3x3 행렬에 대응하는 원소를 뺍니다.
	 *
	 * @param m 연산을 수행할 피연산자입니다.
	 *
	 * @return 연산을 수행한 행렬의 참조자를 반환합니다.
	 */
	Mat3x3& operator-=(const Mat3x3& m) noexcept
	{
		e00 -= m.e00; e01 -= m.e01; e02 -= m.e02;
		e10 -= m.e10; e11 -= m.e11; e12 -= m.e12;
		e20 -= m.e20; e21 -= m.e21; e22 -= m.e22;

		return *this;
	}


	/**
	 * @brief 두 3x3행렬이 동일한지 검사합니다.
	 *
	 * @param m 검사를 수행할 피연산자입니다.
	 *
	 * @return 두 행렬이 동일하다면 true, 그렇지 않으면 false를 반환합니다.
	 */
	bool operator==(Mat3x3&& m) const
	{
		return Abs(e00 - m.e00) <= EPSILON
			&& Abs(e01 - m.e01) <= EPSILON
			&& Abs(e02 - m.e02) <= EPSILON
			&& Abs(e10 - m.e10) <= EPSILON
			&& Abs(e11 - m.e11) <= EPSILON
			&& Abs(e12 - m.e12) <= EPSILON
			&& Abs(e20 - m.e20) <= EPSILON
			&& Abs(e21 - m.e21) <= EPSILON
			&& Abs(e22 - m.e22) <= EPSILON;
	}


	/**
	 * @brief 두 3x3행렬이 동일한지 검사합니다.
	 *
	 * @param m 검사를 수행할 피연산자입니다.
	 *
	 * @return 두 행렬이 동일하다면 true, 그렇지 않으면 false를 반환합니다.
	 */
	bool operator==(const Mat3x3& m) const
	{
		return Abs(e00 - m.e00) <= EPSILON
			&& Abs(e01 - m.e01) <= EPSILON
			&& Abs(e02 - m.e02) <= EPSILON
			&& Abs(e10 - m.e10) <= EPSILON
			&& Abs(e11 - m.e11) <= EPSILON
			&& Abs(e12 - m.e12) <= EPSILON
			&& Abs(e20 - m.e20) <= EPSILON
			&& Abs(e21 - m.e21) <= EPSILON
			&& Abs(e22 - m.e22) <= EPSILON;
	}


	/**
	 * @brief 두 3x3행렬이 동일하지 않은지 검사합니다.
	 *
	 * @param m 검사를 수행할 피연산자입니다.
	 *
	 * @return 두 행렬이 동일하지 않다면 true, 그렇다면 false를 반환합니다.
	 */
	bool operator!=(Mat3x3&& m) const
	{
		return Abs(e00 - m.e00) > EPSILON
			|| Abs(e01 - m.e01) > EPSILON
			|| Abs(e02 - m.e02) > EPSILON
			|| Abs(e10 - m.e10) > EPSILON
			|| Abs(e11 - m.e11) > EPSILON
			|| Abs(e12 - m.e12) > EPSILON
			|| Abs(e20 - m.e20) > EPSILON
			|| Abs(e21 - m.e21) > EPSILON
			|| Abs(e22 - m.e22) > EPSILON;
	}


	/**
	 * @brief 두 3x3행렬이 동일하지 않은지 검사합니다.
	 *
	 * @param m 검사를 수행할 피연산자입니다.
	 *
	 * @return 두 행렬이 동일하지 않다면 true, 그렇다면 false를 반환합니다.
	 */
	bool operator!=(const Mat3x3& m) const
	{
		return Abs(e00 - m.e00) > EPSILON
			|| Abs(e01 - m.e01) > EPSILON
			|| Abs(e02 - m.e02) > EPSILON
			|| Abs(e10 - m.e10) > EPSILON
			|| Abs(e11 - m.e11) > EPSILON
			|| Abs(e12 - m.e12) > EPSILON
			|| Abs(e20 - m.e20) > EPSILON
			|| Abs(e21 - m.e21) > EPSILON
			|| Abs(e22 - m.e22) > EPSILON;
	}


	/**
	 * @brief 3x3 행렬 원소 배열의 포인터를 얻습니다.
	 *
	 * @return 3x3 행렬 원소 배열의 포인터를 반환합니다.
	 */
	const float* GetPtr() const { return &data[0]; }


	/**
	 * @brief 3x3 행렬 원소 배열의 포인터를 얻습니다.
	 *
	 * @return 3x3 행렬 원소 배열의 포인터를 반환합니다.
	 */
	float* GetPtr() { return &data[0]; }


	/**
	 * @brief 3x3 행렬의 모든 원소가 0인 행렬을 얻습니다.
	 *
	 * @return 모든 원소가 0인 3x3 행렬을 반환합니다.
	 */
	static inline Mat3x3 Zero()
	{
		return Mat3x3(
			0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f
		);
	}


	/**
	 * @brief 3x3 행렬의 단위 행렬을 얻습니다.
	 *
	 * @return 3x3 행렬의 단위 행렬를 반환합니다.
	 */
	static inline Mat3x3 Identity()
	{
		return Mat3x3(
			1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 1.0f
		);
	}


	/**
	 * @brief 3x3 행렬의 전치 행렬을 얻습니다.
	 *
	 * @param m 원소들을 전치할 3x3 행렬입니다.
	 *
	 * @return 원소가 전치된 3x3 행렬을 반환합니다.
	 */
	static inline Mat3x3 Transpose(const Mat3x3& m)
	{
		return Mat3x3(
			m.e00, m.e10, m.e20,
			m.e01, m.e11, m.e21,
			m.e02, m.e12, m.e22
		);
	}


	/**
	 * @brief 3x3 행렬의 행렬식 값을 얻습니다.
	 *
	 * @param m 행렬식 값을 계산할 3x3 행렬입니다.
	 *
	 * @return 3x3 행렬의 행렬식 값을 반환합니다.
	 */
	static inline float Determinant(const Mat3x3& m)
	{
		return m.e00 * (m.e11 * m.e22 - m.e21 * m.e12) - m.e10 * (m.e01 * m.e22 - m.e21 * m.e02) + m.e20 * (m.e01 * m.e12 - m.e11 * m.e02);
	}


	/**
	 * @brief 3x3 행렬의 역행렬을 얻습니다.
	 *
	 * @param m 역행렬을 계산할 3x3 행렬입니다.
	 *
	 * @return 3x3 행렬의 역행렬을 반환합니다.
	 */
	static inline Mat3x3 Inverse(const Mat3x3& m)
	{
		float oneOverDeterminant = 1.0f / Determinant(m);

		return Mat3x3(
			+(m.e11 * m.e22 - m.e21 * m.e12) * oneOverDeterminant,
			-(m.e10 * m.e22 - m.e20 * m.e12) * oneOverDeterminant,
			+(m.e10 * m.e21 - m.e20 * m.e11) * oneOverDeterminant,
			-(m.e01 * m.e22 - m.e21 * m.e02) * oneOverDeterminant,
			+(m.e00 * m.e22 - m.e20 * m.e02) * oneOverDeterminant,
			-(m.e00 * m.e21 - m.e20 * m.e01) * oneOverDeterminant,
			+(m.e01 * m.e12 - m.e11 * m.e02) * oneOverDeterminant,
			-(m.e00 * m.e12 - m.e10 * m.e02) * oneOverDeterminant,
			+(m.e00 * m.e11 - m.e10 * m.e01) * oneOverDeterminant
		);
	}


	/**
	 * @brief 3x3 행렬의 원소입니다.
	 */
	union
	{
		struct
		{
			float e00; float e01; float e02;
			float e10; float e11; float e12;
			float e20; float e21; float e22;
		};
		float data[9];
	};
};


/**
 * @brief 4x4 행렬입니다.
 */
struct Mat4x4
{
	/**
	 * @brief 4x4 행렬의 기본 생성자입니다.
	 *
	 * @note 모든 원소의 값을 0으로 초기화합니다.
	 */
	Mat4x4() noexcept
	{
		e00 = 0.0f; e01 = 0.0f; e02 = 0.0f; e03 = 0.0f;
		e10 = 0.0f; e11 = 0.0f; e12 = 0.0f; e13 = 0.0f;
		e20 = 0.0f; e21 = 0.0f; e22 = 0.0f; e23 = 0.0f;
		e30 = 0.0f; e31 = 0.0f; e32 = 0.0f; e33 = 0.0f;
	}


	/**
	 * @brief 4x4 행렬의 생성자입니다.
	 *
	 * @param ee00 행렬의 (0, 0) 성분입니다.
	 * @param ee01 행렬의 (0, 1) 성분입니다.
	 * @param ee02 행렬의 (0, 2) 성분입니다.
	 * @param ee03 행렬의 (0, 3) 성분입니다.
	 * @param ee10 행렬의 (1, 0) 성분입니다.
	 * @param ee11 행렬의 (1, 1) 성분입니다.
	 * @param ee12 행렬의 (1, 2) 성분입니다.
	 * @param ee13 행렬의 (1, 3) 성분입니다.
	 * @param ee20 행렬의 (2, 0) 성분입니다.
	 * @param ee21 행렬의 (2, 1) 성분입니다.
	 * @param ee22 행렬의 (2, 2) 성분입니다.
	 * @param ee23 행렬의 (2, 3) 성분입니다.
	 * @param ee30 행렬의 (3, 0) 성분입니다.
	 * @param ee31 행렬의 (3, 1) 성분입니다.
	 * @param ee32 행렬의 (3, 2) 성분입니다.
	 * @param ee33 행렬의 (3, 3) 성분입니다.
	 */
	Mat4x4(
		float&& ee00, float&& ee01, float&& ee02, float&& ee03,
		float&& ee10, float&& ee11, float&& ee12, float&& ee13,
		float&& ee20, float&& ee21, float&& ee22, float&& ee23,
		float&& ee30, float&& ee31, float&& ee32, float&& ee33
	) noexcept
	{
		e00 = ee00; e01 = ee01; e02 = ee02; e03 = ee03;
		e10 = ee10; e11 = ee11; e12 = ee12; e13 = ee13;
		e20 = ee20; e21 = ee21; e22 = ee22; e23 = ee23;
		e30 = ee30; e31 = ee31; e32 = ee32; e33 = ee33;
	}


	/**
	 * @brief 4x4 행렬의 생성자입니다.
	 *
	 * @param ee00 행렬의 (0, 0) 성분입니다.
	 * @param ee01 행렬의 (0, 1) 성분입니다.
	 * @param ee02 행렬의 (0, 2) 성분입니다.
	 * @param ee03 행렬의 (0, 3) 성분입니다.
	 * @param ee10 행렬의 (1, 0) 성분입니다.
	 * @param ee11 행렬의 (1, 1) 성분입니다.
	 * @param ee12 행렬의 (1, 2) 성분입니다.
	 * @param ee13 행렬의 (1, 3) 성분입니다.
	 * @param ee20 행렬의 (2, 0) 성분입니다.
	 * @param ee21 행렬의 (2, 1) 성분입니다.
	 * @param ee22 행렬의 (2, 2) 성분입니다.
	 * @param ee23 행렬의 (2, 3) 성분입니다.
	 * @param ee30 행렬의 (3, 0) 성분입니다.
	 * @param ee31 행렬의 (3, 1) 성분입니다.
	 * @param ee32 행렬의 (3, 2) 성분입니다.
	 * @param ee33 행렬의 (3, 3) 성분입니다.
	 */
	Mat4x4(
		const float& ee00, const float& ee01, const float& ee02, const float& ee03,
		const float& ee10, const float& ee11, const float& ee12, const float& ee13,
		const float& ee20, const float& ee21, const float& ee22, const float& ee23,
		const float& ee30, const float& ee31, const float& ee32, const float& ee33
	) noexcept
	{
		e00 = ee00; e01 = ee01; e02 = ee02; e03 = ee03;
		e10 = ee10; e11 = ee11; e12 = ee12; e13 = ee13;
		e20 = ee20; e21 = ee21; e22 = ee22; e23 = ee23;
		e30 = ee30; e31 = ee31; e32 = ee32; e33 = ee33;
	}


	/**
	 * @brief 4x4 행렬의 원소를 하나의 값으로 초기화합니다.
	 *
	 * @param e 모든 원소를 초기화 할 값입니다.
	 */
	Mat4x4(float&& e) noexcept
	{
		e00 = e; e01 = e; e02 = e; e03 = e;
		e10 = e; e11 = e; e12 = e; e13 = e;
		e20 = e; e21 = e; e22 = e; e23 = e;
		e30 = e; e31 = e; e32 = e; e33 = e;
	}


	/**
	 * @brief 4x4 행렬의 원소를 하나의 값으로 초기화합니다.
	 *
	 * @param e 모든 원소를 초기화 할 값입니다.
	 */
	Mat4x4(const float& e) noexcept
	{
		e00 = e; e01 = e; e02 = e; e03 = e;
		e10 = e; e11 = e; e12 = e; e13 = e;
		e20 = e; e21 = e; e22 = e; e23 = e;
		e30 = e; e31 = e; e32 = e; e33 = e;
	}


	/**
	 * @brief 4x4 행렬의 복사 생성자입니다.
	 *
	 * @param m 원소를 복사할 행렬의 인스턴스입니다.
	 */
	Mat4x4(Mat4x4&& m) noexcept
	{
		e00 = m.e00; e01 = m.e01; e02 = m.e02; e03 = m.e03;
		e10 = m.e10; e11 = m.e11; e12 = m.e12; e13 = m.e13;
		e20 = m.e20; e21 = m.e21; e22 = m.e22; e23 = m.e23;
		e30 = m.e30; e31 = m.e31; e32 = m.e32; e33 = m.e33;
	}


	/**
	 * @brief 4x4 행렬의 복사 생성자입니다.
	 *
	 * @param m 원소를 복사할 행렬의 인스턴스입니다.
	 */
	Mat4x4(const Mat4x4& m) noexcept
	{
		e00 = m.e00; e01 = m.e01; e02 = m.e02; e03 = m.e03;
		e10 = m.e10; e11 = m.e11; e12 = m.e12; e13 = m.e13;
		e20 = m.e20; e21 = m.e21; e22 = m.e22; e23 = m.e23;
		e30 = m.e30; e31 = m.e31; e32 = m.e32; e33 = m.e33;
	}


	/**
	 * @brief 4x4 행렬의 대입 연산자입니다.
	 *
	 * @param m 원소를 복사할 행렬의 인스턴스입니다.
	 *
	 * @return 대입한 행렬의 참조자를 반환합니다.
	 */
	Mat4x4& operator=(Mat4x4&& m) noexcept
	{
		if (this == &m) return *this;

		e00 = m.e00; e01 = m.e01; e02 = m.e02; e03 = m.e03;
		e10 = m.e10; e11 = m.e11; e12 = m.e12; e13 = m.e13;
		e20 = m.e20; e21 = m.e21; e22 = m.e22; e23 = m.e23;
		e30 = m.e30; e31 = m.e31; e32 = m.e32; e33 = m.e33;

		return *this;
	}


	/**
	 * @brief 4x4 행렬의 대입 연산자입니다.
	 *
	 * @param m 원소를 복사할 행렬의 인스턴스입니다.
	 *
	 * @return 대입한 행렬의 참조자를 반환합니다.
	 */
	Mat4x4& operator=(const Mat4x4& m) noexcept
	{
		if (this == &m) return *this;

		e00 = m.e00; e01 = m.e01; e02 = m.e02; e03 = m.e03;
		e10 = m.e10; e11 = m.e11; e12 = m.e12; e13 = m.e13;
		e20 = m.e20; e21 = m.e21; e22 = m.e22; e23 = m.e23;
		e30 = m.e30; e31 = m.e31; e32 = m.e32; e33 = m.e33;

		return *this;
	}


	/**
	 * @brief 4x4 행렬의 모든 원소에 -부호를 취합니다.
	 *
	 * @return 모든 원소에 -부호를 취한 새로운 행렬을 반환합니다.
	 */
	Mat4x4 operator-() const
	{
		return Mat4x4(
			-e00, -e01, -e02, -e03,
			-e10, -e11, -e12, -e13,
			-e20, -e21, -e22, -e23,
			-e30, -e31, -e32, -e33
		);
	}


	/**
	 * @brief 두 4x4 행렬의 대응하는 원소를 더합니다.
	 *
	 * @param m 행렬의 덧셈을 수행할 피연산자입니다.
	 *
	 * @return 두 행렬의 대응하는 원소를 더한 결과를 반환합니다.
	 */
	Mat4x4 operator+(Mat4x4&& m) const
	{
		return Mat4x4(
			e00 + m.e00, e01 + m.e01, e02 + m.e02, e03 + m.e03,
			e10 + m.e10, e11 + m.e11, e12 + m.e12, e13 + m.e13,
			e20 + m.e20, e21 + m.e21, e22 + m.e22, e23 + m.e23,
			e30 + m.e30, e31 + m.e31, e32 + m.e32, e33 + m.e33
		);
	}


	/**
	 * @brief 두 4x4 행렬의 대응하는 원소를 더합니다.
	 *
	 * @param m 행렬의 덧셈을 수행할 피연산자입니다.
	 *
	 * @return 두 행렬의 대응하는 원소를 더한 결과를 반환합니다.
	 */
	Mat4x4 operator+(const Mat4x4& m) const
	{
		return Mat4x4(
			e00 + m.e00, e01 + m.e01, e02 + m.e02, e03 + m.e03,
			e10 + m.e10, e11 + m.e11, e12 + m.e12, e13 + m.e13,
			e20 + m.e20, e21 + m.e21, e22 + m.e22, e23 + m.e23,
			e30 + m.e30, e31 + m.e31, e32 + m.e32, e33 + m.e33
		);
	}


	/**
	 * @brief 두 4x4 행렬의 대응하는 원소를 뺍니다.
	 *
	 * @param m 행렬의 뺄셈을 수행할 피연산자입니다.
	 *
	 * @return 두 행렬의 대응하는 원소를 뺀 결과를 반환합니다.
	 */
	Mat4x4 operator-(Mat4x4&& m) const
	{
		return Mat4x4(
			e00 - m.e00, e01 - m.e01, e02 - m.e02, e03 - m.e03,
			e10 - m.e10, e11 - m.e11, e12 - m.e12, e13 - m.e13,
			e20 - m.e20, e21 - m.e21, e22 - m.e22, e23 - m.e23,
			e30 - m.e30, e31 - m.e31, e32 - m.e32, e33 - m.e33
		);
	}


	/**
	 * @brief 두 4x4 행렬의 대응하는 원소를 뺍니다.
	 *
	 * @param m 행렬의 뺄셈을 수행할 피연산자입니다.
	 *
	 * @return 두 행렬의 대응하는 원소를 뺀 결과를 반환합니다.
	 */
	Mat4x4 operator-(const Mat4x4& m) const
	{
		return Mat4x4(
			e00 - m.e00, e01 - m.e01, e02 - m.e02, e03 - m.e03,
			e10 - m.e10, e11 - m.e11, e12 - m.e12, e13 - m.e13,
			e20 - m.e20, e21 - m.e21, e22 - m.e22, e23 - m.e23,
			e30 - m.e30, e31 - m.e31, e32 - m.e32, e33 - m.e33
		);
	}


	/**
	 * @brief 4x4 행렬에 부동소수점을 곱합니다.
	 *
	 * @param scalar 4x4 행렬에 부동수소점 수를 곱할 스칼라 값입니다.
	 *
	 * @return 4x4 행렬에 부동소수점을 곱합 결과를 반환합니다.
	 */
	Mat4x4 operator*(float&& scalar) const
	{
		return Mat4x4(
			scalar * e00, scalar * e01, scalar * e02, scalar * e03,
			scalar * e10, scalar * e11, scalar * e12, scalar * e13,
			scalar * e20, scalar * e21, scalar * e22, scalar * e23,
			scalar * e30, scalar * e31, scalar * e32, scalar * e33
		);
	}


	/**
	 * @brief 4x4 행렬에 부동소수점을 곱합니다.
	 *
	 * @param scalar 4x4 행렬에 부동수소점 수를 곱할 스칼라 값입니다.
	 *
	 * @return 4x4 행렬에 부동소수점을 곱합 결과를 반환합니다.
	 */
	Mat4x4 operator*(const float& scalar) const
	{
		return Mat4x4(
			scalar * e00, scalar * e01, scalar * e02, scalar * e03,
			scalar * e10, scalar * e11, scalar * e12, scalar * e13,
			scalar * e20, scalar * e21, scalar * e22, scalar * e23,
			scalar * e30, scalar * e31, scalar * e32, scalar * e33
		);
	}


	/**
	 * @brief 두 4x4 행렬을 곱합니다.
	 *
	 * @param m 행렬의 곱셈을 수행할 피연산자입니다.
	 *
	 * @return 두 행렬을 곱한 결과를 반환합니다.
	 */
	Mat4x4 operator*(Mat4x4&& m) const
	{
		return Mat4x4(
			e00 * m.e00 + e01 * m.e10 + e02 * m.e20 + e03 * m.e30,
			e00 * m.e01 + e01 * m.e11 + e02 * m.e21 + e03 * m.e31,
			e00 * m.e02 + e01 * m.e12 + e02 * m.e22 + e03 * m.e32,
			e00 * m.e03 + e01 * m.e13 + e02 * m.e23 + e03 * m.e33,
			e10 * m.e00 + e11 * m.e10 + e12 * m.e20 + e13 * m.e30,
			e10 * m.e01 + e11 * m.e11 + e12 * m.e21 + e13 * m.e31,
			e10 * m.e02 + e11 * m.e12 + e12 * m.e22 + e13 * m.e32,
			e10 * m.e03 + e11 * m.e13 + e12 * m.e23 + e13 * m.e33,
			e20 * m.e00 + e21 * m.e10 + e22 * m.e20 + e23 * m.e30,
			e20 * m.e01 + e21 * m.e11 + e22 * m.e21 + e23 * m.e31,
			e20 * m.e02 + e21 * m.e12 + e22 * m.e22 + e23 * m.e32,
			e20 * m.e03 + e21 * m.e13 + e22 * m.e23 + e23 * m.e33,
			e30 * m.e00 + e31 * m.e10 + e32 * m.e20 + e33 * m.e30,
			e30 * m.e01 + e31 * m.e11 + e32 * m.e21 + e33 * m.e31,
			e30 * m.e02 + e31 * m.e12 + e32 * m.e22 + e33 * m.e32,
			e30 * m.e03 + e31 * m.e13 + e32 * m.e23 + e33 * m.e33
		);
	}


	/**
	 * @brief 두 4x4 행렬을 곱합니다.
	 *
	 * @param m 행렬의 곱셈을 수행할 피연산자입니다.
	 *
	 * @return 두 행렬을 곱한 결과를 반환합니다.
	 */
	Mat4x4 operator*(const Mat4x4& m) const
	{
		return Mat4x4(
			e00 * m.e00 + e01 * m.e10 + e02 * m.e20 + e03 * m.e30,
			e00 * m.e01 + e01 * m.e11 + e02 * m.e21 + e03 * m.e31,
			e00 * m.e02 + e01 * m.e12 + e02 * m.e22 + e03 * m.e32,
			e00 * m.e03 + e01 * m.e13 + e02 * m.e23 + e03 * m.e33,
			e10 * m.e00 + e11 * m.e10 + e12 * m.e20 + e13 * m.e30,
			e10 * m.e01 + e11 * m.e11 + e12 * m.e21 + e13 * m.e31,
			e10 * m.e02 + e11 * m.e12 + e12 * m.e22 + e13 * m.e32,
			e10 * m.e03 + e11 * m.e13 + e12 * m.e23 + e13 * m.e33,
			e20 * m.e00 + e21 * m.e10 + e22 * m.e20 + e23 * m.e30,
			e20 * m.e01 + e21 * m.e11 + e22 * m.e21 + e23 * m.e31,
			e20 * m.e02 + e21 * m.e12 + e22 * m.e22 + e23 * m.e32,
			e20 * m.e03 + e21 * m.e13 + e22 * m.e23 + e23 * m.e33,
			e30 * m.e00 + e31 * m.e10 + e32 * m.e20 + e33 * m.e30,
			e30 * m.e01 + e31 * m.e11 + e32 * m.e21 + e33 * m.e31,
			e30 * m.e02 + e31 * m.e12 + e32 * m.e22 + e33 * m.e32,
			e30 * m.e03 + e31 * m.e13 + e32 * m.e23 + e33 * m.e33
		);
	}


	/**
	 * @brief 두 4x4 행렬에 대응하는 원소를 더합니다.
	 *
	 * @param m 연산을 수행할 피연산자입니다.
	 *
	 * @return 연산을 수행한 행렬의 참조자를 반환합니다.
	 */
	Mat4x4& operator+=(Mat4x4&& m) noexcept
	{
		e00 += m.e00; e01 += m.e01; e02 += m.e02; e03 += m.e03;
		e10 += m.e10; e11 += m.e11; e12 += m.e12; e13 += m.e13;
		e20 += m.e20; e21 += m.e21; e22 += m.e22; e23 += m.e23;
		e30 += m.e30; e31 += m.e31; e32 += m.e32; e33 += m.e33;

		return *this;
	}


	/**
	 * @brief 두 4x4 행렬에 대응하는 원소를 더합니다.
	 *
	 * @param m 연산을 수행할 피연산자입니다.
	 *
	 * @return 연산을 수행한 행렬의 참조자를 반환합니다.
	 */
	Mat4x4& operator+=(const Mat4x4& m) noexcept
	{
		e00 += m.e00; e01 += m.e01; e02 += m.e02; e03 += m.e03;
		e10 += m.e10; e11 += m.e11; e12 += m.e12; e13 += m.e13;
		e20 += m.e20; e21 += m.e21; e22 += m.e22; e23 += m.e23;
		e30 += m.e30; e31 += m.e31; e32 += m.e32; e33 += m.e33;

		return *this;
	}


	/**
	 * @brief 두 4x4 행렬에 대응하는 원소를 뺍니다.
	 *
	 * @param m 연산을 수행할 피연산자입니다.
	 *
	 * @return 연산을 수행한 행렬의 참조자를 반환합니다.
	 */
	Mat4x4& operator-=(Mat4x4&& m) noexcept
	{
		e00 -= m.e00; e01 -= m.e01; e02 -= m.e02; e03 -= m.e03;
		e10 -= m.e10; e11 -= m.e11; e12 -= m.e12; e13 -= m.e13;
		e20 -= m.e20; e21 -= m.e21; e22 -= m.e22; e23 -= m.e23;
		e30 -= m.e30; e31 -= m.e31; e32 -= m.e32; e33 -= m.e33;

		return *this;
	}


	/**
	 * @brief 두 4x4 행렬에 대응하는 원소를 뺍니다.
	 *
	 * @param m 연산을 수행할 피연산자입니다.
	 *
	 * @return 연산을 수행한 행렬의 참조자를 반환합니다.
	 */
	Mat4x4& operator-=(const Mat4x4& m) noexcept
	{
		e00 -= m.e00; e01 -= m.e01; e02 -= m.e02; e03 -= m.e03;
		e10 -= m.e10; e11 -= m.e11; e12 -= m.e12; e13 -= m.e13;
		e20 -= m.e20; e21 -= m.e21; e22 -= m.e22; e23 -= m.e23;
		e30 -= m.e30; e31 -= m.e31; e32 -= m.e32; e33 -= m.e33;

		return *this;
	}


	/**
	 * @brief 두 4x4행렬이 동일한지 검사합니다.
	 *
	 * @param m 검사를 수행할 피연산자입니다.
	 *
	 * @return 두 행렬이 동일하다면 true, 그렇지 않으면 false를 반환합니다.
	 */
	bool operator==(Mat4x4&& m) const
	{
		return Abs(e00 - m.e00) <= EPSILON
			&& Abs(e01 - m.e01) <= EPSILON
			&& Abs(e02 - m.e02) <= EPSILON
			&& Abs(e03 - m.e03) <= EPSILON
			&& Abs(e10 - m.e10) <= EPSILON
			&& Abs(e11 - m.e11) <= EPSILON
			&& Abs(e12 - m.e12) <= EPSILON
			&& Abs(e13 - m.e13) <= EPSILON
			&& Abs(e20 - m.e20) <= EPSILON
			&& Abs(e21 - m.e21) <= EPSILON
			&& Abs(e22 - m.e22) <= EPSILON
			&& Abs(e23 - m.e23) <= EPSILON
			&& Abs(e30 - m.e30) <= EPSILON
			&& Abs(e31 - m.e31) <= EPSILON
			&& Abs(e32 - m.e32) <= EPSILON
			&& Abs(e33 - m.e33) <= EPSILON;
	}


	/**
	 * @brief 두 4x4행렬이 동일한지 검사합니다.
	 *
	 * @param m 검사를 수행할 피연산자입니다.
	 *
	 * @return 두 행렬이 동일하다면 true, 그렇지 않으면 false를 반환합니다.
	 */
	bool operator==(const Mat4x4& m) const
	{
		return Abs(e00 - m.e00) <= EPSILON
			&& Abs(e01 - m.e01) <= EPSILON
			&& Abs(e02 - m.e02) <= EPSILON
			&& Abs(e03 - m.e03) <= EPSILON
			&& Abs(e10 - m.e10) <= EPSILON
			&& Abs(e11 - m.e11) <= EPSILON
			&& Abs(e12 - m.e12) <= EPSILON
			&& Abs(e13 - m.e13) <= EPSILON
			&& Abs(e20 - m.e20) <= EPSILON
			&& Abs(e21 - m.e21) <= EPSILON
			&& Abs(e22 - m.e22) <= EPSILON
			&& Abs(e23 - m.e23) <= EPSILON
			&& Abs(e30 - m.e30) <= EPSILON
			&& Abs(e31 - m.e31) <= EPSILON
			&& Abs(e32 - m.e32) <= EPSILON
			&& Abs(e33 - m.e33) <= EPSILON;
	}


	/**
	 * @brief 두 4x4행렬이 동일하지 않은지 검사합니다.
	 *
	 * @param m 검사를 수행할 피연산자입니다.
	 *
	 * @return 두 행렬이 동일하지 않다면 true, 그렇다면 false를 반환합니다.
	 */
	bool operator!=(Mat4x4&& m) const
	{
		return Abs(e00 - m.e00) > EPSILON
			|| Abs(e01 - m.e01) > EPSILON
			|| Abs(e02 - m.e02) > EPSILON
			|| Abs(e03 - m.e03) > EPSILON
			|| Abs(e10 - m.e10) > EPSILON
			|| Abs(e11 - m.e11) > EPSILON
			|| Abs(e12 - m.e12) > EPSILON
			|| Abs(e13 - m.e13) > EPSILON
			|| Abs(e20 - m.e20) > EPSILON
			|| Abs(e21 - m.e21) > EPSILON
			|| Abs(e22 - m.e22) > EPSILON
			|| Abs(e23 - m.e23) > EPSILON
			|| Abs(e30 - m.e30) > EPSILON
			|| Abs(e31 - m.e31) > EPSILON
			|| Abs(e32 - m.e32) > EPSILON
			|| Abs(e33 - m.e33) > EPSILON;
	}


	/**
	 * @brief 두 4x4행렬이 동일하지 않은지 검사합니다.
	 *
	 * @param m 검사를 수행할 피연산자입니다.
	 *
	 * @return 두 행렬이 동일하지 않다면 true, 그렇다면 false를 반환합니다.
	 */
	bool operator!=(const Mat4x4& m) const
	{
		return Abs(e00 - m.e00) > EPSILON
			|| Abs(e01 - m.e01) > EPSILON
			|| Abs(e02 - m.e02) > EPSILON
			|| Abs(e03 - m.e03) > EPSILON
			|| Abs(e10 - m.e10) > EPSILON
			|| Abs(e11 - m.e11) > EPSILON
			|| Abs(e12 - m.e12) > EPSILON
			|| Abs(e13 - m.e13) > EPSILON
			|| Abs(e20 - m.e20) > EPSILON
			|| Abs(e21 - m.e21) > EPSILON
			|| Abs(e22 - m.e22) > EPSILON
			|| Abs(e23 - m.e23) > EPSILON
			|| Abs(e30 - m.e30) > EPSILON
			|| Abs(e31 - m.e31) > EPSILON
			|| Abs(e32 - m.e32) > EPSILON
			|| Abs(e33 - m.e33) > EPSILON;
	}


	/**
	 * @brief 4x4 행렬 원소 배열의 포인터를 얻습니다.
	 *
	 * @return 4x4 행렬 원소 배열의 포인터를 반환합니다.
	 */
	const float* GetPtr() const { return &e00; }


	/**
	 * @brief 4x4 행렬 원소 배열의 포인터를 얻습니다.
	 *
	 * @return 4x4 행렬 원소 배열의 포인터를 반환합니다.
	 */
	float* GetPtr() { return &e00; }


	/**
	 * @brief 4x4 행렬의 모든 원소가 0인 행렬을 얻습니다.
	 *
	 * @return 모든 원소가 0인 4x4 행렬을 반환합니다.
	 */
	static inline Mat4x4 Zero()
	{
		return Mat4x4(
			0.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f
		);
	}


	/**
	 * @brief 4x4 행렬의 단위 행렬을 얻습니다.
	 *
	 * @return 4x4 행렬의 단위 행렬를 반환합니다.
	 */
	static inline Mat4x4 Identity()
	{
		return Mat4x4(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
	}


	/**
	 * @brief 4x4 행렬의 전치 행렬을 얻습니다.
	 *
	 * @param m 원소들을 전치할 4x4 행렬입니다.
	 *
	 * @return 원소가 전치된 4x4 행렬을 반환합니다.
	 */
	static inline Mat4x4 Transpose(const Mat4x4& m)
	{
		return Mat4x4(
			m.e00, m.e10, m.e20, m.e30,
			m.e01, m.e11, m.e21, m.e31,
			m.e02, m.e12, m.e22, m.e32,
			m.e03, m.e13, m.e23, m.e33
		);
	}


	/**
	 * @brief 4x4 행렬의 행렬식 값을 얻습니다.
	 *
	 * @param m 행렬식 값을 계산할 4x4 행렬입니다.
	 *
	 * @return 4x4 행렬의 행렬식 값을 반환합니다.
	 */
	static float inline Determinant(const Mat4x4& m)
	{
		float subFactor00 = m.e22 * m.e33 - m.e32 * m.e23;
		float subFactor01 = m.e21 * m.e33 - m.e31 * m.e23;
		float subFactor02 = m.e21 * m.e32 - m.e31 * m.e22;
		float subFactor03 = m.e20 * m.e33 - m.e30 * m.e23;
		float subFactor04 = m.e20 * m.e32 - m.e30 * m.e22;
		float subFactor05 = m.e20 * m.e31 - m.e30 * m.e21;

		float cof0 = +(m.e11 * subFactor00 - m.e12 * subFactor01 + m.e13 * subFactor02);
		float cof1 = -(m.e10 * subFactor00 - m.e12 * subFactor03 + m.e13 * subFactor04);
		float cof2 = +(m.e10 * subFactor01 - m.e11 * subFactor03 + m.e13 * subFactor05);
		float cof3 = -(m.e10 * subFactor02 - m.e11 * subFactor04 + m.e12 * subFactor05);

		return m.e00 * cof0 + m.e01 * cof1 + m.e02 * cof2 + m.e03 * cof3;
	}


	/**
	 * @brief 4x4 행렬의 역행렬을 얻습니다.
	 *
	 * @param m 역행렬을 계산할 4x4 행렬입니다.
	 *
	 * @return 4x4 행렬의 역행렬을 반환합니다.
	 */
	static inline Mat4x4 Inverse(const Mat4x4& m)
	{
		float oneOverDeterminant = 1.0f / Determinant(m);

		float coef00 = m.e22 * m.e33 - m.e32 * m.e23;
		float coef02 = m.e12 * m.e33 - m.e32 * m.e13;
		float coef03 = m.e12 * m.e23 - m.e22 * m.e13;

		float coef04 = m.e21 * m.e33 - m.e31 * m.e23;
		float coef06 = m.e11 * m.e33 - m.e31 * m.e13;
		float coef07 = m.e11 * m.e23 - m.e21 * m.e13;

		float coef08 = m.e21 * m.e32 - m.e31 * m.e22;
		float coef10 = m.e11 * m.e32 - m.e31 * m.e12;
		float coef11 = m.e11 * m.e22 - m.e21 * m.e12;

		float coef12 = m.e20 * m.e33 - m.e30 * m.e23;
		float coef14 = m.e10 * m.e33 - m.e30 * m.e13;
		float coef15 = m.e10 * m.e23 - m.e20 * m.e13;

		float coef16 = m.e20 * m.e32 - m.e30 * m.e22;
		float coef18 = m.e10 * m.e32 - m.e30 * m.e12;
		float coef19 = m.e10 * m.e22 - m.e20 * m.e12;

		float coef20 = m.e20 * m.e31 - m.e30 * m.e21;
		float coef22 = m.e10 * m.e31 - m.e30 * m.e11;
		float coef23 = m.e10 * m.e21 - m.e20 * m.e11;

		Vec4f fac0(coef00, coef00, coef02, coef03);
		Vec4f fac1(coef04, coef04, coef06, coef07);
		Vec4f fac2(coef08, coef08, coef10, coef11);
		Vec4f fac3(coef12, coef12, coef14, coef15);
		Vec4f fac4(coef16, coef16, coef18, coef19);
		Vec4f fac5(coef20, coef20, coef22, coef23);

		Vec4f vec0(m.e10, m.e00, m.e00, m.e00);
		Vec4f vec1(m.e11, m.e01, m.e01, m.e01);
		Vec4f vec2(m.e12, m.e02, m.e02, m.e02);
		Vec4f vec3(m.e13, m.e03, m.e03, m.e03);

		Vec4f inv0(vec1 * fac0 - vec2 * fac1 + vec3 * fac2);
		Vec4f inv1(vec0 * fac0 - vec2 * fac3 + vec3 * fac4);
		Vec4f inv2(vec0 * fac1 - vec1 * fac3 + vec3 * fac5);
		Vec4f inv3(vec0 * fac2 - vec1 * fac4 + vec2 * fac5);

		Vec4f signA(+1.0f, -1.0f, +1.0f, -1.0f);
		Vec4f signB(-1.0f, +1.0f, -1.0f, +1.0f);

		Vec4f col0 = (inv0 * signA) * oneOverDeterminant;
		Vec4f col1 = (inv1 * signB) * oneOverDeterminant;
		Vec4f col2 = (inv2 * signA) * oneOverDeterminant;
		Vec4f col3 = (inv3 * signB) * oneOverDeterminant;

		return Mat4x4(
			col0.x, col0.y, col0.z, col0.w,
			col1.x, col1.y, col1.z, col1.w,
			col2.x, col2.y, col2.z, col2.w,
			col3.x, col3.y, col3.z, col3.w
		);
	}


	/**
	 * @brief 이동 변환 행렬을 생성합니다.
	 *
	 * @param x 이동할 x 좌표값입니다.
	 * @param y 이동할 y 좌표값입니다.
	 * @param z 이동할 z 좌표값입니다.
	 *
	 * @return 생성된 이동 변환 행렬을 반환합니다.
	 */
	static inline Mat4x4 Translation(float x, float y, float z)
	{
		return Mat4x4(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			x, y, z, 1.0f
		);
	}


	/**
	 * @brief 이동 변환 행렬을 생성합니다.
	 *
	 * @param p 이동할 위치 좌표입니다.
	 *
	 * @return 생성된 이동 변환 행렬을 반환합니다.
	 */
	static inline Mat4x4 Translation(const Vec3f& p)
	{
		return Mat4x4(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			p.x, p.y, p.z, 1.0f
		);
	}


	/**
	 * @brief 스케일 변환 행렬을 생성합니다.
	 *
	 * @param xScale 변환할 x축 방향의 스케일입니다.
	 * @param yScale 변환할 y축 방향의 스케일입니다.
	 * @param zScale 변환할 z축 방향의 스케일입니다.
	 *
	 * @return 생성된 스케일 변환 행렬을 반환합니다.
	 */
	static inline Mat4x4 Scale(float xScale, float yScale, float zScale)
	{
		return Mat4x4(
			xScale, 0.0f, 0.0f, 0.0f,
			0.0f, yScale, 0.0f, 0.0f,
			0.0f, 0.0f, zScale, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
	}


	/**
	 * @brief 스케일 변환 행렬을 생성합니다.
	 *
	 * @param scale 변환할 스케일 벡터입니다.
	 *
	 * @return 생성된 스케일 변환 행렬을 반환합니다.
	 */
	static inline Mat4x4 Scale(const Vec3f& scale)
	{
		return Mat4x4(
			scale.x, 0.0f, 0.0f, 0.0f,
			0.0f, scale.y, 0.0f, 0.0f,
			0.0f, 0.0f, scale.z, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
	}


	/**
	 * @brief X축으로 회전시키는 회전 행렬을 생성합니다.
	 *
	 * @param radian 회전할 라디안 각도입니다.
	 *
	 * @return 생성된 회전 행렬을 반환합니다.
	 */
	static inline Mat4x4 RotateX(float radian)
	{
		float c = Cos(radian);
		float s = Sin(radian);

		return Mat4x4(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, c, s, 0.0f,
			0.0f, -s, c, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
	}


	/**
	 * @brief Y축으로 회전시키는 회전 행렬을 생성합니다.
	 *
	 * @param radian 회전할 라디안 각도입니다.
	 *
	 * @return 생성된 회전 행렬을 반환합니다.
	 */
	static inline Mat4x4 RotateY(float radian)
	{
		float c = Cos(radian);
		float s = Sin(radian);

		return Mat4x4(
			c, 0.0f, -s, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			s, 0.0f, c, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
	}


	/**
	 * @brief Z축으로 회전시키는 회전 행렬을 생성합니다.
	 *
	 * @param radian 회전할 라디안 각도입니다.
	 *
	 * @return 생성된 회전 행렬을 반환합니다.
	 */
	static inline Mat4x4 RotateZ(float radian)
	{
		float c = Cos(radian);
		float s = Sin(radian);

		return Mat4x4(
			c, s, 0.0f, 0.0f,
			-s, c, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
	}


	/**
	 * @brief 임의의 벡터를 기준으로 회전 행렬을 생성합니다.
	 *
	 * @param radian 회전할 회전 각도입니다.
	 * @param axis 회전축입니다.
	 *
	 * @return 생성된 회전 행렬을 반환합니다.
	 *
	 * @see
	 * - 로드리게스 회전 공식 참조
	 * - https://en.wikipedia.org/wiki/Rodrigues%27_rotation_formula
	 */
	static inline Mat4x4 Rotate(float radian, const Vec3f& axis)
	{
		float c = Cos(radian);
		float s = Sin(radian);
		Vec3f r = Vec3f::Normalize(axis);

		return Mat4x4(
			c + r.x * r.x * (1.0f - c), r.y * r.x * (1.0f - c) + r.z * s, r.z * r.x * (1.0f - c) - r.y * s, 0.0f,
			r.x * r.y * (1.0f - c) - r.z * s, c + r.y * r.y * (1.0f - c), r.z * r.y * (1.0f - c) + r.x * s, 0.0f,
			r.x * r.z * (1.0f - c) + r.y * s, r.y * r.z * (1.0f - c) - r.x * s, c + r.z * r.z * (1.0f - c), 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
	}


	/**
	 * @brief 직교 투영 행렬을 생성합니다.
	 *
	 * @param left   투영 공간의 좌측 경계 평면 값
	 * @param right  투영 공간의 우측 경계 평면 값
	 * @param bottom 투영 공간의 하단 경계 평면 값
	 * @param top    투영 공간의 상단 경계 평면 값
	 * @param zNear  투영 공간의 전방 경계 평면 값
	 * @param zFar   투영 공간의 후방 경계 평면 값
	 *
	 * @return 생성된 직교 투영 행렬을 반환합니다.
	 */
	static inline Mat4x4 Ortho(float left, float right, float bottom, float top, float zNear, float zFar)
	{
		float width = (right - left);
		float sumLR = (right + left);
		float height = (top - bottom);
		float sumTB = (top + bottom);
		float depth = (zFar - zNear);
		float sumNF = (zFar + zNear);

		return Mat4x4(
			2.0f / width, 0.0f, 0.0f, 0.0f,
			0.0f, 2.0f / height, 0.0f, 0.0f,
			0.0f, 0.0f, -2.0f / depth, 0.0f,
			-sumLR / width, -sumTB / height, -sumNF / depth, 1.0f
		);
	}


	/**
	 * @brief 원근 투영 행렬을 생성합니다.
	 *
	 * @param fov 라디안 단위의 시야 각도입니다.
	 * @param aspect 뷰 공간의 가로/세로 비율입니다.
	 * @param nearZ 가까운 클리핑 평면 사이의 거리입니다. 0보다 커야 합니다.
	 * @param farZ 원거리 클리핑 평면 사이의 거리입니다. 0보다 커야 합니다.
	 *
	 * @return 생성된 원근 투영 행렬을 반환합니다.
	 */
	static inline Mat4x4 Perspective(float fov, float aspect, float nearZ, float farZ)
	{
		float halfFov = fov / 2.0f;
		float tanHalfFovy = Sin(halfFov) / Cos(halfFov);

		return Mat4x4(
			1.0f / (aspect * tanHalfFovy), 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f / (tanHalfFovy), 0.0f, 0.0f,
			0.0f, 0.0f, -(farZ + nearZ) / (farZ - nearZ), -1.0f,
			0.0f, 0.0f, -(2.0f * farZ * nearZ) / (farZ - nearZ), 0.0f
		);
	}


	/**
	 * @brief 시야 행렬을 생성합니다.
	 *
	 * @param eyePosition 카메라의 위치입니다.
	 * @param focusPosition 초점의 위치입니다.
	 * @param upDirection 카메라의 위쪽 방향입니다. 일반적으로 <0.0f, 1.0f, 0.0f> 입니다.
	 *
	 * @return 생성된 시야 행렬을 반환합니다.
	 */
	static inline Mat4x4 LookAt(const Vec3f& eyePosition, const Vec3f& focusPosition, const Vec3f& upDirection)
	{
		Vec3f f = -Vec3f::Normalize(focusPosition - eyePosition);
		Vec3f s = Vec3f::Normalize(Vec3f::Cross(upDirection, f));
		Vec3f u = Vec3f::Cross(f, s);
		Vec3f t = Vec3f(-Vec3f::Dot(s, eyePosition), -Vec3f::Dot(u, eyePosition), -Vec3f::Dot(f, eyePosition));

		return Mat4x4(
			s.x, u.x, f.x, 0.0f,
			s.y, u.y, f.y, 0.0f,
			s.z, u.z, f.z, 0.0f,
			t.x, t.y, t.z, 1.0f
		);
	}


	/**
	 * @brief 4x4 행렬의 원소입니다.
	 */
	union
	{
		struct
		{
			float e00; float e01; float e02; float e03;
			float e10; float e11; float e12; float e13;
			float e20; float e21; float e22; float e23;
			float e30; float e31; float e32; float e33;
		};
		float data[16];
	};
};


/**
 * @brief 2차원 벡터와 2x2행렬을 곱합니다.
 *
 * @param v 곱셈을 수행할 2차원 벡터입니다.
 * @param m 곱셈을 수행할 2x2 행렬입니다.
 */
static inline Vec2f operator*(const Vec2f& v, const Mat2x2& m)
{
	return Vec2f(
		v.x * m.e00 + v.y * m.e10,
		v.x * m.e01 + v.y * m.e11
	);
}


/**
 * @brief 2차원 벡터와 2x2행렬을 곱합니다.
 *
 * @param m 곱셈을 수행할 2x2 행렬입니다.
 * @param v 곱셈을 수행할 2차원 벡터입니다.
 */
static inline Vec2f operator*(const Mat2x2& m, const Vec2f& v)
{
	return Vec2f(
		m.e00 * v.x + m.e01 * v.y,
		m.e10 * v.x + m.e11 * v.y
	);
}


/**
 * @brief 3차원 벡터와 3x3행렬을 곱합니다.
 *
 * @param v 곱셈을 수행할 3차원 벡터입니다.
 * @param m 곱셈을 수행할 3x3 행렬입니다.
 */
static inline Vec3f operator*(const Vec3f& v, const Mat3x3& m)
{
	return Vec3f(
		v.x * m.e00 + v.y * m.e10 + v.z * m.e20,
		v.x * m.e01 + v.y * m.e11 + v.z * m.e21,
		v.x * m.e02 + v.y * m.e12 + v.z * m.e22
	);
}


/**
 * @brief 3차원 벡터와 3x3행렬을 곱합니다.
 *
 * @param m 곱셈을 수행할 3x3 행렬입니다.
 * @param v 곱셈을 수행할 3차원 벡터입니다.
 */
static inline Vec3f operator*(const Mat3x3& m, const Vec3f& v)
{
	return Vec3f(
		m.e00 * v.x + m.e01 * v.y + m.e02 * v.z,
		m.e10 * v.x + m.e11 * v.y + m.e12 * v.z,
		m.e20 * v.x + m.e21 * v.y + m.e22 * v.z
	);
}


/**
 * @brief 4차원 벡터와 4x4행렬을 곱합니다.
 *
 * @param v 곱셈을 수행할 4차원 벡터입니다.
 * @param m 곱셈을 수행할 4x4 행렬입니다.
 */
static inline Vec4f operator*(const Vec4f& v, const Mat4x4& m)
{
	return Vec4f(
		v.x * m.e00 + v.y * m.e10 + v.z * m.e20 + v.w * m.e30,
		v.x * m.e01 + v.y * m.e11 + v.z * m.e21 + v.w * m.e31,
		v.x * m.e02 + v.y * m.e12 + v.z * m.e22 + v.w * m.e32,
		v.x * m.e03 + v.y * m.e13 + v.z * m.e23 + v.w * m.e33
	);
}


/**
 * @brief 4차원 벡터와 4x4행렬을 곱합니다.
 *
 * @param m 곱셈을 수행할 4x4 행렬입니다.
 * @param v 곱셈을 수행할 4차원 벡터입니다.
 */
static inline Vec4f operator*(const Mat4x4& m, const Vec4f& v)
{
	return Vec4f(
		m.e00 * v.x + m.e01 * v.y + m.e02 * v.z + m.e03 * v.w,
		m.e10 * v.x + m.e11 * v.y + m.e12 * v.z + m.e13 * v.w,
		m.e20 * v.x + m.e21 * v.y + m.e22 * v.z + m.e23 * v.w,
		m.e30 * v.x + m.e31 * v.y + m.e32 * v.z + m.e33 * v.w
	);
}


/**
 * @brief 쿼터니언입니다.
 *
 * @see
 * - https://gabormakesgames.com/quaternions.html
 * - https://en.wikipedia.org/wiki/Quaternion
 * - https://github.com/mmp/pbrt-v3/blob/master/src/core/quaternion.h
 */
struct Quat
{
	/**
	 * @brief 쿼터니언의 기본 생성자입니다.
	 *
	 * @note 모든 원소의 값을 0으로 초기화합니다.
	 */
	Quat() noexcept : x(0.0f), y(0.0f), z(0.0f), w(1.0f) {}


	/**
	 * @brief 쿼터니언의 생성자입니다.
	 *
	 * @param xx 쿼터니언의 x 성분입니다.
	 * @param yy 쿼터니언의 y 성분입니다.
	 * @param zz 쿼터니언의 z 성분입니다.
	 * @param ww 쿼터니언의 w 성분입니다.
	 */
	Quat(float&& xx, float&& yy, float&& zz, float&& ww) noexcept : x(xx), y(yy), z(zz), w(ww) {}


	/**
	 * @brief 쿼터니언의 생성자입니다.
	 *
	 * @param xx 쿼터니언의 x 성분입니다.
	 * @param yy 쿼터니언의 y 성분입니다.
	 * @param zz 쿼터니언의 z 성분입니다.
	 * @param ww 쿼터니언의 w 성분입니다.
	 */
	Quat(const float& xx, const float& yy, const float& zz, const float& ww) noexcept : x(xx), y(yy), z(zz), w(ww) {}


	/**
	 * @brief 쿼터니언의 복사 생성자입니다.
	 *
	 * @param q 원소를 복사할 쿼터니언 구조체의 인스턴스입니다.
	 */
	Quat(Quat&& q) noexcept : x(q.x), y(q.y), z(q.z), w(q.w) {}


	/**
	 * @brief 쿼터니언의 복사 생성자입니다.
	 *
	 * @param q 원소를 복사할 쿼터니언 구조체의 인스턴스입니다.
	 */
	Quat(const Quat& q) noexcept : x(q.x), y(q.y), z(q.z), w(q.w) {}


	/**
	 * @brief 쿼터니언의 대입 연산자입니다.
	 *
	 * @param q 원소를 복사할 쿼터니언 구조체의 인스턴스입니다.
	 *
	 * @return 대입한 쿼터니언의 참조자를 반환합니다.
	 */
	Quat& operator=(Quat&& q) noexcept
	{
		if (this == &q) return *this;

		x = q.x;
		y = q.y;
		z = q.z;
		w = q.w;

		return *this;
	}


	/**
	 * @brief 쿼터니언의 대입 연산자입니다.
	 *
	 * @param q 원소를 복사할 쿼터니언 구조체의 인스턴스입니다.
	 *
	 * @return 대입한 쿼터니언의 참조자를 반환합니다.
	 */
	Quat& operator=(const Quat& q) noexcept
	{
		if (this == &q) return *this;

		x = q.x;
		y = q.y;
		z = q.z;
		w = q.w;

		return *this;
	}


	/**
	 * @brief 쿼터니언에 -부호를 취합니다.
	 *
	 * @return 쿼터니언에 -부호를 취한 새로운 벡터를 반환합니다.
	 */
	Quat operator-() const
	{
		return Quat(-x, -y, -z, -w);
	}


	/**
	 * @brief 두 쿼터니언에 대응하는 원소를 더합니다.
	 *
	 * @param q 쿼터니언의 덧셈을 수행할 피연산자입니다.
	 *
	 * @return 두 쿼터니언에 대응하는 원소를 더한 결과를 반환합니다.
	 */
	Quat operator+(Quat&& q) const
	{
		return Quat(x + q.x, y + q.y, z + q.z, w + q.w);
	}


	/**
	 * @brief 두 쿼터니언에 대응하는 원소를 더합니다.
	 *
	 * @param q 쿼터니언의 덧셈을 수행할 피연산자입니다.
	 *
	 * @return 두 쿼터니언에 대응하는 원소를 더한 결과를 반환합니다.
	 */
	Quat operator+(const Quat& q) const
	{
		return Quat(x + q.x, y + q.y, z + q.z, w + q.w);
	}


	/**
	 * @brief 두 쿼터니언에 대응하는 원소를 뺍니다.
	 *
	 * @param q 쿼터니언의 뺄셈을 수행할 피연산자입니다.
	 *
	 * @return 두 쿼터니언에 대응하는 원소를 뺀 결과를 반환합니다.
	 */
	Quat operator-(Quat&& q) const
	{
		return Quat(x - q.x, y - q.y, z - q.z, w - q.w);
	}


	/**
	 * @brief 두 쿼터니언에 대응하는 원소를 뺍니다.
	 *
	 * @param q 쿼터니언의 뺄셈을 수행할 피연산자입니다.
	 *
	 * @return 두 쿼터니언에 대응하는 원소를 뺀 결과를 반환합니다.
	 */
	Quat operator-(const Quat& q) const
	{
		return Quat(x - q.x, y - q.y, z - q.z, w - q.w);
	}


	/**
	 * @brief 쿼터니언의 스케일 연산을 수행합니다.
	 *
	 * @param s 스케일 연산을 수행할 값입니다.
	 *
	 * @return 스케일 연산을 수행한 결과를 반환합니다.
	 */
	Quat operator*(float s) const
	{
		return Quat(x * s, y * s, z * s, w * s);
	}


	/**
	 * @brief 쿼터니언 곱셈 연산을 수행합니다.
	 *
	 * @param q 곱셈 연산을 수행할 쿼터니언 값입니다.
	 *
	 * @return 곱셈 연산을 수행한 결과를 반환합니다.
	 */
	Quat operator*(Quat&& q) const
	{
		return Quat(
			+q.x * w + q.y * z - q.z * y + q.w * x,
			-q.x * z + q.y * w + q.z * x + q.w * y,
			+q.x * y - q.y * x + q.z * w + q.w * z,
			-q.x * x - q.y * y - q.z * z + q.w * w
		);
	}


	/**
	 * @brief 쿼터니언 곱셈 연산을 수행합니다.
	 *
	 * @param q 곱셈 연산을 수행할 쿼터니언 값입니다.
	 *
	 * @return 곱셈 연산을 수행한 결과를 반환합니다.
	 */
	Quat operator*(const Quat& q) const
	{
		return Quat(
			+q.x * w + q.y * z - q.z * y + q.w * x,
			-q.x * z + q.y * w + q.z * x + q.w * y,
			+q.x * y - q.y * x + q.z * w + q.w * z,
			-q.x * x - q.y * y - q.z * z + q.w * w
		);
	}


	/**
	 * @brief 쿼터니언과 벡터의 곱셈 연산을 수행합니다.
	 *
	 * @param vec 곱셈 연산을 수행할 벡터 값입니다.
	 *
	 * @return 곱셈 연산을 수행한 결과를 반환합니다.
	 */
	Vec3f operator*(Vec3f&& vec) const
	{
		return v * 2.0f * Vec3f::Dot(v, vec) + vec * (w * w - Vec3f::Dot(v, v)) + Vec3f::Cross(v, vec) * 2.0f * w;
	}


	/**
	 * @brief 쿼터니언과 벡터의 곱셈 연산을 수행합니다.
	 *
	 * @param vec 곱셈 연산을 수행할 벡터 값입니다.
	 *
	 * @return 곱셈 연산을 수행한 결과를 반환합니다.
	 */
	Vec3f operator*(const Vec3f& vec) const
	{
		return v * 2.0f * Vec3f::Dot(v, vec) + vec * (w * w - Vec3f::Dot(v, v)) + Vec3f::Cross(v, vec) * 2.0f * w;
	}


	/**
	 * @brief 두 쿼터니언에 대응하는 원소를 더합니다.
	 *
	 * @param q 쿼터니언의 덧셈을 수행할 피연산자입니다.
	 *
	 * @return 연산을 수행한 쿼터니언의 참조자를 반환합니다.
	 */
	Quat& operator+=(Quat&& q) noexcept
	{
		x += q.x;
		y += q.y;
		z += q.z;
		w += q.w;

		return *this;
	}


	/**
	 * @brief 두 쿼터니언에 대응하는 원소를 더합니다.
	 *
	 * @param q 쿼터니언의 덧셈을 수행할 피연산자입니다.
	 *
	 * @return 연산을 수행한 쿼터니언의 참조자를 반환합니다.
	 */
	Quat& operator+=(const Quat& q) noexcept
	{
		x += q.x;
		y += q.y;
		z += q.z;
		w += q.w;

		return *this;
	}


	/**
	 * @brief 두 쿼터니언에 대응하는 원소를 뺍니다.
	 *
	 * @param q 쿼터니언의 뺄셈을 수행할 피연산자입니다.
	 *
	 * @return 연산을 수행한 쿼터니언의 참조자를 반환합니다.
	 */
	Quat& operator-=(Quat&& q) noexcept
	{
		x -= q.x;
		y -= q.y;
		z -= q.z;
		w -= q.w;

		return *this;
	}


	/**
	 * @brief 두 쿼터니언에 대응하는 원소를 뺍니다.
	 *
	 * @param q 쿼터니언의 뺄셈을 수행할 피연산자입니다.
	 *
	 * @return 연산을 수행한 쿼터니언의 참조자를 반환합니다.
	 */
	Quat& operator-=(const Quat& q) noexcept
	{
		x -= q.x;
		y -= q.y;
		z -= q.z;
		w -= q.w;

		return *this;
	}


	/**
	 * @brief 쿼터니언의 스케일 연산을 수행합니다.
	 *
	 * @param s 스케일 연산을 수행할 값입니다.
	 *
	 * @return 연산을 수행한 쿼터니언의 참조자를 반환합니다.
	 */
	Quat& operator*=(float s) noexcept
	{
		x *= s;
		y *= s;
		z *= s;
		w *= s;

		return *this;
	}


	/**
	 * @brief 두 쿼터니언 동일한지 검사합니다.
	 *
	 * @param  q 검사를 수행할 피연산자입니다.
	 *
	 * @return 두 쿼터니언이 동일하다면 true, 그렇지 않으면 false를 반환합니다.
	 */
	bool operator==(Quat&& q) const
	{
		return Abs(x - q.x) <= EPSILON && Abs(y - q.y) <= Epsilon && Abs(z - q.z) <= Epsilon && Abs(w - q.w) <= Epsilon;
	}


	/**
	 * @brief 두 쿼터니언 동일한지 검사합니다.
	 *
	 * @param q 검사를 수행할 피연산자입니다.
	 *
	 * @return 두 쿼터니언이 동일하다면 true, 그렇지 않으면 false를 반환합니다.
	 */
	bool operator==(const Quat& q) const
	{
		return Abs(x - q.x) <= Epsilon && Abs(y - q.y) <= Epsilon && Abs(z - q.z) <= Epsilon && Abs(w - q.w) <= Epsilon;
	}


	/**
	 * @brief 두 쿼터니언 동일하지 않은지 검사합니다.
	 *
	 * @param q 검사를 수행할 피연산자입니다.
	 *
	 * @return 두 쿼터니언이 동일하지 않다면 true, 그렇다면 false를 반환합니다.
	 */
	bool operator!=(Quat&& q) const
	{
		return Abs(x - q.x) > Epsilon || Abs(y - q.y) > Epsilon || Abs(z - q.z) > Epsilon || Abs(w - q.w) > Epsilon;
	}


	/**
	 * @brief 두 쿼터니언 동일하지 않은지 검사합니다.
	 *
	 * @param q 검사를 수행할 피연산자입니다.
	 *
	 * @return 두 쿼터니언이 동일하지 않다면 true, 그렇다면 false를 반환합니다.
	 */
	bool operator!=(const Quat& q) const
	{
		return Abs(x - q.x) > Epsilon || Abs(y - q.y) > Epsilon || Abs(z - q.z) > Epsilon || Abs(w - q.w) > Epsilon;
	}


	/**
	 * @brief 각도와 축을 이용해서 쿼터니언을 얻습니다.
	 *
	 * @parma axis 축 입니다.
	 * @param radian 라디안 단위의 각도입니다.
	 */
	static inline Quat AxisRadian(const Vec3f& axis, float radian)
	{
		float s = Sin(radian * 0.5f);
		float c = Cos(radian * 0.5f);
		Vec3f norm = Vec3f::Normalize(axis);

		return Quat(norm.x * s, norm.y * s, norm.z * s, c);
	}


	/**
	 * @brief 각도와 축을 이용해서 쿼터니언을 얻습니다.
	 *
	 * @parma axis 축 입니다.
	 * @param angle 육십분법 단위의 각도입니다.
	 */
	static inline Quat AxisAngle(const Vec3f& axis, float angle)
	{
		float radian = ToRadian(angle);

		float s = Sin(radian * 0.5f);
		float c = Cos(radian * 0.5f);
		Vec3f norm = Vec3f::Normalize(axis);

		return Quat(norm.x * s, norm.y * s, norm.z * s, c);
	}


	/**
	 * @brief 쿼터니언의 축을 얻습니다.
	 *
	 * @param q 축을 얻을 쿼터니언입니다.
	 *
	 * @return 쿼터니언의 축을 반환합니다.
	 */
	static inline Vec3f Axis(const Quat& q)
	{
		return Vec3f::Normalize(Vec3f(q.x, q.y, q.z));
	}


	/**
	 * @brief 쿼터니언의 라디안 각을 얻습니다.
	 *
	 * @param q 라디안 각도를 얻을 쿼터니언입니다.
	 *
	 * @return 쿼터니언의 라디안 각을 반환합니다.
	 */
	static inline float Radian(const Quat& q)
	{
		return 2.0f * ACos(q.w);
	}


	/**
	 * @brief 쿼터니언의 육십분법 각을 얻습니다.
	 *
	 * @param q 육십분법 각도를 얻을 쿼터니언입니다.
	 *
	 * @return 쿼터니언의 육십분법 각을 반환합니다.
	 */
	static inline float Angle(const Quat& q)
	{
		return ToDegree(2.0f * ACos(q.w));
	}


	/**
	 * @brief 두 쿼터니언의 내적 연산을 수행합니다.
	 *
	 * @param lhs 쿼터니언의 내적 연산을 수행할 좌측 피연산자입니다.
	 * @param rhs 쿼터니언의 내적 연산을 수행할 우측 피연산자입니다.
	 *
	 * @return 내적 연산 결과를 반환합니다.
	 */
	static inline float Dot(const Quat& lhs, const Quat& rhs)
	{
		return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
	}


	/**
	 * @brief 쿼터니언의 크기 제곱 값을 계산합니다.
	 *
	 * @param q 크기 제곱을 계산할 쿼터니언입니다.
	 *
	 * @return 계산된 크기 제곱 값을 반환합니다.
	 */
	static inline float LengthSq(const Quat& q)
	{
		return q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w;
	}


	/**
	 * @brief 쿼터니언의 크기를 얻습니다.
	 *
	 * @param q 크기를 계산할 벡터입니다.
	 *
	 * @return 계산된 크기 값을 반환합니다.
	 */
	static inline float Length(const Quat& q)
	{
		float lengthSq = Quat::LengthSq(q);
		return Sqrt(lengthSq);
	}


	/**
	 * @brief 쿼터니언을 정규화합니다.
	 *
	 * @param q 정규화 할 쿼터니언입니다.
	 *
	 * @return 정규화된 쿼터니언을 반환합니다.
	 */
	static inline Quat Normalize(const Quat& q)
	{
		float length = Quat::Length(q);

		if (NearZero(length))
		{
			return Quat();
		}

		float invLength = 1.0f / length;
		return Quat(q.x * invLength, q.y * invLength, q.z * invLength, q.w * invLength);
	}


	/**
	 * @brief 켤레 쿼터니언을 얻습니다.
	 *
	 * @param q 켤러 값을 계산할 쿼터니언입니다.
	 *
	 * @return 켤레 쿼터니언을 반환합니다.
	 */
	static inline Quat Conjugate(const Quat& q)
	{
		return Quat(-q.x, -q.y, -q.z, q.w);
	}


	/**
	 * @brief 쿼터니언의 곱의 역원을 얻습니다.
	 *
	 * @param 곱의 역원을 계산할 쿼터니언입니다.
	 *
	 * @return 쿼터니언의 곱의 역원을 반환합니다.
	 */
	static inline Quat Inverse(const Quat& q)
	{
		float invLengthSq = 1.0f / Quat::LengthSq(q);
		return Quat(-q.x * invLengthSq, -q.y * invLengthSq, -q.z * invLengthSq, q.w * invLengthSq);
	}


	/**
	 * @brief 모든 원소가 0인 쿼터니언을 얻습니다.
	 *
	 * @return 모든 원소가 0인 쿼터니언을 반환합니다.
	 */
	static inline Quat Zero()
	{
		return Quat(0.0f, 0.0f, 0.0f, 0.0f);
	}


	/**
	 * @brief w의 값이 1인 쿼터니언을 얻습니다.
	 *
	 * @return w의 값이 1인 쿼터니언을 얻습니다.
	 */
	static inline Quat Identity()
	{
		return Quat(0.0f, 0.0f, 0.0f, 1.0f);
	}


	/**
	 * @brief 두 벡터 간의 회전 쿼터니언을 얻습니다.
	 *
	 * @param s 회전 시작 방향입니다.
	 * @param e 회전 끝 방향입니다.
	 *
	 * @return 두 벡터 간의 회전 쿼터니언을 반환합니다.
	 */
	static inline Quat Rotate(const Vec3f& s, const Vec3f& e)
	{
		Vec3f start = Vec3f::Normalize(s);
		Vec3f end = Vec3f::Normalize(e);
		Quat quat;

		if (start == end)
		{
			return Quat();
		}

		if (start == -end)
		{
			Vec3f ortho;

			if (Abs(start.y) < Abs(start.x))
			{
				ortho = Vec3f(0.0f, 1.0f, 0.0f);
			}
			else if (Abs(start.z) < Abs(start.y) && Abs(start.z) < Abs(start.x))
			{
				ortho = Vec3f(0.0f, 0.0f, 1.0f);
			}
			else
			{
				ortho = Vec3f(1.0f, 0.0f, 0.0f);
			}

			Vec3f axis = Vec3f::Normalize(Vec3f::Cross(start, ortho));
			return AxisRadian(axis, 0.0f);
		}

		Vec3f half = Vec3f::Normalize(start + end);
		Vec3f axis = Vec3f::Cross(start, half);
		return Quat(axis.x, axis.y, axis.z, Vec3f::Dot(start, half));
	}


	/**
	 * @brief 두 쿼터니언을 선형 보간합니다.
	 *
	 * @param s 보간의 시작 쿼터니언입니다.
	 * @param e 보간의 끝 쿼터니언입니다.
	 * @param t 두 쿼터니언의 보간 비율입니다.
	 *
	 * @return 보간된 쿼터니언을 반환합니다.
	 */
	static inline Quat Lerp(const Quat& s, const Quat& e, const float& t)
	{
		return s * (1.0f - t) + e * t;
	}


	/**
	 * @brief 두 쿼터니언의 정규화된 선형 보간값을 계산합니다.
	 *
	 * @param s 보간의 시작 쿼터니언입니다.
	 * @param e 보간의 끝 쿼터니언입니다.
	 * @param t 두 쿼터니언의 보간 비율입니다.
	 *
	 * @return 정규화된 선형 보간 값을 반환합니다.
	 */
	static inline Quat Nlerp(const Quat& s, const Quat& e, const float& t)
	{
		return Normalize(Lerp(s, e, t));
	}


	/**
	 * @brief 두 쿼터니언을 구면 선형 보간합니다.
	 *
	 * @param s 보간의 시작 쿼터니언입니다.
	 * @param e 보간의 끝 쿼터니언입니다.
	 * @param t 두 쿼터니언의 보간 비율입니다.
	 *
	 * @return 보간된 쿼터니언을 반환합니다.
	 */
	static inline Quat Slerp(const Quat& s, const Quat& e, const float& t)
	{
		return Pow(Inverse(s) * e, t) * s;
	}


	/**
	 * @brief 두 쿼터니언의 정규화된 구면 선형 보간값을 계산합니다.
	 *
	 * @param s 보간의 시작 쿼터니언입니다.
	 * @param e 보간의 끝 쿼터니언입니다.
	 * @param t 두 쿼터니언의 보간 비율입니다.
	 *
	 * @return 정규화된 선형 보간 값을 반환합니다.
	 */
	static inline Quat Nslerp(const Quat& s, const Quat& e, const float& t)
	{
		return Normalize(Slerp(s, e, t));
	}


	/**
	 * @brief 쿼터니언의 거듭 제곱을 수행합니다.
	 *
	 * @param q 거듭 제곱 연산을 수행할 쿼터니언입니다.
	 * @param power 거듭제곱 값입니다.
	 *
	 * @return 거듭 제곱이 수행된 쿼터니언 값을 반환합니다.
	 */
	static inline Quat Pow(const Quat& q, const float power)
	{
		float radian = Quat::Radian(q);
		Vec3f axis = Quat::Axis(q);

		float c = Cos(power * radian * 0.5f);
		float s = Sin(power * radian * 0.5f);

		return Quat(axis.x * s, axis.y * s, axis.z * s, c);
	}


	/**
	 * @brief 방향과 위를 기준으로 쿼터니언을 생성합니다.
	 *
	 * @param direction 쿼터니언 생성에 사용할 방향입니다.
	 * @param up  쿼터니언 생성에 사용할 위 방향입니다.
	 *
	 * @return 생성된 쿼터니언을 반환합니다.
	 */
	static inline Quat LookRotate(const Vec3f& direction, const Vec3f& up)
	{
		Vec3f f = Vec3f::Normalize(direction);
		Vec3f u = Vec3f::Normalize(up);
		Vec3f r = Vec3f::Cross(u, f);

		u = Vec3f::Cross(f, r);

		Quat f2d = Quat::Rotate(Vec3f(0.0f, 0.0f, 1.0f), f);
		Vec3f objectUp = f2d * Vec3f(0.0f, 1.0f, 0.0f);
		Quat u2u = Quat::Rotate(objectUp, u);

		return Quat::Normalize(f2d * u2u);
	}


	/**
	 * @brief 쿼터니언을 행렬로 변환합니다.
	 *
	 * @param q 행렬로 변환할 쿼터니언입니다.
	 *
	 * @return 변환된 행렬을 반환합니다.
	 */
	static inline Mat4x4 ToMat(const Quat& q)
	{
		Vec3f r = q * Vec3f(1.0f, 0.0f, 0.0f);
		Vec3f u = q * Vec3f(0.0f, 1.0f, 0.0f);
		Vec3f f = q * Vec3f(0.0f, 0.0f, 1.0f);

		return Mat4x4(
			r.x, r.y, r.z, 0.0f,
			u.x, u.y, u.z, 0.0f,
			f.x, f.y, f.z, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
	}


	/**
	 * @brief 행렬을 쿼터니언으로 변환합니다.
	 *
	 * @param m 쿼터니언으로 변환할 행렬입니다.
	 *
	 * @return 변환된 쿼터니언을 반환합니다.
	 */
	static inline Quat ToQuat(const Mat4x4& m)
	{
		Vec3f u = Vec3f::Normalize(Vec3f(m.e10, m.e11, m.e12));
		Vec3f f = Vec3f::Normalize(Vec3f(m.e20, m.e21, m.e22));
		Vec3f r = Vec3f::Cross(u, f);
		u = Vec3f::Cross(f, r);

		return LookRotate(f, u);
	}


	/**
	 * @brief 두 쿼터니언의 방향이 같은지 확인합니다.
	 *
	 * @param lhs 방향이 같은지 비교할 쿼터니언입니다.
	 * @param rhs 방향이 같은지 비교할 또 다른 쿼터니언입니다.
	 */
	bool IsSameOrientation(const Quat& lhs, const Quat& rhs)
	{
		return (Abs(lhs.x - rhs.x) <= Epsilon && Abs(lhs.y - rhs.y) <= Epsilon && Abs(lhs.z - rhs.z) <= Epsilon && Abs(lhs.w - rhs.w) <= Epsilon)
			|| (Abs(lhs.x + rhs.x) <= Epsilon && Abs(lhs.y + rhs.y) <= Epsilon && Abs(lhs.z + rhs.z) <= Epsilon && Abs(lhs.w + rhs.w) <= Epsilon);
	}


	/**
	 * @brief 쿼터니언의 다양한 원소 형식입니다.
	 */
	union
	{
		struct
		{
			float x;
			float y;
			float z;
			float w;
		};
		struct
		{
			Vec3f v;
			float w;
		};
		float data[4];
	};
};

}
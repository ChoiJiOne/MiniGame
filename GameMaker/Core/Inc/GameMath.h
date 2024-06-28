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

}
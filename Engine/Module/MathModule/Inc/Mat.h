#pragma once

#include "MathModule.h"
#include "Vec.h"


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
		return MathModule::Abs(e00 - m.e00) <= Epsilon
			&& MathModule::Abs(e01 - m.e01) <= Epsilon
			&& MathModule::Abs(e10 - m.e10) <= Epsilon
			&& MathModule::Abs(e11 - m.e11) <= Epsilon;
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
		return MathModule::Abs(e00 - m.e00) <= Epsilon
			&& MathModule::Abs(e01 - m.e01) <= Epsilon
			&& MathModule::Abs(e10 - m.e10) <= Epsilon
			&& MathModule::Abs(e11 - m.e11) <= Epsilon;
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
		return MathModule::Abs(e00 - m.e00) > Epsilon
			|| MathModule::Abs(e01 - m.e01) > Epsilon
			|| MathModule::Abs(e10 - m.e10) > Epsilon
			|| MathModule::Abs(e11 - m.e11) > Epsilon;
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
		return MathModule::Abs(e00 - m.e00) > Epsilon
			|| MathModule::Abs(e01 - m.e01) > Epsilon
			|| MathModule::Abs(e10 - m.e10) > Epsilon
			|| MathModule::Abs(e11 - m.e11) > Epsilon;
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
		return MathModule::Abs(e00 - m.e00) <= Epsilon
			&& MathModule::Abs(e01 - m.e01) <= Epsilon
			&& MathModule::Abs(e02 - m.e02) <= Epsilon
			&& MathModule::Abs(e10 - m.e10) <= Epsilon
			&& MathModule::Abs(e11 - m.e11) <= Epsilon
			&& MathModule::Abs(e12 - m.e12) <= Epsilon
			&& MathModule::Abs(e20 - m.e20) <= Epsilon
			&& MathModule::Abs(e21 - m.e21) <= Epsilon
			&& MathModule::Abs(e22 - m.e22) <= Epsilon;
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
		return MathModule::Abs(e00 - m.e00) <= Epsilon
			&& MathModule::Abs(e01 - m.e01) <= Epsilon
			&& MathModule::Abs(e02 - m.e02) <= Epsilon
			&& MathModule::Abs(e10 - m.e10) <= Epsilon
			&& MathModule::Abs(e11 - m.e11) <= Epsilon
			&& MathModule::Abs(e12 - m.e12) <= Epsilon
			&& MathModule::Abs(e20 - m.e20) <= Epsilon
			&& MathModule::Abs(e21 - m.e21) <= Epsilon
			&& MathModule::Abs(e22 - m.e22) <= Epsilon;
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
		return MathModule::Abs(e00 - m.e00) > Epsilon
			|| MathModule::Abs(e01 - m.e01) > Epsilon
			|| MathModule::Abs(e02 - m.e02) > Epsilon
			|| MathModule::Abs(e10 - m.e10) > Epsilon
			|| MathModule::Abs(e11 - m.e11) > Epsilon
			|| MathModule::Abs(e12 - m.e12) > Epsilon
			|| MathModule::Abs(e20 - m.e20) > Epsilon
			|| MathModule::Abs(e21 - m.e21) > Epsilon
			|| MathModule::Abs(e22 - m.e22) > Epsilon;
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
		return MathModule::Abs(e00 - m.e00) > Epsilon
			|| MathModule::Abs(e01 - m.e01) > Epsilon
			|| MathModule::Abs(e02 - m.e02) > Epsilon
			|| MathModule::Abs(e10 - m.e10) > Epsilon
			|| MathModule::Abs(e11 - m.e11) > Epsilon
			|| MathModule::Abs(e12 - m.e12) > Epsilon
			|| MathModule::Abs(e20 - m.e20) > Epsilon
			|| MathModule::Abs(e21 - m.e21) > Epsilon
			|| MathModule::Abs(e22 - m.e22) > Epsilon;
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
		return MathModule::Abs(e00 - m.e00) <= Epsilon
			&& MathModule::Abs(e01 - m.e01) <= Epsilon
			&& MathModule::Abs(e02 - m.e02) <= Epsilon
			&& MathModule::Abs(e03 - m.e03) <= Epsilon
			&& MathModule::Abs(e10 - m.e10) <= Epsilon
			&& MathModule::Abs(e11 - m.e11) <= Epsilon
			&& MathModule::Abs(e12 - m.e12) <= Epsilon
			&& MathModule::Abs(e13 - m.e13) <= Epsilon
			&& MathModule::Abs(e20 - m.e20) <= Epsilon
			&& MathModule::Abs(e21 - m.e21) <= Epsilon
			&& MathModule::Abs(e22 - m.e22) <= Epsilon
			&& MathModule::Abs(e23 - m.e23) <= Epsilon
			&& MathModule::Abs(e30 - m.e30) <= Epsilon
			&& MathModule::Abs(e31 - m.e31) <= Epsilon
			&& MathModule::Abs(e32 - m.e32) <= Epsilon
			&& MathModule::Abs(e33 - m.e33) <= Epsilon;
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
		return MathModule::Abs(e00 - m.e00) <= Epsilon
			&& MathModule::Abs(e01 - m.e01) <= Epsilon
			&& MathModule::Abs(e02 - m.e02) <= Epsilon
			&& MathModule::Abs(e03 - m.e03) <= Epsilon
			&& MathModule::Abs(e10 - m.e10) <= Epsilon
			&& MathModule::Abs(e11 - m.e11) <= Epsilon
			&& MathModule::Abs(e12 - m.e12) <= Epsilon
			&& MathModule::Abs(e13 - m.e13) <= Epsilon
			&& MathModule::Abs(e20 - m.e20) <= Epsilon
			&& MathModule::Abs(e21 - m.e21) <= Epsilon
			&& MathModule::Abs(e22 - m.e22) <= Epsilon
			&& MathModule::Abs(e23 - m.e23) <= Epsilon
			&& MathModule::Abs(e30 - m.e30) <= Epsilon
			&& MathModule::Abs(e31 - m.e31) <= Epsilon
			&& MathModule::Abs(e32 - m.e32) <= Epsilon
			&& MathModule::Abs(e33 - m.e33) <= Epsilon;
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
		return MathModule::Abs(e00 - m.e00) > Epsilon
			|| MathModule::Abs(e01 - m.e01) > Epsilon
			|| MathModule::Abs(e02 - m.e02) > Epsilon
			|| MathModule::Abs(e03 - m.e03) > Epsilon
			|| MathModule::Abs(e10 - m.e10) > Epsilon
			|| MathModule::Abs(e11 - m.e11) > Epsilon
			|| MathModule::Abs(e12 - m.e12) > Epsilon
			|| MathModule::Abs(e13 - m.e13) > Epsilon
			|| MathModule::Abs(e20 - m.e20) > Epsilon
			|| MathModule::Abs(e21 - m.e21) > Epsilon
			|| MathModule::Abs(e22 - m.e22) > Epsilon
			|| MathModule::Abs(e23 - m.e23) > Epsilon
			|| MathModule::Abs(e30 - m.e30) > Epsilon
			|| MathModule::Abs(e31 - m.e31) > Epsilon
			|| MathModule::Abs(e32 - m.e32) > Epsilon
			|| MathModule::Abs(e33 - m.e33) > Epsilon;
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
		return MathModule::Abs(e00 - m.e00) > Epsilon
			|| MathModule::Abs(e01 - m.e01) > Epsilon
			|| MathModule::Abs(e02 - m.e02) > Epsilon
			|| MathModule::Abs(e03 - m.e03) > Epsilon
			|| MathModule::Abs(e10 - m.e10) > Epsilon
			|| MathModule::Abs(e11 - m.e11) > Epsilon
			|| MathModule::Abs(e12 - m.e12) > Epsilon
			|| MathModule::Abs(e13 - m.e13) > Epsilon
			|| MathModule::Abs(e20 - m.e20) > Epsilon
			|| MathModule::Abs(e21 - m.e21) > Epsilon
			|| MathModule::Abs(e22 - m.e22) > Epsilon
			|| MathModule::Abs(e23 - m.e23) > Epsilon
			|| MathModule::Abs(e30 - m.e30) > Epsilon
			|| MathModule::Abs(e31 - m.e31) > Epsilon
			|| MathModule::Abs(e32 - m.e32) > Epsilon
			|| MathModule::Abs(e33 - m.e33) > Epsilon;
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
			   x,    y,    z, 1.0f
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
			 p.x,  p.y,  p.z, 1.0f
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
			xScale,   0.0f,   0.0f, 0.0f,
			  0.0f, yScale,   0.0f, 0.0f,
			  0.0f,   0.0f, zScale, 0.0f,
			  0.0f,   0.0f,   0.0f, 1.0f
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
			scale.x,    0.0f,    0.0f, 0.0f,
			   0.0f, scale.y,    0.0f, 0.0f,
			   0.0f,    0.0f, scale.z, 0.0f,
		       0.0f,    0.0f,    0.0f, 1.0f
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
		float c = MathModule::Cos(radian);
		float s = MathModule::Sin(radian);

		return Mat4x4(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f,    c,    s, 0.0f,
			0.0f,   -s,    c, 0.0f,
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
		float c = MathModule::Cos(radian);
		float s = MathModule::Sin(radian);

		return Mat4x4(
			   c, 0.0f,   -s, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			   s, 0.0f,    c, 0.0f,
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
		float c = MathModule::Cos(radian);
		float s = MathModule::Sin(radian);

		return Mat4x4(
			   c,    s, 0.0f, 0.0f,
			  -s,    c, 0.0f, 0.0f,
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
		float c = MathModule::Cos(radian);
		float s = MathModule::Sin(radian);
		Vec3f r = Vec3f::Normalize(axis);

		return Mat4x4(
				  c + r.x * r.x * (1.0f - c), r.y * r.x * (1.0f - c) + r.z * s, r.z * r.x * (1.0f - c) - r.y * s, 0.0f,
			r.x * r.y * (1.0f - c) - r.z * s,       c + r.y * r.y * (1.0f - c), r.z * r.y * (1.0f - c) + r.x * s, 0.0f,
			r.x * r.z * (1.0f - c) + r.y * s, r.y * r.z * (1.0f - c) - r.x * s,       c + r.z * r.z * (1.0f - c), 0.0f,
	    								0.0f,                             0.0f,                             0.0f, 1.0f
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
			  2.0f / width,            0.0f,           0.0f, 0.0f,
			          0.0f,   2.0f / height,           0.0f, 0.0f,
			          0.0f,            0.0f,  -2.0f / depth, 0.0f,
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
		float tanHalfFovy = MathModule::Sin(halfFov) / MathModule::Cos(halfFov);

		return Mat4x4(
			1.0f / (aspect * tanHalfFovy),                 0.0f,                                    0.0f,  0.0f,
					                 0.0f, 1.0f / (tanHalfFovy),                                    0.0f,  0.0f,
					                 0.0f,                 0.0f,        -(farZ + nearZ) / (farZ - nearZ), -1.0f,
					                 0.0f,                 0.0f, -(2.0f * farZ * nearZ) / (farZ - nearZ),  1.0f
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
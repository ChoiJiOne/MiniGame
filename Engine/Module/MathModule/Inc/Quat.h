#pragma once

#include "Mat.h"
#include "MathModule.h"
#include "Vec.h"


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
		return MathModule::Abs(x - q.x) <= Epsilon && MathModule::Abs(y - q.y) <= Epsilon && MathModule::Abs(z - q.z) <= Epsilon && MathModule::Abs(w - q.w) <= Epsilon;
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
		return MathModule::Abs(x - q.x) <= Epsilon && MathModule::Abs(y - q.y) <= Epsilon && MathModule::Abs(z - q.z) <= Epsilon && MathModule::Abs(w - q.w) <= Epsilon;
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
		return MathModule::Abs(x - q.x) > Epsilon || MathModule::Abs(y - q.y) > Epsilon || MathModule::Abs(z - q.z) > Epsilon || MathModule::Abs(w - q.w) > Epsilon;
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
		return MathModule::Abs(x - q.x) > Epsilon || MathModule::Abs(y - q.y) > Epsilon || MathModule::Abs(z - q.z) > Epsilon || MathModule::Abs(w - q.w) > Epsilon;
	}


	/**
	 * @brief 각도와 축을 이용해서 쿼터니언을 얻습니다.
	 *
	 * @parma axis 축 입니다.
	 * @param radian 라디안 단위의 각도입니다.
	 */
	static inline Quat AxisRadian(const Vec3f& axis, float radian)
	{
		float s = MathModule::Sin(radian * 0.5f);
		float c = MathModule::Cos(radian * 0.5f);
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
		float radian = MathModule::ToRadian(angle);

		float s = MathModule::Sin(radian * 0.5f);
		float c = MathModule::Cos(radian * 0.5f);
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
		return 2.0f * MathModule::ACos(q.w);
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
		return MathModule::ToDegree(2.0f * MathModule::ACos(q.w));
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
		return MathModule::Sqrt(lengthSq);
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

		if (MathModule::NearZero(length))
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

			if (MathModule::Abs(start.y) < MathModule::Abs(start.x))
			{
				ortho = Vec3f(0.0f, 1.0f, 0.0f);
			}
			else if (MathModule::Abs(start.z) < MathModule::Abs(start.y) && MathModule::Abs(start.z) < MathModule::Abs(start.x))
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

		float c = MathModule::Cos(power * radian * 0.5f);
		float s = MathModule::Sin(power * radian * 0.5f);

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
			 r.x,  r.y,  r.z, 0.0f,
			 u.x,  u.y,  u.z, 0.0f,
		 	 f.x,  f.y,  f.z, 0.0f,
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
		return (MathModule::Abs(lhs.x - rhs.x) <= Epsilon && MathModule::Abs(lhs.y - rhs.y) <= Epsilon && MathModule::Abs(lhs.z - rhs.z) <= Epsilon && MathModule::Abs(lhs.w - rhs.w) <= Epsilon)
			|| (MathModule::Abs(lhs.x + rhs.x) <= Epsilon && MathModule::Abs(lhs.y + rhs.y) <= Epsilon && MathModule::Abs(lhs.z + rhs.z) <= Epsilon && MathModule::Abs(lhs.w + rhs.w) <= Epsilon);
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
#pragma once

#include "Mat.h"
#include "Quat.h"
#include "Vec.h"


/**
 * @brief TRS(Translation, Rotation, Scale) 변환을 수행합니다.
 */
struct Transform
{
	/**
	 * @brief 트랜스폼의 기본 생성자입니다.
	 */
	Transform() noexcept
		: position(0.0f, 0.0f, 0.0f)
		, rotate(0.0f, 0.0f, 0.0f, 1.0f)
		, scale(1.0f, 1.0f, 1.0f) {}


	/**
	 * @brief 트랜스폼의 생성자입니다.
	 *
	 * @param p 이동 변환 수행 시 이동할 위치입니다.
	 * @param r 회전 변환 수행 시 참조할 쿼터니언입니다.
	 * @param s 크기 변환 수행 시 참조할 크기입니다.
	 */
	Transform(Vec3f&& p, Quat&& r, Vec3f&& s) noexcept
		: position(p)
		, rotate(r)
		, scale(s) {}


	/**
	 * @brief 트랜스폼의 생성자입니다.
	 *
	 * @param p 이동 변환 수행 시 이동할 위치입니다.
	 * @param r 회전 변환 수행 시 참조할 쿼터니언입니다.
	 * @param s 크기 변환 수행 시 참조할 크기입니다.
	 */
	Transform(const Vec3f& p, const Quat& r, const Vec3f& s) noexcept
		: position(p)
		, rotate(r)
		, scale(s) {}


	/**
	 * @brief 트랜스폼의 복사 생성자입니다.
	 *
	 * @param instance 복사할 트랜스폼의 인스턴스입니다.
	 */
	Transform(Transform&& instance) noexcept
		: position(instance.position)
		, rotate(instance.rotate)
		, scale(instance.scale) {}


	/**
	 * @brief 트랜스폼의 복사 생성자입니다.
	 *
	 * @param instance 복사할 트랜스폼의 인스턴스입니다.
	 */
	Transform(const Transform& instance) noexcept
		: position(instance.position)
		, rotate(instance.rotate)
		, scale(instance.scale) {}


	/**
	 * @brief 트랜스폼의 대입 연산자입니다.
	 *
	 * @param instance 복사할 트랜스폼의 인스턴스입니다.
	 *
	 * @return 대입한 트랜스폼의 참조자를 반환합니다.
	 */
	Transform& operator=(Transform&& instance) noexcept
	{
		if (this == &instance) return *this;

		position = instance.position;
		rotate = instance.rotate;
		scale = instance.scale;

		return *this;
	}


	/**
	 * @brief 트랜스폼의 대입 연산자입니다.
	 *
	 * @param instance 복사할 트랜스폼의 인스턴스입니다.
	 *
	 * @return 대입한 트랜스폼의 참조자를 반환합니다.
	 */
	Transform& operator=(const Transform& instance) noexcept
	{
		if (this == &instance) return *this;

		position = instance.position;
		rotate = instance.rotate;
		scale = instance.scale;

		return *this;
	}


	/**
	 * @brief 두 트랜스폼의 원소가 일치하는지 확인합니다.
	 *
	 * @param transform 원소가 일치하는지 확인할 트랜스폼입니다.
	 *
	 * @return 두 트랜스폼의 원소가 일치한다면 true, 그렇지 않으면 false를 반환합니다.
	 */
	bool operator==(Transform&& transform) noexcept
	{
		return position == transform.position && rotate == transform.rotate && scale == transform.scale;
	}


	/**
	 * @brief 두 트랜스폼의 원소가 일치하는지 확인합니다.
	 *
	 * @param transform 원소가 일치하는지 확인할 트랜스폼입니다.
	 *
	 * @return 두 트랜스폼의 원소가 일치한다면 true, 그렇지 않으면 false를 반환합니다.
	 */
	bool operator==(const Transform& transform) noexcept
	{
		return position == transform.position && rotate == transform.rotate && scale == transform.scale;
	}


	/**
	 * @brief 두 트랜스폼의 원소가 일치지 않는지 확인합니다.
	 *
	 * @param transform 원소가 일치하지 않는지 확인할 트랜스폼입니다.
	 *
	 * @return 두 트랜스폼의 원소가 일치하지 않는다면 true, 그렇다면 false를 반환합니다.
	 */
	bool operator!=(Transform&& transform) noexcept
	{
		return position != transform.position || rotate != transform.rotate || scale != transform.scale;
	}


	/**
	 * @brief 두 트랜스폼의 원소가 일치지 않는지 확인합니다.
	 *
	 * @param transform 원소가 일치하지 않는지 확인할 트랜스폼입니다.
	 *
	 * @return 두 트랜스폼의 원소가 일치하지 않는다면 true, 그렇다면 false를 반환합니다.
	 */
	bool operator!=(const Transform& transform) noexcept
	{
		return position != transform.position || rotate != transform.rotate || scale != transform.scale;
	}


	/**
	 * @brief 트랜스 폼을 행렬로 변환합니다.
	 *
	 * @param transform 행렬로 변환할 트랜스폼입니다.
	 *
	 * @return 변환된 행렬을 반환합니다.
	 */
	static inline Mat4x4 ToMat(const Transform& transform)
	{
		Vec3f x = transform.rotate * Vec3f(1.0f, 0.0f, 0.0f);
		Vec3f y = transform.rotate * Vec3f(0.0f, 1.0f, 0.0f);
		Vec3f z = transform.rotate * Vec3f(0.0f, 0.0f, 1.0f);

		x = x * transform.scale.x;
		y = y * transform.scale.y;
		z = z * transform.scale.z;

		Vec3f p = transform.position;

		return Mat4x4(
			x.x, x.y, x.z, 0.0f,
			y.x, y.y, y.z, 0.0f,
			z.x, z.y, z.z, 0.0f,
			p.x, p.y, p.z, 1.0f
		);
	}


	/**
	 * @brief 행렬을 트랜스폼으로 변환합니다.
	 *
	 * @param m 트랜스폼으로 변환할 행렬입니다.
	 *
	 * @return 변환된 트랜스폼을 반환합니다.
	 */
	static inline Transform ToTransform(const Mat4x4& m)
	{
		Transform t;

		t.position = Vec3f(m.e30, m.e31, m.e32);
		t.rotate = Quat::ToQuat(m);

		Mat4x4 rotateScale(
			m.e00, m.e01, m.e02, 0.0f,
			m.e10, m.e11, m.e12, 0.0f,
			m.e20, m.e21, m.e22, 0.0f,
			 0.0f,  0.0f,  0.0f, 1.0f
		);

		Mat4x4 invRotateScale = Quat::ToMat(Quat::Inverse(t.rotate));
		Mat4x4 scaleKew = invRotateScale * rotateScale;

		t.scale = Vec3f(scaleKew.e00, scaleKew.e11, scaleKew.e22);

		return t;
	}


	/**
	 * @brief 두 트랜스폼을 결합합니다.
	 *
	 * @param lhs 결합할 트랜스폼입니다.
	 * @param rhs 결합할 또 다른 트랜스폼입니다.
	 */
	static inline Transform Combine(const Transform& lhs, const Transform& rhs)
	{
		Transform t;

		t.scale = lhs.scale * rhs.scale;
		t.rotate = rhs.rotate * lhs.rotate;

		t.position = lhs.rotate * (lhs.scale * rhs.position);
		t.position = lhs.position + t.position;

		return t;
	}


	/**
	 * @brief 트랜스폼의 역변환을 얻습니다.
	 *
	 * @param transform 역변환을 계산할 트랜스폼입니다.
	 *
	 * @return 계산된 역변환 트랜스폼을 반환합니다.
	 */
	static inline Transform Inverse(const Transform& transform)
	{
		Transform inv;

		inv.rotate = Quat::Inverse(transform.rotate);

		inv.scale.x = MathModule::Abs(transform.scale.x) < Epsilon ? 0.0f : 1.0f / transform.scale.x;
		inv.scale.y = MathModule::Abs(transform.scale.y) < Epsilon ? 0.0f : 1.0f / transform.scale.y;
		inv.scale.z = MathModule::Abs(transform.scale.z) < Epsilon ? 0.0f : 1.0f / transform.scale.z;

		Vec3f invTranslation = -transform.position;
		inv.position = inv.rotate * (inv.scale * invTranslation);

		return inv;
	}


	/**
	 * @brief 두 트랜스폼을 보간합니다.
	 *
	 * @param s 보간의 시작 트랜스폼입니다.
	 * @param e 보간의 끝 트랜스폼입니다.
	 * @param t 두 트랜스폼의 보간 비율입니다.
	 *
	 * @return 보간된 트랜스폼을 반환합니다.
	 */
	static inline Transform Mix(const Transform& s, const Transform& e, const float& t)
	{
		Quat eRotate = e.rotate;

		if (Quat::Dot(s.rotate, eRotate) < 0.0f)
		{
			eRotate = -eRotate;
		}

		return Transform(
			Vec3f::Lerp(s.position, e.position, t),
			Quat::Nlerp(s.rotate, eRotate, t),
			Vec3f::Lerp(s.scale, e.scale, t)
		);
	}


	/**
	 * @brief 이동 변환 수행 시 이동할 위치입니다.
	 */
	Vec3f position;


	/**
	 * @brief 회전 변환 수행 시 참조할 쿼터니언입니다.
	 */
	Quat rotate;


	/**
	 * @brief 크기 변환 수행 시 참조할 크기입니다.
	 */
	Vec3f scale;
};
#pragma once

#include "GameMath.h"


namespace GameMaker
{
struct Transform
{
	Transform() noexcept
		: position(0.0f, 0.0f, 0.0f)
		, rotate(0.0f, 0.0f, 0.0f, 1.0f)
		, scale(1.0f, 1.0f, 1.0f) {}

	Transform(Vec3f&& p, Quat&& r, Vec3f&& s) noexcept
		: position(p)
		, rotate(r)
		, scale(s) {}

	Transform(const Vec3f& p, const Quat& r, const Vec3f& s) noexcept
		: position(p)
		, rotate(r)
		, scale(s) {}

	Transform(Transform&& instance) noexcept
		: position(instance.position)
		, rotate(instance.rotate)
		, scale(instance.scale) {}

	Transform(const Transform& instance) noexcept
		: position(instance.position)
		, rotate(instance.rotate)
		, scale(instance.scale) {}

	Transform& operator=(Transform&& instance) noexcept
	{
		if (this == &instance) return *this;

		position = instance.position;
		rotate = instance.rotate;
		scale = instance.scale;

		return *this;
	}

	Transform& operator=(const Transform& instance) noexcept
	{
		if (this == &instance) return *this;

		position = instance.position;
		rotate = instance.rotate;
		scale = instance.scale;

		return *this;
	}

	bool operator==(Transform&& transform) noexcept
	{
		return position == transform.position && rotate == transform.rotate && scale == transform.scale;
	}

	bool operator==(const Transform& transform) noexcept
	{
		return position == transform.position && rotate == transform.rotate && scale == transform.scale;
	}

	bool operator!=(Transform&& transform) noexcept
	{
		return position != transform.position || rotate != transform.rotate || scale != transform.scale;
	}

	bool operator!=(const Transform& transform) noexcept
	{
		return position != transform.position || rotate != transform.rotate || scale != transform.scale;
	}

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

	static inline Transform ToTransform(const Mat4x4& m)
	{
		Transform t;

		t.position = Vec3f(m.e30, m.e31, m.e32);
		t.rotate = Quat::ToQuat(m);

		Mat4x4 rotateScale(
			m.e00, m.e01, m.e02, 0.0f,
			m.e10, m.e11, m.e12, 0.0f,
			m.e20, m.e21, m.e22, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);

		Mat4x4 invRotateScale = Quat::ToMat(Quat::Inverse(t.rotate));
		Mat4x4 scaleKew = invRotateScale * rotateScale;

		t.scale = Vec3f(scaleKew.e00, scaleKew.e11, scaleKew.e22);

		return t;
	}

	static inline Transform Combine(const Transform& lhs, const Transform& rhs)
	{
		Transform t;

		t.scale = lhs.scale * rhs.scale;
		t.rotate = rhs.rotate * lhs.rotate;

		t.position = lhs.rotate * (lhs.scale * rhs.position);
		t.position = lhs.position + t.position;

		return t;
	}

	static inline Transform Inverse(const Transform& transform)
	{
		Transform inv;

		inv.rotate = Quat::Inverse(transform.rotate);

		inv.scale.x = Abs(transform.scale.x) < EPSILON ? 0.0f : 1.0f / transform.scale.x;
		inv.scale.y = Abs(transform.scale.y) < EPSILON ? 0.0f : 1.0f / transform.scale.y;
		inv.scale.z = Abs(transform.scale.z) < EPSILON ? 0.0f : 1.0f / transform.scale.z;

		Vec3f invTranslation = -transform.position;
		inv.position = inv.rotate * (inv.scale * invTranslation);

		return inv;
	}

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

	Vec3f position;
	Quat rotate;
	Vec3f scale;
};

}
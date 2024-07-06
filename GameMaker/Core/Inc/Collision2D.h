#pragma once

#include "GameMath.h"


namespace GameMaker
{
struct ICollision2D
{
	enum class EType
	{
		NONE          = 0x00,
		POINT         = 0x01,
		LINE          = 0x02,
		CIRCLE        = 0x03,
		RECT          = 0x04, // AABB
		ORIENTED_RECT = 0x05, // OBB
	};

	ICollision2D() = default;
	virtual ~ICollision2D() {}

	virtual EType GetType() const = 0;
	virtual bool Intersect(const ICollision2D* target) const = 0;
};

struct Line2D : public ICollision2D
{
	Line2D() = default;
	Line2D(const Vec2f& s, const Vec2f& e) : start(s), end(e) {}
	Line2D(Line2D&& instance) noexcept : start(instance.start), end(instance.end) {}
	Line2D(const Line2D& instance) noexcept : start(instance.start), end(instance.end) {}
	virtual ~Line2D() {}
	
	Line2D& operator=(Line2D&& instance) noexcept
	{
		if (this == &instance) return *this;

		start = instance.start;
		end = instance.end;

		return *this;
	}

	Line2D& operator=(const Line2D& instance) noexcept
	{
		if (this == &instance) return *this;

		start = instance.start;
		end = instance.end;

		return *this;
	}

	virtual EType GetType() const override { return EType::LINE; }
	virtual bool Intersect(const ICollision2D* target) const override;

	Vec2f start;
	Vec2f end;
};

struct Circle2D : public ICollision2D
{
	Circle2D() = default;
	Circle2D(const Vec2f& c, float r) : center(c), radius(r) {}
	Circle2D(Circle2D&& instance) noexcept : center(instance.center), radius(instance.radius) {}
	Circle2D(const Circle2D& instance) noexcept : center(instance.center), radius(instance.radius) {}
	virtual ~Circle2D() {}

	Circle2D& operator=(Circle2D&& instance) noexcept
	{
		if (this == &instance) return *this;

		center = instance.center;
		radius = instance.radius;

		return *this;
	}

	Circle2D& operator=(const Circle2D& instance) noexcept
	{
		if (this == &instance) return *this;

		center = instance.center;
		radius = instance.radius;

		return *this;
	}

	virtual EType GetType() const override { return EType::CIRCLE; }
	virtual bool Intersect(const ICollision2D* target) const override;

	Vec2f center;
	float radius;
};

}
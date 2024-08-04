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

struct Point2D : public ICollision2D
{
	Point2D() = default;
	Point2D(const Vec2f& c) : center(c) {}
	Point2D(Point2D&& instance) noexcept : center(instance.center) {}
	Point2D(const Point2D& instance) noexcept : center(instance.center) {}
	virtual ~Point2D() {}

	Point2D& operator=(Point2D&& instance) noexcept
	{
		if (this == &instance) return *this;

		center = instance.center;

		return *this;
	}

	Point2D& operator=(const Point2D& instance) noexcept
	{
		if (this == &instance) return *this;

		center = instance.center;

		return *this;
	}

	virtual EType GetType() const override { return EType::POINT; }
	virtual bool Intersect(const ICollision2D* target) const override;

	Vec2f center;
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

struct Rect2D : public ICollision2D /** 이 사각형은 AABB(Axis-Aligned Bounding Box)입니다. */
{
	Rect2D() = default;
	Rect2D(const Vec2f& c, const Vec2f& s) : center(c), size(s) {}
	Rect2D(Rect2D&& instance) noexcept : center(instance.center), size(instance.size) {}
	Rect2D(const Rect2D& instance) noexcept : center(instance.center), size(instance.size) {}
	virtual ~Rect2D() {}

	Rect2D& operator=(Rect2D&& instance) noexcept
	{
		if (this == &instance) return *this;

		center = instance.center;
		size = instance.size;

		return *this;
	}

	Rect2D& operator=(const Rect2D& instance) noexcept
	{
		if (this == &instance) return *this;

		center = instance.center;
		size = instance.size;

		return *this;
	}

	Vec2f GetMin() const { return center - size * 0.5f; }
	Vec2f GetMax() const { return center + size * 0.5f; }

	virtual EType GetType() const override { return EType::RECT; }
	virtual bool Intersect(const ICollision2D* target) const override;

	void GetInterval(const Vec2f& axis, float& outMin, float& outMax) const;

	Vec2f center;
	Vec2f size;
};

struct OrientedRect2D : public ICollision2D /** 이 사각형은 OBB(Oriented Bounding Box)입니다. */
{
	OrientedRect2D() = default;
	OrientedRect2D(const Vec2f& c, const Vec2f& s, float r) : center(c), size(s), rotate(r) {}
	OrientedRect2D(OrientedRect2D&& instance) noexcept : center(instance.center), size(instance.size), rotate(instance.rotate) {}
	OrientedRect2D(const OrientedRect2D& instance) noexcept : center(instance.center), size(instance.size), rotate(instance.rotate) {}
	virtual ~OrientedRect2D() {}

	OrientedRect2D& operator=(OrientedRect2D&& instance) noexcept
	{
		if (this == &instance) return *this;

		center = instance.center;
		size = instance.size;
		rotate = instance.rotate;

		return *this;
	}

	OrientedRect2D& operator=(const OrientedRect2D& instance) noexcept
	{
		if (this == &instance) return *this;

		center = instance.center;
		size = instance.size;
		rotate = instance.rotate;

		return *this;
	}

	virtual EType GetType() const override { return EType::ORIENTED_RECT; }
	virtual bool Intersect(const ICollision2D* target) const override;

	void GetInterval(const Vec2f& axis, float& outMin, float& outMax) const;

	Vec2f center;
	Vec2f size;
	float rotate; /** 라디안 각도 기준 */
};

}
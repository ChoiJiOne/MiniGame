#include <array>

#include "Assertion.h"
#include "Collision2D.h"

using namespace GameMaker;

/** 점과 점 끼리의 충돌 처리 */
bool IsCollision(const Point2D* point0, const Point2D* point1)
{
	return point0->center == point1->center;
}

/** 점과 선 끼리의 충돌 처리 */
bool IsCollision(const Point2D* point, const Line2D* line)
{
	if (NearZero(line->start.x - line->end.x)) /** Case 1. 직선이 y축과 평행 */
	{
		float minY = Min<float>(line->start.y, line->end.y);
		float maxY = Max<float>(line->start.y, line->end.y);

		return NearZero(line->start.x - point->center.x) && (minY <= point->center.y && point->center.y <= maxY);
	}
	else if (NearZero(line->start.y - line->end.y)) /** Case 2. 직선이 x축과 평행 */
	{
		float minX = Min<float>(line->start.x, line->end.x);
		float maxX = Max<float>(line->start.x, line->end.x);

		return NearZero(line->start.y - point->center.y) && (minX <= point->center.x && point->center.x <= maxX);
	}
	else /** Case 3. 그 이외의 모든 경우 */
	{
		float m = (line->end.y - line->start.y) / (line->end.x - line->start.x);
		float y = line->start.y + m * (point->center.x - line->start.x); /** 직선의 방정식에 대입 */

		return NearZero(y - point->center.y);
	}
}

/** 점과 원 끼리의 충돌 처리 */
bool IsCollision(const Point2D* point, const Circle2D* circle)
{
	float d2 = Vec2f::LengthSq(point->center - circle->center);
	float r2 = circle->radius * circle->radius;

	return (d2 <= r2);
}

/** 점과 AABB 끼리의 충돌 처리 */
bool IsCollision(const Point2D* point, const Rect2D* rect)
{
	Vec2f minPos = rect->GetMin();
	Vec2f maxPos = rect->GetMax();

	return (minPos.x <= point->center.x && point->center.x <= maxPos.x) && (minPos.y <= point->center.y && point->center.y <= maxPos.y);
}

/** 점과 OBB 끼리의 충돌 처리 */
bool IsCollision(const Point2D* point, const OrientedRect2D* orientedRect)
{
	Vec2f targetPos = point->center - orientedRect->center;
	float rotate = -orientedRect->rotate;

	Mat2x2 roateMat(Cos(rotate), -Sin(rotate), Sin(rotate), Cos(rotate));
	targetPos = roateMat * targetPos;

	Vec2f minPos = -orientedRect->size * 0.5f;
	Vec2f maxPos = orientedRect->size * 0.5f;

	return (minPos.x <= targetPos.x && targetPos.x <= maxPos.x) && (minPos.y <= targetPos.y && targetPos.y <= maxPos.y);
}

/** 선과 선 끼리의 충돌 처리 */
bool IsCollision(const Line2D* line0, const Line2D* line1)
{
	Vec2f p1 = line0->start;
	Vec2f p2 = line0->end;
	Vec2f p3 = line1->start;
	Vec2f p4 = line1->end;

	Vec2f p12 = p2 - p1;
	Vec2f p34 = p4 - p3;
	Vec2f p31 = p1 - p3;

	float cross = Vec2f::Cross(p12, p34);
	if (NearZero(cross)) /** 두 직선이 서로 평행한 경우. */
	{
		static auto checkBound = [](const Vec2f& minPos, const Vec2f& maxPos, const Vec2f& pos)->bool
			{
				float minX = Min<float>(minPos.x, maxPos.x);
				float minY = Min<float>(minPos.y, maxPos.y);
				float maxX = Max<float>(minPos.x, maxPos.x);
				float maxY = Max<float>(minPos.y, maxPos.y);

				return (minX <= pos.x && pos.x <= maxX) && (minY <= pos.y && pos.y <= maxY);
			};

		return (checkBound(p1, p2, p3) || checkBound(p1, p2, p4) || checkBound(p3, p4, p1) || checkBound(p3, p4, p2));
	}
	else /** 그렇지 않은 모든 경우 */
	{
		float uA = Vec2f::Cross(p34, p31) / cross;
		float uB = Vec2f::Cross(p12, p31) / cross;
		return (uA >= 0.0f && uA <= 1.0f && uB >= 0.0f && uB <= 1.0f);
	}
}

/** 선과 원 사이의 충돌 처리 */
bool IsCollision(const Line2D* line, const Circle2D* circle)
{
	static auto checkCircleBound = [](const Circle2D* circle, const Vec2f& pos)->bool
		{
			float dist2 = Vec2f::LengthSq(pos - circle->center);
			float r2 = circle->radius * circle->radius;
			return dist2 <= r2;
		};

	if (checkCircleBound(circle, line->start) || checkCircleBound(circle, line->end))
	{
		return true;
	}

	Vec2f d = line->end - line->start;
	float t = Vec2f::Dot(circle->center - line->start, d) / Vec2f::Dot(d, d);
	if (t < 0.0f || t > 1.0f)
	{
		return false;
	}

	Vec2f pos = line->start + d * t;
	pos = pos - circle->center;
	float r2 = circle->radius * circle->radius;

	return Vec2f::LengthSq(pos) <= r2;
}

/** 선과 AABB 끼리의 충돌 처리 */
bool IsCollision(const Line2D* line, const Rect2D* rect)
{
	Vec2f minPos = rect->GetMin();
	Vec2f maxPos = rect->GetMax();
	std::array<Line2D, 4> targetLines =
	{
		Line2D(Vec2f(minPos.x, minPos.y), Vec2f(maxPos.x, minPos.y)),
		Line2D(Vec2f(maxPos.x, minPos.y), Vec2f(maxPos.x, maxPos.y)),
		Line2D(Vec2f(maxPos.x, maxPos.y), Vec2f(minPos.x, maxPos.y)),
		Line2D(Vec2f(minPos.x, maxPos.y), Vec2f(minPos.x, minPos.y)),
	};

	for (const auto& targetLine : targetLines)
	{
		if (IsCollision(line, &targetLine))
		{
			return true;
		}
	}

	return false;
}

/** 선과 OBB 끼리의 충돌 처리 */
bool IsCollision(const Line2D* line, const OrientedRect2D* orientedRect)
{
	float rotate = -orientedRect->rotate;
	Mat2x2 roateMat(Cos(rotate), -Sin(rotate), Sin(rotate), Cos(rotate));

	Vec2f start = roateMat * (line->start - orientedRect->center);
	Vec2f end = roateMat * (line->end - orientedRect->center);

	Line2D targetLine(start, end);
	Rect2D targetRect(Vec2f(0.0f, 0.0f), orientedRect->size);

	return IsCollision(&targetLine, &targetRect);
}

/** 원과 원 끼리의 충돌 처리 */
bool IsCollision(const Circle2D* circle0, const Circle2D* circle1)
{
	float dist = Vec2f::LengthSq(circle1->center - circle0->center);
	float radiusSum = circle0->radius + circle1->radius;

	return dist <= radiusSum * radiusSum;
}

/** 원과 AABB 끼리의 충돌 처리 */
bool IsCollision(const Circle2D* circle, const Rect2D* rect)
{
	Vec2f minPos = rect->GetMin();
	Vec2f maxPos = rect->GetMax();
	Vec2f closest(Clamp<float>(circle->center.x, minPos.x, maxPos.x), Clamp<float>(circle->center.y, minPos.y, maxPos.y));

	float dist = Vec2f::LengthSq(closest - circle->center);
	float r2 = circle->radius * circle->radius;

	return dist <= r2;
}

bool Point2D::Intersect(const ICollision2D* target) const
{
	CHECK(target != nullptr);

	EType type = target->GetType();
	bool bIsIntersect = false;

	switch (type)
	{
	case ICollision2D::EType::POINT:
	{
		const Point2D* other = reinterpret_cast<const Point2D*>(target);
		bIsIntersect = IsCollision(this, other);
		break;
	}

	case ICollision2D::EType::LINE:
	{
		const Line2D* other = reinterpret_cast<const Line2D*>(target);
		bIsIntersect = IsCollision(this, other);
		break;
	}

	case ICollision2D::EType::CIRCLE: 
	{
		const Circle2D* other = reinterpret_cast<const Circle2D*>(target);
		bIsIntersect = IsCollision(this, other);
		break;
	}

	case ICollision2D::EType::RECT:
	{
		const Rect2D* other = reinterpret_cast<const Rect2D*>(target);
		bIsIntersect = IsCollision(this, other);
		break;
	}

	case ICollision2D::EType::ORIENTED_RECT:
	{
		const OrientedRect2D* other = reinterpret_cast<const OrientedRect2D*>(target);
		bIsIntersect = IsCollision(this, other);
		break;
	}
	break;

	default: break;// ICollision2D::EType::NONE:
	}

	return bIsIntersect;
}

bool Line2D::Intersect(const ICollision2D* target) const
{
	CHECK(target != nullptr);

	EType type = target->GetType();
	bool bIsIntersect = false;

	switch (type)
	{
	case ICollision2D::EType::POINT:
	{
		const Point2D* other = reinterpret_cast<const Point2D*>(target);
		bIsIntersect = IsCollision(other, this);
		break;
	}

	case ICollision2D::EType::LINE:
	{
		const Line2D* other = reinterpret_cast<const Line2D*>(target);
		bIsIntersect = IsCollision(this, other);
		break;
	}

	case ICollision2D::EType::CIRCLE:
	{
		const Circle2D* other = reinterpret_cast<const Circle2D*>(target);
		bIsIntersect = IsCollision(this, other);
		break;
	}

	case ICollision2D::EType::RECT:
	{
		const Rect2D* other = reinterpret_cast<const Rect2D*>(target);
		bIsIntersect = IsCollision(this, other);
		break;
	}

	case ICollision2D::EType::ORIENTED_RECT:
	{
		const OrientedRect2D* other = reinterpret_cast<const OrientedRect2D*>(target);
		bIsIntersect = IsCollision(this, other);
		break;
	}

	default: break;// ICollision2D::EType::NONE:
	}

	return bIsIntersect;
}

bool Circle2D::Intersect(const ICollision2D* target) const
{
	CHECK(target != nullptr);

	EType type = target->GetType();
	bool bIsIntersect = false;

	switch (type)
	{
	case ICollision2D::EType::POINT:
	{
		const Point2D* other = reinterpret_cast<const Point2D*>(target);
		bIsIntersect = IsCollision(other, this);
		break;
	}

	case ICollision2D::EType::LINE:
	{
		const Line2D* other = reinterpret_cast<const Line2D*>(target);
		bIsIntersect = IsCollision(other, this);
		break;
	}

	case ICollision2D::EType::CIRCLE:
	{
		const Circle2D* other = reinterpret_cast<const Circle2D*>(target);
		bIsIntersect = IsCollision(this, other);
		break;
	}

	case ICollision2D::EType::RECT:
	{
		const Rect2D* other = reinterpret_cast<const Rect2D*>(target);
		bIsIntersect = IsCollision(this, other);
		break;
	}

	case ICollision2D::EType::ORIENTED_RECT:
	{
		break;
	}

	default: break;// ICollision2D::EType::NONE:
	}

	return bIsIntersect;
}

bool Rect2D::Intersect(const ICollision2D* target) const
{
	CHECK(target != nullptr);

	EType type = target->GetType();
	bool bIsIntersect = false;

	switch (type)
	{
	case ICollision2D::EType::POINT:
	{
		break;
	}

	case ICollision2D::EType::LINE:
	{
		break;
	}

	case ICollision2D::EType::CIRCLE:
	{
		break;
	}

	case ICollision2D::EType::RECT:
	{
		break;
	}

	case ICollision2D::EType::ORIENTED_RECT:
	{
		break;
	}

	default: break;// ICollision2D::EType::NONE:
	}

	return bIsIntersect;
}

bool OrientedRect2D::Intersect(const ICollision2D* target) const
{
	CHECK(target != nullptr);

	EType type = target->GetType();
	bool bIsIntersect = false;

	switch (type)
	{
	case ICollision2D::EType::POINT:
	{
		break;
	}

	case ICollision2D::EType::LINE:
	{
		break;
	}

	case ICollision2D::EType::CIRCLE:
	{
		break;
	}

	case ICollision2D::EType::RECT:
	{
		break;
	}

	case ICollision2D::EType::ORIENTED_RECT:
	{
		break;
	}

	default: break;// ICollision2D::EType::NONE:
	}

	return bIsIntersect;
}

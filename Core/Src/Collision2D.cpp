#include <array>

#include "Assertion.h"
#include "Collision2D.h"

/** 점과 점 끼리의 충돌 처리 */
bool IsCollision(const Point2D* point0, const Point2D* point1)
{
	return point0->center == point1->center;
}

/** 점과 선 끼리의 충돌 처리 */
bool IsCollision(const Point2D* point, const Line2D* line)
{
	if (GameMath::NearZero(line->start.x - line->end.x)) /** Case 1. 직선이 y축과 평행 */
	{
		float minY = GameMath::Min<float>(line->start.y, line->end.y);
		float maxY = GameMath::Max<float>(line->start.y, line->end.y);

		return GameMath::NearZero(line->start.x - point->center.x) && (minY <= point->center.y && point->center.y <= maxY);
	}
	else if (GameMath::NearZero(line->start.y - line->end.y)) /** Case 2. 직선이 x축과 평행 */
	{
		float minX = GameMath::Min<float>(line->start.x, line->end.x);
		float maxX = GameMath::Max<float>(line->start.x, line->end.x);

		return GameMath::NearZero(line->start.y - point->center.y) && (minX <= point->center.x && point->center.x <= maxX);
	}
	else /** Case 3. 그 이외의 모든 경우 */
	{
		float m = (line->end.y - line->start.y) / (line->end.x - line->start.x);
		float y = line->start.y + m * (point->center.x - line->start.x); /** 직선의 방정식에 대입 */

		return GameMath::NearZero(y - point->center.y);
	}
}

/** 점과 원 끼리의 충돌 처리 */
bool IsCollision(const Point2D* point, const Circle2D* circle)
{
	float d2 = GameMath::Vec2f::LengthSq(point->center - circle->center);
	float r2 = circle->radius * circle->radius;

	return (d2 <= r2);
}

/** 점과 AABB 끼리의 충돌 처리 */
bool IsCollision(const Point2D* point, const Rect2D* rect)
{
	GameMath::Vec2f minPos = rect->GetMin();
	GameMath::Vec2f maxPos = rect->GetMax();

	return (minPos.x <= point->center.x && point->center.x <= maxPos.x) && (minPos.y <= point->center.y && point->center.y <= maxPos.y);
}

/** 점과 OBB 끼리의 충돌 처리 */
bool IsCollision(const Point2D* point, const OrientedRect2D* orientedRect)
{
	GameMath::Vec2f targetPos = point->center - orientedRect->center;
	float rotate = -orientedRect->rotate;

	GameMath::Mat2x2 roateMat(
		GameMath::Cos(rotate), -GameMath::Sin(rotate), 
		GameMath::Sin(rotate), GameMath::Cos(rotate)
	);
	targetPos = roateMat * targetPos;

	GameMath::Vec2f minPos = -orientedRect->size * 0.5f;
	GameMath::Vec2f maxPos = orientedRect->size * 0.5f;

	return (minPos.x <= targetPos.x && targetPos.x <= maxPos.x) && (minPos.y <= targetPos.y && targetPos.y <= maxPos.y);
}

/** 선과 선 끼리의 충돌 처리 */
bool IsCollision(const Line2D* line0, const Line2D* line1)
{
	GameMath::Vec2f p1 = line0->start;
	GameMath::Vec2f p2 = line0->end;
	GameMath::Vec2f p3 = line1->start;
	GameMath::Vec2f p4 = line1->end;

	GameMath::Vec2f p12 = p2 - p1;
	GameMath::Vec2f p34 = p4 - p3;
	GameMath::Vec2f p31 = p1 - p3;

	float cross = GameMath::Vec2f::Cross(p12, p34);
	if (GameMath::NearZero(cross)) /** 두 직선이 서로 평행한 경우. */
	{
		static auto checkBound = [](const GameMath::Vec2f& minPos, const GameMath::Vec2f& maxPos, const GameMath::Vec2f& pos)->bool
			{
				float minX = GameMath::Min<float>(minPos.x, maxPos.x);
				float minY = GameMath::Min<float>(minPos.y, maxPos.y);
				float maxX = GameMath::Max<float>(minPos.x, maxPos.x);
				float maxY = GameMath::Max<float>(minPos.y, maxPos.y);

				return (minX <= pos.x && pos.x <= maxX) && (minY <= pos.y && pos.y <= maxY);
			};

		return (checkBound(p1, p2, p3) || checkBound(p1, p2, p4) || checkBound(p3, p4, p1) || checkBound(p3, p4, p2));
	}
	else /** 그렇지 않은 모든 경우 */
	{
		float uA = GameMath::Vec2f::Cross(p34, p31) / cross;
		float uB = GameMath::Vec2f::Cross(p12, p31) / cross;
		return (uA >= 0.0f && uA <= 1.0f && uB >= 0.0f && uB <= 1.0f);
	}
}

/** 선과 원 사이의 충돌 처리 */
bool IsCollision(const Line2D* line, const Circle2D* circle)
{
	static auto checkCircleBound = [](const Circle2D* circle, const GameMath::Vec2f& pos)->bool
		{
			float dist2 = GameMath::Vec2f::LengthSq(pos - circle->center);
			float r2 = circle->radius * circle->radius;
			return dist2 <= r2;
		};

	if (checkCircleBound(circle, line->start) || checkCircleBound(circle, line->end))
	{
		return true;
	}

	GameMath::Vec2f d = line->end - line->start;
	float t = GameMath::Vec2f::Dot(circle->center - line->start, d) / GameMath::Vec2f::Dot(d, d);
	if (t < 0.0f || t > 1.0f)
	{
		return false;
	}

	GameMath::Vec2f pos = line->start + d * t;
	pos = pos - circle->center;
	float r2 = circle->radius * circle->radius;

	return GameMath::Vec2f::LengthSq(pos) <= r2;
}

/** 선과 AABB 끼리의 충돌 처리 */
bool IsCollision(const Line2D* line, const Rect2D* rect)
{
	GameMath::Vec2f minPos = rect->GetMin();
	GameMath::Vec2f maxPos = rect->GetMax();
	std::array<Line2D, 4> targetLines =
	{
		Line2D(GameMath::Vec2f(minPos.x, minPos.y), GameMath::Vec2f(maxPos.x, minPos.y)),
		Line2D(GameMath::Vec2f(maxPos.x, minPos.y), GameMath::Vec2f(maxPos.x, maxPos.y)),
		Line2D(GameMath::Vec2f(maxPos.x, maxPos.y), GameMath::Vec2f(minPos.x, maxPos.y)),
		Line2D(GameMath::Vec2f(minPos.x, maxPos.y), GameMath::Vec2f(minPos.x, minPos.y)),
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
	GameMath::Mat2x2 roateMat(
		GameMath::Cos(rotate), -GameMath::Sin(rotate), 
		GameMath::Sin(rotate), GameMath::Cos(rotate)
	);

	GameMath::Vec2f start = roateMat * (line->start - orientedRect->center);
	GameMath::Vec2f end = roateMat * (line->end - orientedRect->center);

	Line2D targetLine(start, end);
	Rect2D targetRect(GameMath::Vec2f(0.0f, 0.0f), orientedRect->size);

	return IsCollision(&targetLine, &targetRect);
}

/** 원과 원 끼리의 충돌 처리 */
bool IsCollision(const Circle2D* circle0, const Circle2D* circle1)
{
	float dist = GameMath::Vec2f::LengthSq(circle1->center - circle0->center);
	float radiusSum = circle0->radius + circle1->radius;

	return dist <= radiusSum * radiusSum;
}

/** 원과 AABB 끼리의 충돌 처리 */
bool IsCollision(const Circle2D* circle, const Rect2D* rect)
{
	GameMath::Vec2f minPos = rect->GetMin();
	GameMath::Vec2f maxPos = rect->GetMax();
	GameMath::Vec2f closest(GameMath::Clamp<float>(circle->center.x, minPos.x, maxPos.x), GameMath::Clamp<float>(circle->center.y, minPos.y, maxPos.y));

	float dist = GameMath::Vec2f::LengthSq(closest - circle->center);
	float r2 = circle->radius * circle->radius;

	return dist <= r2;
}

/** 원과 OBB 끼리의 충돌 처리 */
bool IsCollision(const Circle2D* circle, const OrientedRect2D* orientedRect)
{
	float rotate = -orientedRect->rotate;
	GameMath::Mat2x2 roateMat(
		GameMath::Cos(rotate), -GameMath::Sin(rotate), 
		GameMath::Sin(rotate), GameMath::Cos(rotate)
	);

	GameMath::Vec2f center = roateMat * (circle->center - orientedRect->center);

	Circle2D targetCircle(center, circle->radius);
	Rect2D targetRect(GameMath::Vec2f(0.0f, 0.0f), orientedRect->size);
	
	return IsCollision(&targetCircle, &targetRect);
}

/** AABB와 AABB 끼리의 충돌 처리 */
bool IsCollision(const Rect2D* rect0, const Rect2D* rect1)
{
	GameMath::Vec2f minPos0 = rect0->GetMin();
	GameMath::Vec2f maxPos0 = rect0->GetMax();
	GameMath::Vec2f minPos1 = rect1->GetMin();
	GameMath::Vec2f maxPos1 = rect1->GetMax();

	bool bIsOverlapX = ((minPos1.x <= maxPos0.x) && (minPos0.x <= maxPos1.x));
	bool bIsOverlapY = ((minPos1.y <= maxPos0.y) && (minPos0.y <= maxPos1.y));

	return bIsOverlapX && bIsOverlapY;
}

/** 축을 기준으로 AABB와 OBB가 오버랩(겹치는지) 확인 */
bool IsOverlapOnAxis(const Rect2D* rect, const OrientedRect2D* orientedRect, const GameMath::Vec2f& axis)
{
	float rectMin = 0.0f;
	float rectMax = 0.0f;
	rect->GetInterval(axis, rectMin, rectMax);

	float orientedRectMin = 0.0f;
	float orientedRectMax = 0.0f;
	orientedRect->GetInterval(axis, orientedRectMin, orientedRectMax);

	return ((orientedRectMin <= rectMax) && (rectMin <= orientedRectMax));
}

/** AABB와 OBB 끼리의 충돌 처리 */
bool IsCollision(const Rect2D* rect, const OrientedRect2D* orientedRect)
{
	float rotate = orientedRect->rotate;
	GameMath::Mat2x2 roateMat(
		GameMath::Cos(rotate), -GameMath::Sin(rotate), 
		GameMath::Sin(rotate), GameMath::Cos(rotate)
	);

	GameMath::Vec2f axis0(orientedRect->size.x * 0.5f, 0.0f);
	GameMath::Vec2f axis1(0.0f, orientedRect->size.y * 0.5f);

	std::array<GameMath::Vec2f, 4> separateAxis =
	{
		GameMath::Vec2f(1.0f, 0.0f),
		GameMath::Vec2f(0.0f, 1.0f),
		roateMat * GameMath::Vec2f::Normalize(axis0),
		roateMat * GameMath::Vec2f::Normalize(axis1),
	};

	for (const auto& axis : separateAxis)
	{
		if (!IsOverlapOnAxis(rect, orientedRect, axis))
		{
			return false;
		}
	}

	return true;
}

/** 축을 기준으로 OBB와 OBB가 오버랩(겹치는지) 확인 */
bool IsOverlapOnAxis(const OrientedRect2D* orientedRect0, const OrientedRect2D* orientedRect1, const GameMath::Vec2f& axis)
{
	float orientedRectMin0 = 0.0f;
	float orientedRectMax0 = 0.0f;
	orientedRect0->GetInterval(axis, orientedRectMin0, orientedRectMax0);

	float orientedRectMin1 = 0.0f;
	float orientedRectMax1 = 0.0f;
	orientedRect1->GetInterval(axis, orientedRectMin1, orientedRectMax1);

	return ((orientedRectMin1 <= orientedRectMax0) && (orientedRectMin0 <= orientedRectMax1));
}

/** OBB와 OBB 끼리의 충돌 처리 */
bool IsCollision(const OrientedRect2D* orientedRect0, const OrientedRect2D* orientedRect1)
{
	float rotate0 = orientedRect0->rotate;
	GameMath::Mat2x2 roateMat0(GameMath::Cos(rotate0), -GameMath::Sin(rotate0), GameMath::Sin(rotate0), GameMath::Cos(rotate0));

	float rotate1 = orientedRect1->rotate;
	GameMath::Mat2x2 roateMat1(GameMath::Cos(rotate1), -GameMath::Sin(rotate1), GameMath::Sin(rotate1), GameMath::Cos(rotate1));

	std::array<GameMath::Vec2f, 6> separateAxis =
	{ 
		GameMath::Vec2f(1.0f, 0.0f),
		GameMath::Vec2f(0.0f, 1.0f),
		roateMat0 * GameMath::Vec2f::Normalize(GameMath::Vec2f(orientedRect0->size.x * 0.5f, 0.0f)),
		roateMat0 * GameMath::Vec2f::Normalize(GameMath::Vec2f(0.0f, orientedRect0->size.y * 0.5f)),
		roateMat1 * GameMath::Vec2f::Normalize(GameMath::Vec2f(orientedRect1->size.x * 0.5f, 0.0f)),
		roateMat1 * GameMath::Vec2f::Normalize(GameMath::Vec2f(0.0f, orientedRect1->size.y * 0.5f)),
	};

	for (const auto& axis : separateAxis)
	{
		if (!IsOverlapOnAxis(orientedRect0, orientedRect1, axis))
		{
			return false;
		}
	}

	return true;
}

bool Point2D::Intersect(const ICollision2D* target) const
{
	CHECK(target != nullptr);

	Type type = target->GetType();
	bool bIsIntersect = false;

	switch (type)
	{
	case ICollision2D::Type::POINT:
	{
		const Point2D* other = reinterpret_cast<const Point2D*>(target);
		bIsIntersect = IsCollision(this, other);
		break;
	}

	case ICollision2D::Type::LINE:
	{
		const Line2D* other = reinterpret_cast<const Line2D*>(target);
		bIsIntersect = IsCollision(this, other);
		break;
	}

	case ICollision2D::Type::CIRCLE: 
	{
		const Circle2D* other = reinterpret_cast<const Circle2D*>(target);
		bIsIntersect = IsCollision(this, other);
		break;
	}

	case ICollision2D::Type::RECT:
	{
		const Rect2D* other = reinterpret_cast<const Rect2D*>(target);
		bIsIntersect = IsCollision(this, other);
		break;
	}

	case ICollision2D::Type::ORIENTED_RECT:
	{
		const OrientedRect2D* other = reinterpret_cast<const OrientedRect2D*>(target);
		bIsIntersect = IsCollision(this, other);
		break;
	}
	break;

	default: break;// ICollision2D::Type::NONE:
	}

	return bIsIntersect;
}

bool Line2D::Intersect(const ICollision2D* target) const
{
	CHECK(target != nullptr);

	Type type = target->GetType();
	bool bIsIntersect = false;

	switch (type)
	{
	case ICollision2D::Type::POINT:
	{
		const Point2D* other = reinterpret_cast<const Point2D*>(target);
		bIsIntersect = IsCollision(other, this);
		break;
	}

	case ICollision2D::Type::LINE:
	{
		const Line2D* other = reinterpret_cast<const Line2D*>(target);
		bIsIntersect = IsCollision(this, other);
		break;
	}

	case ICollision2D::Type::CIRCLE:
	{
		const Circle2D* other = reinterpret_cast<const Circle2D*>(target);
		bIsIntersect = IsCollision(this, other);
		break;
	}

	case ICollision2D::Type::RECT:
	{
		const Rect2D* other = reinterpret_cast<const Rect2D*>(target);
		bIsIntersect = IsCollision(this, other);
		break;
	}

	case ICollision2D::Type::ORIENTED_RECT:
	{
		const OrientedRect2D* other = reinterpret_cast<const OrientedRect2D*>(target);
		bIsIntersect = IsCollision(this, other);
		break;
	}

	default: break;// ICollision2D::Type::NONE:
	}

	return bIsIntersect;
}

bool Circle2D::Intersect(const ICollision2D* target) const
{
	CHECK(target != nullptr);

	Type type = target->GetType();
	bool bIsIntersect = false;

	switch (type)
	{
	case ICollision2D::Type::POINT:
	{
		const Point2D* other = reinterpret_cast<const Point2D*>(target);
		bIsIntersect = IsCollision(other, this);
		break;
	}

	case ICollision2D::Type::LINE:
	{
		const Line2D* other = reinterpret_cast<const Line2D*>(target);
		bIsIntersect = IsCollision(other, this);
		break;
	}

	case ICollision2D::Type::CIRCLE:
	{
		const Circle2D* other = reinterpret_cast<const Circle2D*>(target);
		bIsIntersect = IsCollision(this, other);
		break;
	}

	case ICollision2D::Type::RECT:
	{
		const Rect2D* other = reinterpret_cast<const Rect2D*>(target);
		bIsIntersect = IsCollision(this, other);
		break;
	}

	case ICollision2D::Type::ORIENTED_RECT:
	{
		const OrientedRect2D* other = reinterpret_cast<const OrientedRect2D*>(target);
		bIsIntersect = IsCollision(this, other);
		break;
	}

	default: break;// ICollision2D::Type::NONE:
	}

	return bIsIntersect;
}

bool Rect2D::Intersect(const ICollision2D* target) const
{
	CHECK(target != nullptr);

	Type type = target->GetType();
	bool bIsIntersect = false;

	switch (type)
	{
	case ICollision2D::Type::POINT:
	{
		const Point2D* other = reinterpret_cast<const Point2D*>(target);
		bIsIntersect = IsCollision(other, this);
		break;
	}

	case ICollision2D::Type::LINE:
	{
		const Line2D* other = reinterpret_cast<const Line2D*>(target);
		bIsIntersect = IsCollision(other, this);
		break;
	}

	case ICollision2D::Type::CIRCLE:
	{
		const Circle2D* other = reinterpret_cast<const Circle2D*>(target);
		bIsIntersect = IsCollision(other, this);
		break;
	}

	case ICollision2D::Type::RECT:
	{
		const Rect2D* other = reinterpret_cast<const Rect2D*>(target);
		bIsIntersect = IsCollision(this, other);
		break;
	}

	case ICollision2D::Type::ORIENTED_RECT:
	{
		const OrientedRect2D* other = reinterpret_cast<const OrientedRect2D*>(target);
		bIsIntersect = IsCollision(this, other);
		break;
	}

	default: break;// ICollision2D::Type::NONE:
	}

	return bIsIntersect;
}

void Rect2D::GetInterval(const GameMath::Vec2f& axis, float& outMin, float& outMax) const
{
	GameMath::Vec2f minPos = GetMin();
	GameMath::Vec2f maxPos = GetMax();

	std::array<GameMath::Vec2f, 4> vertices =
	{
		GameMath::Vec2f(minPos.x, minPos.y),
		GameMath::Vec2f(minPos.x, maxPos.y),
		GameMath::Vec2f(maxPos.x, maxPos.y),
		GameMath::Vec2f(maxPos.x, minPos.y),
	};
	
	outMin = +FLT_MAX;
	outMax = -FLT_MAX;

	for (int32_t index = 0; index < vertices.size(); ++index)
	{
		float projection = GameMath::Vec2f::Dot(axis, vertices[index]);

		if (projection < outMin)
		{
			outMin = projection;
		}

		if (projection > outMax)
		{
			outMax = projection;
		}
	}
}

bool OrientedRect2D::Intersect(const ICollision2D* target) const
{
	CHECK(target != nullptr);

	Type type = target->GetType();
	bool bIsIntersect = false;

	switch (type)
	{
	case ICollision2D::Type::POINT:
	{
		const Point2D* other = reinterpret_cast<const Point2D*>(target);
		bIsIntersect = IsCollision(other, this);
		break;
	}

	case ICollision2D::Type::LINE:
	{
		const Line2D* other = reinterpret_cast<const Line2D*>(target);
		bIsIntersect = IsCollision(other, this);
		break;
	}

	case ICollision2D::Type::CIRCLE:
	{
		const Circle2D* other = reinterpret_cast<const Circle2D*>(target);
		bIsIntersect = IsCollision(other, this);
		break;
	}

	case ICollision2D::Type::RECT:
	{
		const Rect2D* other = reinterpret_cast<const Rect2D*>(target);
		bIsIntersect = IsCollision(other, this);
		break;
	}

	case ICollision2D::Type::ORIENTED_RECT:
	{
		const OrientedRect2D* other = reinterpret_cast<const OrientedRect2D*>(target);
		bIsIntersect = IsCollision(this, other);
		break;
	}

	default: break;// ICollision2D::Type::NONE:
	}

	return bIsIntersect;
}

void OrientedRect2D::GetInterval(const GameMath::Vec2f& axis, float& outMin, float& outMax) const
{
	Rect2D rect(center, size);
	GameMath::Vec2f minPos = rect.GetMin();
	GameMath::Vec2f maxPos = rect.GetMax();

	std::array<GameMath::Vec2f, 4> vertices =
	{
		GameMath::Vec2f(minPos.x, minPos.y),
		GameMath::Vec2f(minPos.x, maxPos.y),
		GameMath::Vec2f(maxPos.x, maxPos.y),
		GameMath::Vec2f(maxPos.x, minPos.y),
	};

	GameMath::Mat2x2 roateMat(GameMath::Cos(rotate), -GameMath::Sin(rotate), GameMath::Sin(rotate), GameMath::Cos(rotate));
	for (int32_t index = 0; index < vertices.size(); ++index)
	{
		GameMath::Vec2f targetPos = roateMat * (vertices[index] - center);
		vertices[index] = targetPos + center;
	}

	outMin = +FLT_MAX;
	outMax = -FLT_MAX;

	for (int32_t index = 0; index < vertices.size(); ++index)
	{
		float projection = GameMath::Vec2f::Dot(axis, vertices[index]);

		if (projection < outMin)
		{
			outMin = projection;
		}

		if (projection > outMax)
		{
			outMax = projection;
		}
	}

}

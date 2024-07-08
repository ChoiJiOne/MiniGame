#include "Assertion.h"
#include "Collision2D.h"

using namespace GameMaker;

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
		bIsIntersect = (center == other->center);
		break;
	}

	case ICollision2D::EType::LINE:
	{
		const Line2D* other = reinterpret_cast<const Line2D*>(target);

		if (NearZero(other->start.x - other->end.x)) /** Case 1. 직선이 y축과 평행 */
		{
			float minY = Min<float>(other->start.y, other->end.y);
			float maxY = Max<float>(other->start.y, other->end.y);

			bIsIntersect = NearZero(other->start.x - center.x) && (minY <= center.y && center.y <= maxY);
		}
		else if (NearZero(other->start.y - other->end.y)) /** Case 2. 직선이 x축과 평행 */
		{
			float minX = Min<float>(other->start.x, other->end.x);
			float maxX = Max<float>(other->start.x, other->end.x);

			bIsIntersect = NearZero(other->start.y - center.y) && (minX <= center.x && center.x <= maxX);
		}
		else /** Case 3. 그 이외의 모든 경우 */
		{
			float m = (other->end.y - other->start.y) / (other->end.x - other->start.x);
			float y = other->start.y + m * (center.x - other->start.x); /** 직선의 방정식에 대입 */

			bIsIntersect = NearZero(y - center.y);
		}
		break;
	}

	case ICollision2D::EType::CIRCLE: 
	{
		const Circle2D* other = reinterpret_cast<const Circle2D*>(target);
		float d2 = Vec2f::LengthSq(center - other->center);
		float r2 = other->radius * other->radius;

		bIsIntersect = (d2 <= r2);
		break;
	}

	case ICollision2D::EType::RECT:
	{
		const Rect2D* other = reinterpret_cast<const Rect2D*>(target);
		Vec2f minPos = other->GetMin();
		Vec2f maxPos = other->GetMax();

		bIsIntersect = (minPos.x <= center.x && center.x <= maxPos.x) && (minPos.y <= center.y && center.y <= maxPos.y);
		break;
	}

	case ICollision2D::EType::ORIENTED_RECT:
	{
		const OrientedRect2D* other = reinterpret_cast<const OrientedRect2D*>(target);
		Vec2f targetPos = center - other->center;
		float rotate = -other->rotate;

		Mat2x2 roateMat(Cos(rotate), -Sin(rotate), Sin(rotate), Cos(rotate));
		targetPos = roateMat * targetPos;

		Vec2f minPos = -other->size * 0.5f;
		Vec2f maxPos = other->size * 0.5f;

		bIsIntersect = (minPos.x <= targetPos.x && targetPos.x <= maxPos.x) && (minPos.y <= targetPos.y && targetPos.y <= maxPos.y);
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
		bIsIntersect = other->Intersect(this);
		break;
	}

	case ICollision2D::EType::LINE:
	{
		const Line2D* other = reinterpret_cast<const Line2D*>(target);

		Vec2f p1 = start;
		Vec2f p2 = end;
		Vec2f p3 = other->start;
		Vec2f p4 = other->end;

		Vec2f p12 = p2 - p1;
		Vec2f p34 = p4 - p3;
		Vec2f p31 = p1 - p3;

		float cross = Vec2f::Cross(p12, p34);
		if (NearZero(cross))
		{
			static auto checkBound = [](const Vec2f& minPos, const Vec2f& maxPos, const Vec2f& pos) -> bool
				{
					float minX = Min<float>(minPos.x, maxPos.x);
					float minY = Min<float>(minPos.y, maxPos.y);
					float maxX = Max<float>(minPos.x, maxPos.x);
					float maxY = Max<float>(minPos.y, maxPos.y);

					return (minX <= pos.x && pos.x <= maxX) && (minY <= pos.y && pos.y <= maxY);
				};

			bIsIntersect = (checkBound(p1, p2, p3) || checkBound(p1, p2, p4) || checkBound(p3, p4, p1) || checkBound(p3, p4, p2));

		}
		else
		{
			float uA = Vec2f::Cross(p34, p31) / cross;
			float uB = Vec2f::Cross(p12, p31) / cross;
			bIsIntersect = (uA >= 0.0f && uA <= 1.0f && uB >= 0.0f && uB <= 1.0f);
		}
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

bool Circle2D::Intersect(const ICollision2D* target) const
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

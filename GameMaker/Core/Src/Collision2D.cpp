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
	return false;
}

bool Circle2D::Intersect(const ICollision2D* target) const
{
	return false;
}

bool Rect2D::Intersect(const ICollision2D* target) const
{
	return false;
}

bool OrientedRect2D::Intersect(const ICollision2D* target) const
{
	return false;
}

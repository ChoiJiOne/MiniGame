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
		break;
	}

	case ICollision2D::EType::ORIENTED_RECT:
	{
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

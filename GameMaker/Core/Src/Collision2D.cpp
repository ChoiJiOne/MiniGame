#include "Assertion.h"
#include "Collision2D.h"

using namespace GameMaker;

bool Line2D::Intersect(const ICollision2D* target) const
{
	return false;
}

bool Circle2D::Intersect(const ICollision2D* target) const
{
	return false;
}
